#include "Environment.h"
#include "Statement.h"
#include "interpreter_utils.h"

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
    const Identifier& name, const TypeRef& type, std::set<Modifier>&& modifiers) {
  if (m_stack.empty()) {
    return m_globalScope.declareVar(name, type, std::move(modifiers));
  }
  return m_stack.top().declareVar(name, type, std::move(modifiers));
}

std::pair<Scope::VariableTable::iterator, bool> Environment::defineVar(const Identifier& name,
                                                                       Variable&& var) {
  if (m_stack.empty()) {
    return m_globalScope.defineVar(name, std::move(var));
  }
  return m_stack.top().defineVar(name, std::move(var));
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
  if (!m_stack.empty() && m_stack.top().varIsDeclared(name)) {
    return m_stack.top().getVar(name);
  }
  return m_globalScope.getVar(name);
}

/* ------------------------------ Type methods ------------------------------ */

bool Environment::typeIsDefined(const TypeIdentifier& name) const noexcept {
  if (m_stack.empty()) return m_globalScope.typeIsDefined(name);
  return m_stack.top().typeIsDefined(name) || m_globalScope.typeIsDefined(name);
}

std::pair<Scope::TypeTable::iterator, bool> Environment::defineType(const TypeIdentifier& name,
                                                                    Type&& type) {
  if (m_stack.empty()) {
    return m_globalScope.defineType(name, std::move(type));
  }
  return m_stack.top().defineType(name, std::move(type));
}

std::optional<TypeRef> Environment::getType(const TypeIdentifier& name) const noexcept {
  if (!m_stack.empty() && m_stack.top().typeIsDefined(name)) {
    return m_stack.top().getType(name);
  }
  return m_globalScope.getType(name);
}

/* ---------------------------- Function methods ---------------------------- */

bool Environment::fnIsDefined(const Identifier& name) const noexcept {
  return m_functions.contains(name);
}

std::pair<Environment::FunctionTable::iterator, bool> Environment::defineFn(
    const Identifier& name, Function&& func) noexcept {
  if (nameConflict(name) || fnIsDefined(name)) return {m_functions.end(), false};
  return m_functions.emplace(name, std::make_pair(fnToFnSignature(func), std::move(func)));
}

std::optional<Type> Environment::getFnSignature(const Identifier& fnName) const noexcept {
  if (m_functions.contains(fnName)) {
    auto fnSignature = m_functions.at(fnName).first;
    return Type(std::move(fnSignature));
  }
  return std::nullopt;
}

std::optional<Value> Environment::getFunction(const Identifier& fnName) const noexcept {
  if (fnIsDefined(fnName)) {
    auto fn = m_functions.at(fnName).second;
    return Value(fn);
  }
  return std::nullopt;
}

/* -------------------------- Flow control methods -------------------------- */

void Environment::pushStackFrame(const Identifier& name) {
  if (++m_fnCallDepth > RECURSION_LIMIT) {
    throw std::logic_error("Recursion limit exceeded!");
  }
  m_stack.emplace(name);
}

void Environment::popStackFrame() {
  if (m_stack.empty()) {
    throw std::logic_error("Stack is empty!");
  }
  m_stack.pop();
  --m_fnCallDepth;
}

void Environment::enterScope() {
  if (m_stack.empty()) {
    throw std::logic_error("Stack is empty!");
  }
  m_stack.top().enterScope();
}

void Environment::exitScope() {
  if (m_stack.empty()) {
    throw std::logic_error("Stack is empty!");
  }
  m_stack.top().exitScope();
}

bool Environment::bindArguments(const std::vector<Value>& args) {
  if (m_stack.empty()) {
    throw std::logic_error("Stack is empty!");
  }

  const auto& fnName = m_stack.top().getFnName();
  auto fn = m_functions.at(fnName).second;

  if (fn.params.size() != args.size()) return false;

  for (size_t i = 0; i < fn.params.size(); i++) {
    auto& param = fn.params[i];
    auto& arg = args[i];

    if (!valueTypeMatch(arg, param.type)) return false;

    m_stack.top().defineVar(param.name, Variable(param.name, param.type, param.modifiers, arg));
  }

  return true;
}

std::optional<TypeRef> Environment::getCurrentFnReturnType() const {
  if (m_stack.empty()) {
    throw std::logic_error("Stack is empty!");
  }
  auto fnName = m_stack.top().getFnName();
  if (m_functions.contains(fnName)) {
    return m_functions.at(fnName).second.returnType;
  }
  return std::nullopt;
}

std::optional<TypeRef> Environment::getLastExpressionType() {
  auto type = std::move(m_lastExpressionType);
  m_lastExpressionType.reset();
  return type;
}

void Environment::setLastExpressionType(const TypeRef& type) { m_lastExpressionType = type; }

std::optional<Value> Environment::getLastExpressionValue() {
  auto value = std::move(m_lastExpressionValue);
  m_lastExpressionValue.reset();
  return value;
}

void Environment::setLastExpressionValue(const Value& value) { m_lastExpressionValue = value; }

std::optional<Value> Environment::getLastReturnValue() {
  auto value = std::move(m_lastReturnValue);
  m_lastReturnValue.reset();
  return value;
}

void Environment::setLastReturnValue(const Value& value) { m_lastReturnValue = value; }

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
