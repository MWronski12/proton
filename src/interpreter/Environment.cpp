#include "Environment.h"
#include "Statement.h"

namespace Interpreter {

Environment::Environment() { initGlobalScope(); }

bool Environment::nameConflict(const Identifier& name) const noexcept {
  if (m_stack.empty()) return m_globalScope.nameConflict(name) || m_functions.contains(name);
  return m_stack.top().nameConflict(name);
}

/* ---------------------------- Variable methods ---------------------------- */

bool Environment::varIsDeclared(const Identifier& name) const noexcept {
  if (m_stack.empty()) return m_globalScope.varIsDeclared(name);
  return m_stack.top().varIsDeclared(name) || m_globalScope.varIsDeclared(name);
}

bool Environment::varIsDefined(const Identifier& name) const noexcept {
  if (m_stack.empty()) return m_globalScope.varIsDefined(name);
  return m_stack.top().varIsDefined(name) || m_globalScope.varIsDefined(name);
}

std::pair<Scope::VariableTable::iterator, bool> Environment::declareVar(
    const Identifier& name, const TypeIdentifier& type, std::set<Modifier>&& modifiers) {
  if (m_stack.empty()) {
    return m_globalScope.declareVar(name, type, std::move(modifiers));
  }
  return m_stack.top().declareVar(name, type, std::move(modifiers));
}

std::pair<Scope::VariableTable::iterator, bool> Environment::defineVar(
    const Identifier& name, const TypeIdentifier& type, const Value& value,
    std::set<Modifier>&& modifiers) {
  if (m_stack.empty()) {
    return m_globalScope.defineVar(name, type, value, std::move(modifiers));
  }
  return m_stack.top().defineVar(name, type, value, std::move(modifiers));
}

void Environment::assignVar(const Identifier& name, const Value& value) noexcept {
  if (m_stack.empty()) {
    m_globalScope.assignVar(name, value);
    return;
  }
  m_stack.top().assignVar(name, value);
}

std::optional<std::reference_wrapper<Variable>> Environment::getVar(
    const Identifier& name) noexcept {
  if (m_stack.empty()) {
    return m_globalScope.getVar(name);
  }
  return m_stack.top().getVar(name);
}

/* ------------------------------ Type methods ------------------------------ */

bool Environment::typeIsDeclared(const TypeIdentifier& name) const noexcept {
  if (m_stack.empty()) return m_globalScope.typeIsDeclared(name);
  return m_stack.top().typeIsDeclared(name) || m_globalScope.typeIsDeclared(name);
}

bool Environment::typeIsDefined(const TypeIdentifier& name) const noexcept {
  if (m_stack.empty()) return m_globalScope.typeIsDefined(name);
  return m_stack.top().typeIsDefined(name) || m_globalScope.typeIsDefined(name);
}

std::pair<Scope::TypeTable::iterator, bool> Environment::declareType(const TypeIdentifier& name) {
  if (m_stack.empty()) {
    return m_globalScope.declareType(name);
  }
  return m_stack.top().declareType(name);
}

std::pair<Scope::TypeTable::iterator, bool> Environment::defineType(const TypeIdentifier& name,
                                                                    Type&& type) {
  if (m_stack.empty()) {
    return m_globalScope.defineType(name, std::move(type));
  }
  return m_stack.top().defineType(name, std::move(type));
}

std::optional<TypeRef> Environment::getType(const TypeIdentifier& name) const noexcept {
  if (m_stack.empty()) {
    return m_globalScope.getType(name);
  }
  return m_stack.top().getType(name);
}

/* ---------------------------- Function methods ---------------------------- */

// bool Environment::fnIsDeclared(const Identifier& name) const { return m_functions.contains(name);
// }

// bool Environment::fnIsDefined(const Identifier& name) const {
//   return m_functions.contains(name) && m_functions.at(name).body != std::nullopt;
// }

// std::pair<Environment::FunctionTable::iterator, bool> Environment::declareFn(
//     const Identifier& name, const TypeIdentifier& returnType,
//     std::vector<Function::Param>&& params) {
//   if (fnIsDeclared(name)) throw std::logic_error("Function already declared!");
//   Function fn{std::move(params), returnType, std::nullopt};
//   return m_functions.emplace(name, std::move(fn));
// }

// void Environment::bindFunctionBody(const Identifier& name,
//                                    std::reference_wrapper<::BlockStmt> body) {
//   if (!fnIsDeclared(name)) throw std::logic_error("Function not declared!");
//   if (fnIsDefined(name)) throw std::logic_error("Function already defined!");
//   m_functions.at(name).body = body;
// }

// void Environment::bindFunctionArgs(const Identifier& name, std::vector<Value>&& args) {
//   if (!fnIsDeclared(name)) throw std::logic_error("Function not declared!");
//   if (!fnIsDefined(name)) throw std::logic_error("Function not defined!");
//   // TODO: check if arg binding conditions are met (new stack frame wiht exactly one empty scope
//   for
//   // args)
// }

/* -------------------------- Flow control methods -------------------------- */

// void Environment::pushStackFrame() {
//   if (++m_fnCallDepth > RECURSION_LIMIT) {
//     throw std::logic_error("Recursion limit exceeded!");
//   }
//   m_stack.emplace();
// }

void Environment::popStackFrame() {
  if (m_stack.empty()) {
    throw std::logic_error("Stack is empty!");
  }
  m_stack.pop();
  --m_fnCallDepth;
}

void Environment::enterScope() {}

void Environment::exitScope() {}

std::optional<Value> Environment::getLastReturn() {
  auto value = std::move(m_lastReturn);
  m_lastReturn.reset();
  return value;
}

void Environment::setLastReturn(const Value& value) { m_lastReturn = value; }

std::optional<Value> Environment::getLastResult() {
  auto value = std::move(m_lastResult);
  m_lastResult.reset();
  return value;
}

void Environment::setLastResult(const Value& value) { m_lastResult = value; }

FlowControlStatus& Environment::flowControlStatus() { return m_flowControlStatus; }

int& Environment::loopDepth() { return m_loopDepth; }

/* -------------------------------- Internal -------------------------------- */

void Environment::initGlobalScope() {
  m_globalScope.defineType(VOID, Type(Void{}));
  m_globalScope.defineType(INT, Type(Int{}));
  m_globalScope.defineType(FLOAT, Type(Float{}));
  m_globalScope.defineType(CHAR, Type(Char{}));
  m_globalScope.defineType(BOOL, Type(Bool{}));
  m_globalScope.defineType(STRING, Type(String{}));
}

}  // namespace Interpreter
