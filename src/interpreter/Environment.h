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
  using FnSignatureTable = std::map<Identifier, TypePtr>;  // FnSignature
  using FunctionTable = std::map<Identifier, Function>;    // Function

  Environment();

  bool nameConflict(const Identifier& name) const noexcept;

  /* ---------------------------- Variable methods ---------------------------- */

  bool containsVar(const Identifier& name) const noexcept;
  std::pair<Scope::VariableTable::iterator, bool> insertVar(const Identifier& name, Variable&& var);
  std::optional<VariablePtr> getVar(const Identifier& name) noexcept;

  /* ------------------------------ Type methods ------------------------------ */

  bool containsType(const TypeIdentifier& name) const noexcept;
  std::pair<Scope::TypeTable::iterator, bool> insertType(const TypeIdentifier& name, Type&& type);
  std::optional<TypePtr> getType(const TypeIdentifier& name) const noexcept;

  /* ---------------------------- Function methods ---------------------------- */

  bool containsFunction(const Identifier& name) const noexcept;
  std::pair<FunctionTable::iterator, bool> insertFunction(const Identifier& name,
                                                          Function&& func) noexcept;
  std::optional<std::shared_ptr<Function>> getFunction(
      const Identifier& name) const noexcept;  // Todo ptr or ref?

  /* --------------------------- FnSignature methods -------------------------- */

  bool containsFnSignature(const Identifier& name) const noexcept;
  std::pair<FnSignatureTable::iterator, bool> insertFnSignature(const Identifier& name,
                                                                FnSignature&& signature) noexcept;
  std::optional<TypePtr> getFnSignature(const Identifier& name) const noexcept;

  /* -------------------------- Flow control methods -------------------------- */

  void pushStackFrame(const Identifier& fnName);
  void popStackFrame();
  void enterScope();
  void exitScope();

  // Interpretation

  std::optional<ValuePtr> getLastReturnValue();
  std::optional<ValuePtr> getLastExpressionValue();

  void setLastReturnValue(const ValuePtr& value);
  void setLastExpressionValue(const ValuePtr& value);

  FlowControlStatus& flowControlStatus();
  int& loopDepth();

  // Typechecking
  std::optional<TypePtr> getCurrentFnReturnType() const;
  std::optional<TypePtr> getLastExpressionType();
  void setLastExpressionType(const TypePtr& value);

 private:
  void initGlobalScope();
  void initBuiltinTypes();
  void initBuiltinFunctions();

  std::optional<ValuePtr> m_lastReturnValue;
  std::optional<ValuePtr> m_lastExpressionValue;

  std::optional<TypePtr> m_lastExpressionType;

  FlowControlStatus m_flowControlStatus = FlowControlStatus::NORMAL;
  int m_loopDepth = 0;
  int m_fnCallDepth = 0;

  std::stack<StackFrame> m_stack;  // Holds contexts for each function call
  Scope m_globalScope;             // Holds tables of global variables and types

  FnSignatureTable m_fnSignatures;
  FunctionTable m_functions;
};

}  // namespace Interpreter
