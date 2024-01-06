#include <cassert>

#include "Environment.h"
#include "FunctionBody.h"
#include "Statement.h"
#include "interpreter_utils.h"

namespace Interpreter {

Environment::Environment() { initGlobalScope(); }

bool Environment::nameConflict(const Identifier& name) const noexcept {
  if (m_stack.empty()) return m_globalScope.nameConflict(name) || m_fnSignatures.contains(name);
  return m_stack.top().nameConflict(name);
}

/* ---------------------------- Variable methods ---------------------------- */

bool Environment::containsVar(const Identifier& name) const noexcept {
  if (m_stack.empty()) return m_globalScope.containsVar(name);
  return m_stack.top().containsVar(name) || m_globalScope.containsVar(name);
}

std::pair<Scope::VariableTable::iterator, bool> Environment::insertVar(const Identifier& name,
                                                                       Variable&& var) {
  if (m_stack.empty()) {
    return m_globalScope.insertVar(name, std::move(var));
  }
  return m_stack.top().insertVar(name, std::move(var));
}

std::optional<std::reference_wrapper<Variable>> Environment::getVar(
    const Identifier& name) noexcept {
  if (!m_stack.empty() && m_stack.top().containsVar(name)) {
    return m_stack.top().getVar(name);
  }
  return m_globalScope.getVar(name);
}

/* ------------------------------ Type methods ------------------------------ */

bool Environment::containsType(const TypeIdentifier& name) const noexcept {
  if (m_stack.empty()) return m_globalScope.containsType(name);
  return m_stack.top().containsType(name) || m_globalScope.containsType(name);
}

std::pair<Scope::TypeTable::iterator, bool> Environment::insertType(const TypeIdentifier& name,
                                                                    Type&& type) {
  if (m_stack.empty()) {
    return m_globalScope.insertType(name, std::move(type));
  }
  return m_stack.top().insertType(name, std::move(type));
}

std::optional<TypeRef> Environment::getType(const TypeIdentifier& name) const noexcept {
  if (!m_stack.empty() && m_stack.top().containsType(name)) {
    return m_stack.top().getType(name);
  }
  return m_globalScope.getType(name);
}

/* ---------------------------- Function methods ---------------------------- */

bool Environment::containsFunction(const Identifier& name) const noexcept {
  return m_functions.contains(name);
}

std::pair<Environment::FunctionTable::iterator, bool> Environment::insertFunction(
    const Identifier& name, Function&& func) noexcept {
  if (nameConflict(name) || containsFunction(name)) return {m_functions.end(), false};
  return m_functions.emplace(name, std::move(func));
}

std::optional<std::reference_wrapper<const Function>> Environment::getFunction(
    const Identifier& fnName) const noexcept {
  auto it = m_functions.find(fnName);
  if (it != m_functions.end()) {
    return std::ref(it->second);
  }
  return std::nullopt;
}

/* --------------------------- FnSignature methods -------------------------- */

bool Environment::containsFnSignature(const Identifier& name) const noexcept {
  return m_fnSignatures.contains(name);
}

std::pair<Environment::FnSignatureTable::iterator, bool> Environment::insertFnSignature(
    const Identifier& name, FnSignature&& signature) noexcept {
  if (nameConflict(name) || containsFnSignature(name)) return {m_fnSignatures.end(), false};
  return m_fnSignatures.emplace(name, std::move(signature));
}

std::optional<std::reference_wrapper<const FnSignature>> Environment::getFnSignature(
    const Identifier& fnName) const noexcept {
  auto it = m_fnSignatures.find(fnName);
  if (it != m_fnSignatures.end()) {
    return std::ref(it->second);
  }
  return std::nullopt;
}

/* -------------------------- Flow control methods -------------------------- */

void Environment::pushStackFrame(const Identifier& fnName) {
  if (++m_fnCallDepth > RECURSION_LIMIT) {
    throw std::logic_error("Recursion limit exceeded!");
  }
  m_stack.emplace(fnName);
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

std::optional<Type> Environment::getCurrentFnReturnType() const {
  if (m_stack.empty()) {
    throw std::logic_error("Stack is empty!");
  }
  auto fnName = m_stack.top().getFnName();
  if (m_fnSignatures.contains(fnName)) {
    return m_fnSignatures.at(fnName).returnType.get();
  }
  return std::nullopt;
}

std::optional<Type> Environment::getLastExpressionType() {
  auto type = std::move(m_lastExpressionType);
  m_lastExpressionType.reset();
  return type;
}

void Environment::setLastExpressionType(const Type& type) { m_lastExpressionType.emplace(type); }

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
  initBuiltinTypes();
  initBuiltinFunctions();
}

void Environment::initBuiltinTypes() {
  m_globalScope.insertType(Void::typeId, Type(Void{}));
  m_globalScope.insertType(Int::typeId, Type(Int{}));
  m_globalScope.insertType(Float::typeId, Type(Float{}));
  m_globalScope.insertType(Char::typeId, Type(Char{}));
  m_globalScope.insertType(Bool::typeId, Type(Bool{}));
  m_globalScope.insertType(String::typeId, Type(String{}));
}

void Environment::initBuiltinFunctions() {
  // Proof of concept, for now only len function for string length calculation

  /* --------------------- Typechecking needs FnSignature --------------------- */

  // fn len(const str: string) -> int { ... }
  std::vector<FnSignature::Param> signatureParams = {
      {*getType(String::typeId), true}};  // string, isConst
  FnSignature lenFnSignature = {std::move(signatureParams),
                                *getType(Int::typeId)};       // params, returnType
  m_fnSignatures.emplace(LEN_FN, std::move(lenFnSignature));  // name, signature

  /* ------------------------ Execution needs Function ------------------------ */

  // 1) Build the function body
  BuiltinFunctionBody lenFnBody;
  lenFnBody.body = [](const std::vector<Value>& args) -> Value {
    assert(args.size() == 1 && std::holds_alternative<std::wstring>(args[0].value) &&
           "Invalid arguments bound to builtin 'len' function call!");
    auto str = std::get<std::wstring>(args[0].value);
    return Value(int(str.size()));
  };

  // 2) Build the function params
  std::vector<Function::Param> params = {{L"str", String::typeId, true}};  // name, type, isConst

  // 3) Build the function
  Function lenFn = {std::move(params), String::typeId,
                    std::make_shared<BuiltinFunctionBody>(std::move(lenFnBody))};

  // 4) Jump out of the window
  m_functions.emplace(LEN_FN, std::move(lenFn));
}

}  // namespace Interpreter
