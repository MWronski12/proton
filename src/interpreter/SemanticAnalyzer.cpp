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

void SemanticAnalyzer::expect(bool condition, ErrorType error, Position position) {
  if (!condition) {
    m_error(error, position);
  }
}

SemanticAnalyzer::SemanticAnalyzer(Environment& environment, ::ErrorHandler& error)
    : m_env{environment}, m_error{error} {}

void SemanticAnalyzer::visit(::Program& program) {
  for (auto& definition : program.definitions) {
    definition->accept(*this);
  }
}

void SemanticAnalyzer::visit(::VarDef& var) {
  expect(!m_env.nameConflict(var.name), ErrorType::REDEFINITION, var.position);
  expect(!m_env.varIsDeclared(var.name), ErrorType::REDEFINITION, var.position);
  expect(m_env.typeIsDefined(var.type), ErrorType::UNDEFINED_TYPE, var.position);

  var.value->accept(*this);

  auto varType = m_env.getType(var.type);
  auto exprType = m_env.getLastExpressionType();

  if (exprType == std::nullopt || varType == std::nullopt) {
    throw std::logic_error("Visiting expression should set Environment::lastExpressionType");
  }

  expect(exprType.value() == varType.value(), ErrorType::TYPE_MISMATCH, var.position);

  m_env.declareVar(var.name, varType.value());
}

void SemanticAnalyzer::visit(::ConstDef& constant) {
  expect(!m_env.nameConflict(constant.name), ErrorType::REDEFINITION, constant.position);
  expect(m_env.typeIsDefined(constant.type), ErrorType::UNDEFINED_TYPE, constant.position);

  constant.value->accept(*this);

  auto varType = m_env.getType(constant.type);
  auto exprType = m_env.getLastExpressionType();

  if (exprType == std::nullopt || varType == std::nullopt) {
    throw std::logic_error("Visiting expression should set Environment::lastExpressionType");
  }

  expect(exprType.value() == varType.value(), ErrorType::TYPE_MISMATCH, constant.position);

  m_env.declareVar(constant.name, varType.value(), {Modifier::CONST});
}

void SemanticAnalyzer::visit(::StructDef& structure) {
  expect(!m_env.nameConflict(structure.name), ErrorType::REDEFINITION, structure.position);
  expect(!m_env.typeIsDefined(structure.name), ErrorType::REDEFINITION, structure.position);

  Struct structType;
  for (auto& [name, member] : structure.members) {
    member.accept(*this);
    structType.members.emplace(name, m_env.getType(member.type).value());
  }

  m_env.defineType(structure.name, Type(std::move(structType)));
}

void SemanticAnalyzer::visit(::StructMember& member) {
  expect(m_env.typeIsDefined(member.type), ErrorType::UNDEFINED_TYPE, member.position);
}

void SemanticAnalyzer::visit(::VariantDef& variant) {
  expect(!m_env.nameConflict(variant.name), ErrorType::REDEFINITION, variant.position);
  expect(!m_env.typeIsDefined(variant.name), ErrorType::REDEFINITION, variant.position);

  Variant variantType;
  for (auto& type : variant.types) {
    expect(m_env.typeIsDefined(type), ErrorType::UNDEFINED_TYPE, variant.position);
    variantType.types.emplace(*m_env.getType(type));
  }

  m_env.defineType(variant.name, Type(std::move(variantType)));
}

void SemanticAnalyzer::visit(::FnDef& def) {
  expect(!m_env.nameConflict(def.name), ErrorType::REDEFINITION, def.position);
  expect(m_env.typeIsDefined(def.returnType), ErrorType::UNDEFINED_TYPE, def.position);

  std::vector<Function::Param> params;
  for (auto& param : def.parameters) {
    param.accept(*this);
    params.emplace_back(param.name, m_env.getType(param.type).value(), param.isConst);
  }

  Function fn{std::move(params), m_env.getType(def.returnType).value(), def.body};
  m_env.defineFn(def.name, std::move(fn));

  m_env.pushStackFrame(def.name);
  for (auto& param : def.parameters) {
    std::set<Modifier> modifiers =
        param.isConst ? std::set<Modifier>{Modifier::CONST} : std::set<Modifier>();
    m_env.declareVar(param.name, m_env.getType(param.type).value());
  }

  def.body.accept(*this);
}

void SemanticAnalyzer::visit(::FnParam& param) {
  expect(m_env.typeIsDefined(param.type), ErrorType::UNDEFINED_TYPE, param.position);
}

// void SemanticAnalyzer::visit(::BinaryExpression& expr);
// void SemanticAnalyzer::visit(::UnaryExpression& expr);
// void SemanticAnalyzer::visit(::FunctionalExpression& expr);
// void SemanticAnalyzer::visit(::MemberAccessPostfix& postfix);
// void SemanticAnalyzer::visit(::VariantAccessPostfix& postfix);
// void SemanticAnalyzer::visit(::FnCallPostfix& postfix);
// void SemanticAnalyzer::visit(::IdentifierExpr& expr);

void SemanticAnalyzer::visit(::Literal<int>& expr [[maybe_unused]]) {
  auto type = m_env.getType(INT);
  if (type == std::nullopt) {
    throw std::logic_error("Int type should be defined");
  }
  m_env.setLastExpressionType(*type);
}

void SemanticAnalyzer::visit(::Literal<float>& expr [[maybe_unused]]) {
  auto type = m_env.getType(FLOAT);
  if (type == std::nullopt) {
    throw std::logic_error("Float type should be defined");
  }
  m_env.setLastExpressionType(*type);
}

void SemanticAnalyzer::visit(::Literal<bool>& expr [[maybe_unused]]) {
  auto type = m_env.getType(BOOL);
  if (type == std::nullopt) {
    throw std::logic_error("Bool type should be defined");
  }
  m_env.setLastExpressionType(*type);
}

void SemanticAnalyzer::visit(::Literal<wchar_t>& expr [[maybe_unused]]) {
  auto type = m_env.getType(CHAR);
  if (type == std::nullopt) {
    throw std::logic_error("Char type should be defined");
  }
  m_env.setLastExpressionType(*type);
}

void SemanticAnalyzer::visit(::Literal<std::wstring>& expr [[maybe_unused]]) {
  auto type = m_env.getType(STRING);
  if (type == std::nullopt) {
    throw std::logic_error("String type should be defined");
  }
  m_env.setLastExpressionType(*type);
}

// void SemanticAnalyzer::visit(::Object& expr);
// void SemanticAnalyzer::visit(::ObjectMember& expr);
// void SemanticAnalyzer::visit(::ParenExpr& expr);
// void SemanticAnalyzer::visit(::CastExpr& expr);

void SemanticAnalyzer::visit(::BlockStmt& stmt) {
  m_env.enterScope();
  std::cout << stmt.statements.size() << std::endl;
  m_env.exitScope();
}

// void SemanticAnalyzer::visit(::ExpressionStmt& stmt);
// void SemanticAnalyzer::visit(::AssignmentStmt& stmt);
// void SemanticAnalyzer::visit(::StdinExtractionStmt& stmt);
// void SemanticAnalyzer::visit(::StdoutInsertionStmt& stmt);
// void SemanticAnalyzer::visit(::VariantMatchStmt& stmt);
// void SemanticAnalyzer::visit(::VariantMatchCase& stmt);
// void SemanticAnalyzer::visit(::IfStmt& stmt);
// void SemanticAnalyzer::visit(::Else& stmt);
// void SemanticAnalyzer::visit(::Elif& stmt);
// void SemanticAnalyzer::visit(::ForStmt& stmt);
// void SemanticAnalyzer::visit(::Range& stmt);
// void SemanticAnalyzer::visit(::WhileStmt& stmt);
// void SemanticAnalyzer::visit(::ContinueStmt& stmt);
// void SemanticAnalyzer::visit(::BreakStmt& stmt);
// void SemanticAnalyzer::visit(::ReturnStmt& stmt);

}  // namespace Interpreter