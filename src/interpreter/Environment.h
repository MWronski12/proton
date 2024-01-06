#pragma once

#include <optional>
#include <stack>

#include "StackFrame.h"
#include "Type.h"
#include "Value.h"
#include "Variable.h"
#include "config.h"

struct BlockStmt;

namespace Interpreter {

enum class FlowControlStatus { NORMAL, CONTINUE, BREAK, RETURN };

struct Environment {
 public:
  using FnSignatureTable = std::map<Identifier, const FnSignature>;
  using FunctionTable = std::map<Identifier, const Function>;

  Environment();

  bool nameConflict(const Identifier& name) const noexcept;

  /* ---------------------------- Variable methods ---------------------------- */

  bool containsVar(const Identifier& name) const noexcept;
  std::pair<Scope::VariableTable::iterator, bool> insertVar(const Identifier& name, Variable&& var);
  std::optional<std::reference_wrapper<Variable>> getVar(const Identifier& name) noexcept;

  /* ------------------------------ Type methods ------------------------------ */

  bool containsType(const TypeIdentifier& name) const noexcept;
  std::pair<Scope::TypeTable::iterator, bool> insertType(const TypeIdentifier& name, Type&& type);
  std::optional<TypeRef> getType(const TypeIdentifier& name) const noexcept;

  /* ---------------------------- Function methods ---------------------------- */

  bool containsFunction(const Identifier& name) const noexcept;
  std::pair<FunctionTable::iterator, bool> insertFunction(const Identifier& name,
                                                          Function&& func) noexcept;
  std::optional<std::reference_wrapper<const Function>> getFunction(
      const Identifier& name) const noexcept;

  /* --------------------------- FnSignature methods -------------------------- */

  bool containsFnSignature(const Identifier& name) const noexcept;
  std::pair<FnSignatureTable::iterator, bool> insertFnSignature(const Identifier& name,
                                                                FnSignature&& signature) noexcept;
  std::optional<std::reference_wrapper<const FnSignature>> getFnSignature(
      const Identifier& name) const noexcept;

  /* -------------------------- Flow control methods -------------------------- */

  void pushStackFrame(const Identifier& fnName);
  void popStackFrame();
  void enterScope();
  void exitScope();

  // Interpretation

  std::optional<Value> getLastReturnValue();
  std::optional<Value> getLastExpressionValue();

  void setLastReturnValue(const Value& value);
  void setLastExpressionValue(const Value& value);

  FlowControlStatus& flowControlStatus();
  int& loopDepth();

  // Typechecking
  std::optional<Type> getCurrentFnReturnType() const;
  std::optional<Type> getLastExpressionType();
  void setLastExpressionType(const Type& value);

 private:
  void initGlobalScope();
  void initBuiltinTypes();
  void initBuiltinFunctions();

  std::optional<Value> m_lastReturnValue;
  std::optional<Value> m_lastExpressionValue;

  std::optional<Type> m_lastExpressionType;
  std::optional<Type> m_currentFnReturnType;

  FlowControlStatus m_flowControlStatus = FlowControlStatus::NORMAL;
  int m_loopDepth = 0;
  int m_fnCallDepth = 0;

  std::stack<StackFrame> m_stack;  // Holds contexts for each function call
  Scope m_globalScope;             // Holds tables of global variables and types

  FnSignatureTable m_fnSignatures;
  FunctionTable m_functions;
};

}  // namespace Interpreter
