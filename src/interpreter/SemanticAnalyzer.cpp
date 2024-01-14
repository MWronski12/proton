#include <cassert>
#include <iostream>

#include "ASTNode.h"
#include "ASTVisitor.h"
#include "Definition.h"
#include "Environment.h"
#include "ErrorHandler.h"
#include "Expression.h"
#include "Program.h"
#include "SemanticAnalyzer.h"
#include "Statement.h"

namespace Interpreter {

using namespace std::string_literals;

/* --------------------------- TypeChecking Utils --------------------------- */

bool isAssignable(const TypePtr& objType, const TypePtr& assignedTo);

bool variantIsAssignable(const TypePtr& objType, const TypePtr& assignedTo) {
  assert(objType != nullptr && assignedTo != nullptr && "Types should not be null!");
  assert(std::holds_alternative<Variant>(assignedTo->type) && "Helper function only for variants");

  // Variant is assignable to itself
  if (objType == assignedTo) return true;

  // Variant is assignable to any of its types
  auto variantType = std::get<Variant>(assignedTo->type);
  for (const auto& type : variantType.types) {
    if (objType == type) return true;
  }
  return false;
}

bool structIsAssignable(const TypePtr& objType, const TypePtr& assignedTo) {
  assert(objType != nullptr && assignedTo != nullptr && "Types should not be null!");
  assert(std::holds_alternative<Struct>(assignedTo->type) && "Helper function only for structs");

  // Struct is assignable to itself
  if (objType == assignedTo) return true;

  // Struct is assignable to any struct with a subset of its members
  if (!std::holds_alternative<Struct>(objType->type)) return false;
  const auto& assignedStruct = std::get<Struct>(objType->type);
  const auto& assignedToStruct = std::get<Struct>(assignedTo->type);

  for (const auto& [name, assignedToMember] : assignedToStruct.members) {
    if (!assignedStruct.members.contains(name)) return false;
    if (!isAssignable(assignedStruct.members.at(name), assignedToMember)) return false;
  }
  return true;
}

bool isAssignable(const TypePtr& objType, const TypePtr& assignedTo) {
  return std::visit(
      overloaded{// Void and FnSignature are not assignable
                 [](const Void&) { return false; }, [](const FnSignature&) { return false; },
                 // Struct and Variant are handled separately
                 [&objType, &assignedTo](const Struct&) {
                   return structIsAssignable(objType, assignedTo);
                 },
                 [&objType, &assignedTo](const Variant&) {
                   return variantIsAssignable(objType, assignedTo);
                 },
                 // Primitive types simply have to be the same
                 [&objType, &assignedTo](const auto&) { return objType == assignedTo; }},
      assignedTo->type);
}

/* ----------------------------- Helper methods ----------------------------- */

void SemanticAnalyzer::expect(bool condition, ErrorType error, Position position) const {
  if (!condition) {
    m_error(error, position);
  }
}

bool SemanticAnalyzer::exprIsAssignable(const std::unique_ptr<::Expression>& expr) {
  // We can only assign values to variables
  if (auto idExpr = dynamic_cast<IdentifierExpr*>(expr.get()); idExpr != nullptr) {
    auto var = m_env.getVar(idExpr->name);
    expect(var != std::nullopt, ErrorType::UNDEFINED_VARIABLE, expr->position);
    expect(!(*var)->isConst(), ErrorType::CONST_ASSIGNMENT, expr->position);
  }
  // or object members
  else if (auto memberAccess = dynamic_cast<MemberAccessPostfix*>(expr.get());
           memberAccess != nullptr) {
    expect(m_env.containsVar(memberAccess->member), ErrorType::UNDEFINED_VARIABLE, expr->position);
  }
  // If its neither, assignment doesnt make sense
  else {
    expect(false, ErrorType::INVALID_ASSIGNMENT, expr->position);
  }

  return true;
}

/* -------------------------------------------------------------------------- */
/*                              SemanticAnalyzer                              */
/* -------------------------------------------------------------------------- */

SemanticAnalyzer::SemanticAnalyzer(::ErrorHandler& error) : m_error{error} {}

/* --------------------------- Visitation methods --------------------------- */

void SemanticAnalyzer::visit(::Program& program) {
  for (auto& definition : program.definitions) {
    definition->accept(*this);
  }
}

void SemanticAnalyzer::visit(::VarDef& var) {
  expect(!m_env.nameConflict(var.name), ErrorType::REDEFINITION, var.position);
  expect(m_env.containsType(var.type), ErrorType::UNDEFINED_TYPE, var.position);

  auto varType = m_env.getType(var.type);

  var.value->accept(*this);
  auto exprType = m_env.getLastExpressionType();
  assert(exprType != std::nullopt && varType != std::nullopt &&
         "Visiting expression should set Environment::lastExpressionType");

  expect(isAssignable(*exprType, *varType), ErrorType::ASSIGNMENT_TYPE_MISMATCH, var.position);

  m_env.insertVar(var.name, Variable{var.name, *varType});
}

void SemanticAnalyzer::visit(::ConstDef& constant) {
  expect(!m_env.nameConflict(constant.name), ErrorType::REDEFINITION, constant.position);
  expect(m_env.containsType(constant.type), ErrorType::UNDEFINED_TYPE, constant.position);

  auto constantType = m_env.getType(constant.type);

  constant.value->accept(*this);
  auto exprType = m_env.getLastExpressionType();
  assert(exprType != std::nullopt && constantType != std::nullopt &&
         "Visiting expression should set Environment::lastExpressionType");

  expect(isAssignable(*exprType, *constantType), ErrorType::ASSIGNMENT_TYPE_MISMATCH,
         constant.position);

  m_env.insertVar(constant.name, Variable{constant.name, *constantType, true});
}

void SemanticAnalyzer::visit(::StructDef& structure) {
  expect(!m_env.nameConflict(structure.name), ErrorType::REDEFINITION, structure.position);

  Struct structType;
  for (auto& [name, member] : structure.members) {
    member.accept(*this);
    auto memberType = m_env.getType(member.type);
    assert(memberType != std::nullopt && "Visiting member should assert that its type is defined");
    structType.members.emplace(name, *memberType);
  }

  m_env.insertType(structure.name, Type(std::move(structType)));
}

void SemanticAnalyzer::visit(::StructMember& member) {
  expect(m_env.containsType(member.type), ErrorType::UNDEFINED_TYPE, member.position);
}

void SemanticAnalyzer::visit(::VariantDef& variant) {
  expect(!m_env.nameConflict(variant.name), ErrorType::REDEFINITION, variant.position);

  Variant variantType;
  for (auto& type : variant.types) {
    expect(m_env.containsType(type), ErrorType::UNDEFINED_TYPE, variant.position);
    variantType.types.push_back(*m_env.getType(type));
  }

  m_env.insertType(variant.name, Type(std::move(variantType)));
}

void SemanticAnalyzer::visit(::FnDef& def) {
  expect(!m_env.nameConflict(def.name), ErrorType::REDEFINITION, def.position);
  expect(m_env.containsType(def.returnType), ErrorType::UNDEFINED_TYPE, def.position);

  m_fnHasReturnStmt = false;

  // Build function params
  std::vector<FnSignature::Param> params;
  for (auto& param : def.parameters) {
    param.accept(*this);
    auto paramType = m_env.getType(param.type);
    assert(paramType != std::nullopt &&
           "Visiting parameter should assert that its type is defined");
    params.emplace_back(*paramType, param.isConst);
  }

  // Insert function type into environment
  FnSignature fn{params, *m_env.getType(def.returnType)};
  m_env.insertFnSignature(def.name, std::move(fn));
  assert(m_env.getType(def.returnType) != std::nullopt);

  // Push stackframe and declare parameters
  m_env.pushStackFrame(def.name);
  for (auto& param : def.parameters) {
    auto paramType = m_env.getType(param.type);
    assert(paramType != std::nullopt && "Parameter type should be defined");
    m_env.insertVar(param.name, Variable(param.name, *paramType, param.isConst));
  }

  // Visit function body
  def.body.accept(*this);

  // Pop stackframe
  m_env.popStackFrame();

  // Check for return statement and reset the flag
  expect(def.returnType == Void::typeId || m_fnHasReturnStmt, ErrorType::MISSING_RETURN_STMT,
         def.position);
  m_fnHasReturnStmt = false;
}

void SemanticAnalyzer::visit(::FnParam& param) {
  expect(m_env.containsType(param.type), ErrorType::UNDEFINED_TYPE, param.position);
}

void SemanticAnalyzer::visit(::BinaryExpression& expr) {
  expr.lhs->accept(*this);
  auto lhsType = m_env.getLastExpressionType();
  assert(lhsType != std::nullopt &&
         "Visiting expression should set Environment::lastExpressionType");

  expr.rhs->accept(*this);
  auto rhsType = m_env.getLastExpressionType();
  assert(rhsType != std::nullopt &&
         "Visiting expression should set Environment::lastExpressionType");

  // Typechecking of binary expressions requires asserting that both operands have the same type,
  // operator is supported for that type and determining the type of result expression
  auto isValid = m_operatorsHandler.operandsMatch(*lhsType, *rhsType) &&
                 m_operatorsHandler.operatorIsSupported(*rhsType, expr.op);

  expect(isValid, ErrorType::EXPRESSION_TYPE_MISMATCH, expr.position);

  auto typeId = m_operatorsHandler.resultTypeId(*lhsType, expr.op);
  auto typePtr = m_env.getType(typeId);
  assert(typePtr != std::nullopt && "Type should be defined");

  m_env.setLastExpressionType(*typePtr);
}

void SemanticAnalyzer::visit(::UnaryExpression& expr) {
  expr.expr->accept(*this);
  auto exprType = m_env.getLastExpressionType();
  assert(exprType != std::nullopt &&
         "Visiting expression should set Environment::lastExpressionType");

  // Typechecking of unary expression requires asserting that operator is supported for the type and
  // determining the type of result expression
  auto isValid = m_operatorsHandler.operatorIsSupported(*exprType, expr.op);

  expect(isValid, ErrorType::EXPRESSION_TYPE_MISMATCH, expr.position);

  auto typeId = m_operatorsHandler.resultTypeId(*exprType, expr.op);
  auto typePtr = m_env.getType(typeId);
  assert(typePtr != std::nullopt && "Type should be defined");

  m_env.setLastExpressionType(*typePtr);
}

void SemanticAnalyzer::visit(::FunctionalExpression& expr) {
  expr.expr->accept(*this);     // Visiting expression should set Environment::lastExpressionType
  expr.postfix->accept(*this);  // Which will be consumed and checked by postfix
}

void SemanticAnalyzer::visit(::MemberAccessPostfix& accessedMember) {
  auto exprType = m_env.getLastExpressionType();
  assert(exprType != std::nullopt &&
         "Visiting functional expression base should set Environment::lastExpressionType");

  // Check if expression is a struct in the first place if we try to access a member
  expect((*exprType)->typeId() == Struct::typeId, ErrorType::INVALID_MEMBER_ACCESS,
         accessedMember.position);

  // Unpack the struct type from variant
  auto structType = std::get<Struct>((*exprType)->type);

  // Assert member we're trying to access exists
  expect(structType.members.contains(accessedMember.member), ErrorType::INVALID_MEMBER_ACCESS,
         accessedMember.position);

  // Set last expression type to the type of the member we're accessing
  m_env.setLastExpressionType(structType.members.at(accessedMember.member));
}

void SemanticAnalyzer::visit(::VariantAccessPostfix& accessedVariant) {
  auto exprType = m_env.getLastExpressionType();
  assert(exprType != std::nullopt &&
         "Visiting functional expression base should set Environment::lastExpressionType");

  // Check if expression is a variant in the first place if we try to unpack it
  expect((*exprType)->typeId() == Variant::typeId, ErrorType::INVALID_VARIANT_ACCESS,
         accessedVariant.position);

  // Check if variant type we're trying to unpack exists at all
  auto accessedType = m_env.getType(accessedVariant.variant);
  expect(accessedType != std::nullopt, ErrorType::INVALID_VARIANT_ACCESS, accessedVariant.position);

  // Check if variant type we're trying to unpack can be held by the variant
  expect(isAssignable(*accessedType, *exprType), ErrorType::INVALID_VARIANT_ACCESS,
         accessedVariant.position);

  // Set last expression type to the type we unpacked
  m_env.setLastExpressionType(*accessedType);
}

void SemanticAnalyzer::visit(::FnCallPostfix& fnCall) {
  auto exprType = m_env.getLastExpressionType();
  assert(exprType != std::nullopt &&
         "Visiting functional expression base should set Environment::lastExpressionType");

  // Check if expression is a function in the first place if we try to call it
  expect((*exprType)->typeId() == FnSignature::typeId, ErrorType::INVALID_FN_CALL, fnCall.position);
  auto fnCallSignature = std::get<FnSignature>((*exprType)->type);

  // Check if argument count matches
  expect(fnCall.args.size() == fnCallSignature.params.size(), ErrorType::INVALID_FN_CALL,
         fnCall.position);

  // Check if each argument type can be bound to the corresponding parameter type
  for (size_t i = 0; i < fnCall.args.size(); ++i) {
    fnCall.args[i]->accept(*this);
    auto argType = m_env.getLastExpressionType();
    assert(argType.has_value() && "Visiting argument should set Environment::lastExpressionType");
    expect(isAssignable(*argType, fnCallSignature.params[i].type), ErrorType::INVALID_FN_CALL,
           fnCall.position);
  }

  // Set last expression type to the return type of the function
  m_env.setLastExpressionType(fnCallSignature.returnType);
}

void SemanticAnalyzer::visit(::IdentifierExpr& expr) {
  // is it a variable?
  if (auto var = m_env.getVar(expr.name); var != std::nullopt) {
    assert((*var)->type != nullptr && "Type should not be null");
    m_env.setLastExpressionType((*var)->type);
  }
  // is it a type?
  else if (auto typePtr = m_env.getType(expr.name); typePtr != std::nullopt) {
    m_env.setLastExpressionType(*typePtr);
  }
  // is it a function?
  else if (auto fnSignature = m_env.getFnSignature(expr.name); fnSignature != std::nullopt) {
    m_env.setLastExpressionType(*fnSignature);
  }
  // if its none of the above, its a reference to undefined identifier
  else {
    m_error(ErrorType::UNDEFINED_IDENTIFIER, expr.position);
  }
}

void SemanticAnalyzer::visit(::Literal<int>&) {
  auto typePtr = m_env.getType(Int::typeId);
  assert(typePtr != std::nullopt && "Int type should be defined");
  m_env.setLastExpressionType(*typePtr);
}

void SemanticAnalyzer::visit(::Literal<float>&) {
  auto typePtr = m_env.getType(Float::typeId);
  assert(typePtr != std::nullopt && "Float type should be defined");
  m_env.setLastExpressionType(*typePtr);
}

void SemanticAnalyzer::visit(::Literal<bool>&) {
  auto typePtr = m_env.getType(Bool::typeId);
  assert(typePtr != std::nullopt && "Bool type should be defined");
  m_env.setLastExpressionType(*typePtr);
}

void SemanticAnalyzer::visit(::Literal<wchar_t>&) {
  auto typePtr = m_env.getType(Char::typeId);
  assert(typePtr != std::nullopt && "Char type should be defined");
  m_env.setLastExpressionType(*typePtr);
}

void SemanticAnalyzer::visit(::Literal<std::wstring>&) {
  auto typePtr = m_env.getType(String::typeId);
  assert(typePtr != std::nullopt && "String type should be defined");
  m_env.setLastExpressionType(*typePtr);
}

void SemanticAnalyzer::visit(::Object& object) {
  // Build an anonymous type for the object literal
  Struct objectType;
  for (auto& [name, value] : object.members) {
    value.accept(*this);

    auto valueType = m_env.getLastExpressionType();
    assert(valueType != std::nullopt &&
           "Visiting expression should set Environment::lastExpressionType");

    objectType.members.emplace(name, *valueType);
  }

  auto anonymousTypePtr = std::make_shared<Type>(Type(std::move(objectType)));
  m_env.setLastExpressionType(anonymousTypePtr);
}

void SemanticAnalyzer::visit(::ObjectMember& objectMember) {
  objectMember.value->accept(*this);

  auto valueType = m_env.getLastExpressionType();
  assert(valueType != std::nullopt &&
         "Visiting expression should set Environment::lastExpressionType");

  m_env.setLastExpressionType(*valueType);
}

void SemanticAnalyzer::visit(::ParenExpr& expr) {
  expr.expr->accept(*this);
  auto exprType = m_env.getLastExpressionType();
  assert(exprType != std::nullopt &&
         "Visiting expression should set Environment::lastExpressionType");
  m_env.setLastExpressionType(*exprType);
}

void SemanticAnalyzer::visit(::CastExpr& expr) {
  expr.expr->accept(*this);
  auto exprType = m_env.getLastExpressionType();
  assert(exprType != std::nullopt &&
         "Visiting expression should set Environment::lastExpressionType");

  // For cast expression to be valid, we need to check if expression can be casted to the
  // specified type
  auto typeId = m_castHandler.primitiveTypeToTypeId(expr.type);
  bool isValid = m_castHandler.isCastable(*exprType, typeId);
  expect(isValid, ErrorType::INVALID_CAST, expr.position);

  auto typePtr = m_env.getType(typeId);
  assert(typePtr != std::nullopt && "Type should be defined");

  m_env.setLastExpressionType(*typePtr);
}

void SemanticAnalyzer::visit(::BlockStmt& block) {
  m_env.enterScope();
  for (auto& statement : block.statements) {
    statement->accept(*this);
  }
  m_env.exitScope();
}

void SemanticAnalyzer::visit(::ExpressionStmt& exprStmt) {
  exprStmt.expr->accept(*this);
  // In this case we dont care about the type of the expression, it just has to be valid
}

void SemanticAnalyzer::visit(::AssignmentStmt& assignment) {
  assignment.lhs->accept(*this);
  auto lhsType = m_env.getLastExpressionType();
  assert(lhsType != std::nullopt &&
         "Visiting expression should set Environment::lastExpressionType");

  assignment.rhs->accept(*this);
  auto rhsType = m_env.getLastExpressionType();
  assert(rhsType != std::nullopt &&
         "Visiting expression should set Environment::lastExpressionType");

  expect(isAssignable(*rhsType, *lhsType), ErrorType::ASSIGNMENT_TYPE_MISMATCH,
         assignment.position);
  expect(exprIsAssignable(assignment.lhs), ErrorType::INVALID_ASSIGNMENT, assignment.position);
}

void SemanticAnalyzer::visit(::StdinExtractionStmt& inputStmt) {
  for (auto& expr : inputStmt.expressions) {
    expr->accept(*this);
    auto exprType = m_env.getLastExpressionType();
    assert(exprType != std::nullopt &&
           "Visiting expression should set Environment::lastExpressionType");

    expect(exprIsAssignable(expr), ErrorType::INVALID_ASSIGNMENT, expr->position);
  }
}

void SemanticAnalyzer::visit(::StdoutInsertionStmt& printStmt) {
  for (auto& expr : printStmt.expressions) {
    expr->accept(*this);
    auto exprType = m_env.getLastExpressionType();
    assert(exprType != std::nullopt &&
           "Visiting expression should set Environment::lastExpressionType");
  }
}

void SemanticAnalyzer::visit(::VariantMatchStmt& matchStmt) {
  matchStmt.expr->accept(*this);
  auto matchedExpr = m_env.getLastExpressionType();
  assert(matchedExpr != std::nullopt &&
         "Visiting expression should set Environment::lastExpressionType");

  expect((*matchedExpr)->typeId() == Variant::typeId, ErrorType::INVALID_VARIANT_MATCH_STMT,
         matchStmt.position);

  auto variantType = std::get<Variant>((*matchedExpr)->type);
  for (auto& entry : matchStmt.cases) {
    // Leave the variant we're matching for cases to check
    m_env.setLastExpressionType(*matchedExpr);
    entry.second.accept(*this);  // visit matchCase
  }
}

void SemanticAnalyzer::visit(::VariantMatchCase& matchCase) {
  auto variantTypePtr = m_env.getLastExpressionType();
  assert(variantTypePtr != std::nullopt && (*variantTypePtr)->typeId() == Variant::typeId &&
         "Visiting variant match stmt should leave information about variant we re matching");

  auto caseTypePtr = m_env.getType(matchCase.variant);

  expect(caseTypePtr != std::nullopt, ErrorType::UNDEFINED_TYPE, matchCase.position);
  expect(isAssignable(*caseTypePtr, *variantTypePtr), ErrorType::INVALID_VARIANT_MATCH_STMT,
         matchCase.position);

  matchCase.block.accept(*this);
}

void SemanticAnalyzer::visit(::IfStmt& ifStmt) {
  ifStmt.condition->accept(*this);
  auto conditionTypePtr = m_env.getLastExpressionType();
  assert(conditionTypePtr != std::nullopt &&
         "Visiting expression should set Environment::lastExpressionType");

  expect((*conditionTypePtr)->typeId() == Bool::typeId, ErrorType::EXPECTED_BOOL_EXPR,
         ifStmt.position);

  ifStmt.block.accept(*this);

  for (auto& elif : ifStmt.elifs) {
    elif.accept(*this);
  }

  if (ifStmt.elseClause != nullptr) {
    ifStmt.elseClause->accept(*this);
  }
}

void SemanticAnalyzer::visit(::Elif& elifClause) {
  elifClause.condition->accept(*this);
  auto conditionTypePtr = m_env.getLastExpressionType();
  assert(conditionTypePtr != std::nullopt &&
         "Visiting expression should set Environment::lastExpressionType");

  expect((*conditionTypePtr)->typeId() == Bool::typeId, ErrorType::EXPECTED_BOOL_EXPR,
         elifClause.position);

  elifClause.block.accept(*this);
}

void SemanticAnalyzer::visit(::Else& elseClause) { elseClause.block.accept(*this); }

void SemanticAnalyzer::visit(::ForStmt& stmt) {
  stmt.range.accept(*this);
  m_env.loopDepth()++;
  stmt.block.accept(*this);
  m_env.loopDepth()--;
}

void SemanticAnalyzer::visit(::Range& stmt) {
  stmt.start->accept(*this);
  auto startTypePtr = m_env.getLastExpressionType();
  assert(startTypePtr != std::nullopt &&
         "Visiting expression should set Environment::lastExpressionType");

  stmt.end->accept(*this);
  auto endTypePtr = m_env.getLastExpressionType();
  assert(endTypePtr != std::nullopt &&
         "Visiting expression should set Environment::lastExpressionType");

  expect((*startTypePtr)->typeId() == Int::typeId, ErrorType::INVALID_RANGE_EXPR, stmt.position);
  expect((*endTypePtr)->typeId() == Int::typeId, ErrorType::INVALID_RANGE_EXPR, stmt.position);
}

void SemanticAnalyzer::visit(::WhileStmt& whileStmt) {
  whileStmt.condition->accept(*this);
  auto conditionTypePtr = m_env.getLastExpressionType();
  assert(conditionTypePtr != std::nullopt &&
         "Visiting expression should set Environment::lastExpressionType");

  expect((*conditionTypePtr)->typeId() == Bool::typeId, ErrorType::EXPECTED_BOOL_EXPR,
         whileStmt.position);

  m_env.loopDepth()++;
  whileStmt.block.accept(*this);
  m_env.loopDepth()--;
}

void SemanticAnalyzer::visit(::ContinueStmt& stmt) {
  expect(m_env.loopDepth() > 0, ErrorType::CONTINUE_OUTSIDE_LOOP, stmt.position);
}

void SemanticAnalyzer::visit(::BreakStmt& stmt) {
  expect(m_env.loopDepth() > 0, ErrorType::BREAK_OUTSIDE_LOOP, stmt.position);
}

void SemanticAnalyzer::visit(::ReturnStmt& stmt) {
  m_fnHasReturnStmt = true;
  auto returnTypePtr = m_env.getCurrentFnReturnType();

  // Since statements are not allowed in global scope, we should be inside a function
  // If we cant retrieve it, it means logic failed somewhere and has to be fixed
  assert(returnTypePtr != std::nullopt && "Expected fnReturnType to be set");

  if (stmt.expr == nullptr) {
    expect((*returnTypePtr)->typeId() == Void::typeId, ErrorType::RETURN_TYPE_MISMATCH,
           stmt.position);
    return;
  }

  stmt.expr->accept(*this);
  auto exprTypePtr = m_env.getLastExpressionType();
  assert(exprTypePtr != std::nullopt &&
         "Visiting expression should set Environment::lastExpressionType");

  expect(isAssignable(*exprTypePtr, *returnTypePtr), ErrorType::RETURN_TYPE_MISMATCH,
         stmt.position);
}

}  // namespace Interpreter
