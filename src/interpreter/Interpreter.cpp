#include <cassert>

#include "Definition.h"
#include "ErrorHandler.h"
#include "Expression.h"
#include "FunctionBody.h"
#include "Interpreter.h"
#include "Program.h"
#include "Statement.h"
#include "Value.h"
#include "Variable.h"

namespace Interpreter {

/* ---------------------------------- Utils --------------------------------- */

/* ----------------------------- Helper methods ----------------------------- */

bool Interpreter::isVariant(const TypeIdentifier& type) const noexcept {
  return m_variantTypes.contains(type);
}

void Interpreter::expect(bool condition, ErrorType error, Position position) const {
  if (!condition) {
    m_error(error, position);
  }
}

/* ------------------------------ Constructors ------------------------------ */

Interpreter::Interpreter(::ErrorHandler& error) : m_error(error) {}

/* -------------------------------------------------------------------------- */
/*                             Visitation methods                             */
/* -------------------------------------------------------------------------- */

void Interpreter::visit(::Program& program) {
  // Functions are also values, so we need to initialize them
  for (auto& def : program.definitions) {
    def->accept(*this);
  }

  // Main function is the entry point of the program
  auto mainFn = m_env.getFunction(MAIN);
  assert(mainFn != std::nullopt && *mainFn != nullptr && "Main function should be defined");
  (*mainFn)->body->accept(*this);
}

void Interpreter::visit(::VarDef& varDef) {
  varDef.value->accept(*this);
  auto lastResult = m_env.getLastResult();
  assert(lastResult != std::nullopt &&
         "Visiting expression should set Environment::lastExpressionValue");

  auto [typeId, value] = std::move(*lastResult);

  if (isVariant(varDef.type)) {
    value = std::make_shared<Value>(VariantValue{typeId, std::move(value)});
  }

  m_env.insertVar(varDef.name, Variable{varDef.name, typeId, std::move(value)});
}

void Interpreter::visit(::ConstDef& constDef) {
  constDef.value->accept(*this);
  auto lastResult = m_env.getLastResult();
  assert(lastResult != std::nullopt &&
         "Visiting expression should set Environment::lastExpressionValue");

  auto [typeId, value] = std::move(*lastResult);

  if (isVariant(constDef.type)) {
    value = std::make_shared<Value>(VariantValue{typeId, std::move(value)});
  }

  m_env.insertVar(constDef.name, Variable{constDef.name, typeId, std::move(value)});
}

void Interpreter::visit(::FnDef& fnDef) {
  std::vector<Function::Param> params;
  for (auto& param : fnDef.parameters) {
    params.emplace_back(param.name, param.type, param.isConst);
  }

  UserFunction body{fnDef.body};
  Function fn{std::move(params), fnDef.returnType, std::make_shared<UserFunction>(body)};
  m_env.insertFunction(fnDef.name, std::move(fn));
}

// void Interpreter::visit(::BinaryExpression& expr) {
//   expr.lhs->accept(*this);
//   auto lhs = m_env.getLastExpressionValue();
//   assert(lhs != std::nullopt && "Visiting expression should set
//   Environment::lastExpressionValue");

//   expr.rhs->accept(*this);
//   auto rhs = m_env.getLastExpressionValue();
//   assert(rhs != std::nullopt && "Visiting expression should set
//   Environment::lastExpressionValue");

//   auto result = m_operatorsHandler.applyOperator(*lhs, *rhs, expr.op);
//   m_env.setLastExpressionValue(std::move(result));
// }

// void Interpreter::visit(::UnaryExpression& expr) {
//   expr.expr->accept(*this);
//   auto value = m_env.getLastExpressionValue();
//   assert(value != std::nullopt &&
//          "Visiting expression should set Environment::lastExpressionValue");

//   auto result = m_operatorsHandler.applyOperator(*value, expr.op);
//   m_env.setLastExpressionValue(std::move(result));
// }

// void Interpreter::visit(::FunctionalExpression& expr) {
//   expr.expr->accept(*this);     // Visiting expression should set Environment::lastExpressionType
//   expr.postfix->accept(*this);  // Which will be consumed and checked by postfix
// }

// void Interpreter::visit(::MemberAccessPostfix& accessedMember) {
//   auto value = m_env.getLastExpressionValue();
//   assert(value != std::nullopt &&
//          "Visiting expression should set Environment::lastExpressionValue");

//   assert(std::holds_alternative<ObjectValue>(value->value) && "Expected object value");
//   auto objectValue = std::get<ObjectValue>(value->value);

//   auto memberValue = objectValue.members.find(accessedMember.member);
//   assert(memberValue != objectValue.members.end() && "Object member should be defined");

//   m_env.setLastExpressionValue(std::move(memberValue->second));
// }

// void Interpreter::visit(::VariantAccessPostfix&) {
//   // Todo: I am not sure if any other action is required here
//   auto value = m_env.getLastExpressionValue();
//   assert(value != std::nullopt &&
//          "Visiting expression should set Environment::lastExpressionValue");
//   assert(std::holds_alternative<VariantValue>(value->value) && "Expected variant value");

//   auto variantValue = std::get<VariantValue>(value->value);
//   m_env.setLastExpressionValue(*variantValue.value);
// }

// void Interpreter::visit(::FnCallPostfix& fnCall) {
//   // Check function pointer left by FunctionalExpression
//   auto func = m_env.getLastExpressionValue();
//   assert(func != std::nullopt && "Visiting expression should set
//   Environment::lastExpressionValue");

//   assert(std::holds_alternative<Function>(func->value) && "Expected function value");
//   auto function = std::get<Function>(func->value);
//   assert(function.params.size() == fnCall.args.size() && "Expected correct number of arguments");

//   // Push stackframe
//   m_env.pushStackFrame(L"");

//   // Bind arguments
//   for (uint i = 0; i < fnCall.args.size(); ++i) {
//     fnCall.args[i]->accept(*this);
//     auto value = m_env.getLastExpressionValue();
//     assert(value != std::nullopt &&
//            "Visiting expression should set Environment::lastExpressionValue");

//     // Insert argument variable into stackframe
//     auto name = function.params[i].name;
//     bool isConst = function.params[i].modifiers.contains(Modifier::CONST);
//     auto argVariable = Variable(function.params[i].name, nullptr, isConst, std::move(*value));
//     m_env.insertVar(name, std::move(argVariable));
//   }

//   // Visit function body
//   function.body->accept(*this);

//   // Pop stackframe
//   m_env.popStackFrame();
// }

// void Interpreter::visit(BuiltinFunction& func) {
//   // Fetch bound arguments
//   std::vector<Value> args;
//   for (auto& name : func.argNames) {
//     auto var = m_env.getVar(name);
//     assert(var != std::nullopt && *var != nullptr && (*var)->value != std::nullopt &&
//            "Arg should be bound");
//     args.emplace_back((*var)->value.value());
//   }

//   // Call builtin function
//   func.body(args);
// }

void Interpreter::visit(::IdentifierExpr& expr) {
  auto var = m_env.getVar(expr.name);
  assert(var != std::nullopt && var "Variable should be defined");

  auto value = (*var)->value;
  assert(value != nullptr && "Variable should have a value");

  m_env.setLastExpressionValue(std::move(*value));
}

// void Interpreter::visit(::Literal<int>& expr) { m_env.setLastExpressionValue(Value(expr.value));
// }

// void Interpreter::visit(::Literal<float>& expr) {
// m_env.setLastExpressionValue(Value(expr.value)); }

// void Interpreter::visit(::Literal<bool>& expr) { m_env.setLastExpressionValue(Value(expr.value));
// }

// void Interpreter::visit(::Literal<wchar_t>& expr) {
//   m_env.setLastExpressionValue(Value(expr.value));
// }

// void Interpreter::visit(::Literal<std::wstring>& expr) {
//   m_env.setLastExpressionValue(Value(expr.value));
// }

// void Interpreter::visit(::Object& objectLiteral) {
//   ObjectValue objectValue;
//   for (auto& [name, member] : objectLiteral.members) {
//     member.accept(*this);
//     auto memberValue = m_env.getLastExpressionValue();
//     assert(memberValue != std::nullopt &&
//            "Visiting expression should set Environment::lastExpressionValue");

//     objectValue.members.emplace(name, std::move(*memberValue));
//   }
//   m_env.setLastExpressionValue(std::move(objectValue));
// }

// void Interpreter::visit(::ObjectMember& member) {
//   member.value->accept(*this);
//   auto value = m_env.getLastExpressionValue();
//   assert(value != std::nullopt &&
//          "Visiting expression should set Environment::lastExpressionValue");

//   m_env.setLastExpressionValue(std::move(*value));
// }

// void Interpreter::visit(::ParenExpr& parenExpr) {
//   parenExpr.expr->accept(*this);
//   auto value = m_env.getLastExpressionValue();
//   assert(value != std::nullopt &&
//          "Visiting expression should set Environment::lastExpressionValue");

//   m_env.setLastExpressionValue(std::move(*value));
// }

// void Interpreter::visit(::CastExpr& castExpr) {
//   castExpr.expr->accept(*this);
//   auto value = m_env.getLastExpressionValue().value();
//   // assert(value != std::nullopt &&
//   //        "Visiting expression should set Environment::lastExpressionValue");

//   m_castHandler.applyCast(value, castExpr.type);
//   // m_env.setLastExpressionValue(std::move(result));
// }

// // void Interpreter::visit(::BlockStmt&) {}
// // void Interpreter::visit(::ExpressionStmt&) {}
// // void Interpreter::visit(::AssignmentStmt&) {}
// // void Interpreter::visit(::StdinExtractionStmt&) {}
// // void Interpreter::visit(::StdoutInsertionStmt&) {}
// // void Interpreter::visit(::VariantMatchStmt&) {}
// // void Interpreter::visit(::VariantMatchCase&) {}
// // void Interpreter::visit(::IfStmt&) {}
// // void Interpreter::visit(::Else&) {}
// // void Interpreter::visit(::Elif&) {}
// // void Interpreter::visit(::ForStmt&) {}
// // void Interpreter::visit(::Range&) {}
// // void Interpreter::visit(::WhileStmt&) {}
// // void Interpreter::visit(::ContinueStmt&) {}
// // void Interpreter::visit(::BreakStmt&) {}
// // void Interpreter::visit(::ReturnStmt&) {}

}  // namespace Interpreter
