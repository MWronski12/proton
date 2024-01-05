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
  using FunctionTable = std::map<Identifier, std::pair<FnSignature, Function>>;

  Environment();

  bool nameConflict(const Identifier& name) const noexcept;

  /* ---------------------------- Variable methods ---------------------------- */

  bool varIsDeclared(const Identifier& name) const noexcept;
  bool varIsDefined(const Identifier& name) const noexcept;
  std::pair<Scope::VariableTable::iterator, bool> declareVar(const Identifier& name,
                                                             const TypeRef& type,
                                                             std::set<Modifier>&& modifiers = {});
  std::pair<Scope::VariableTable::iterator, bool> defineVar(const Identifier& name, Variable&& var);
  void assignVar(const Identifier& name, const Value& value) noexcept;
  std::optional<std::reference_wrapper<Variable>> getVar(const Identifier& name) noexcept;

  /* ------------------------------ Type methods ------------------------------ */

  bool typeIsDefined(const TypeIdentifier& name) const noexcept;
  std::pair<Scope::TypeTable::iterator, bool> defineType(const TypeIdentifier& name, Type&& type);
  std::optional<TypeRef> getType(const TypeIdentifier& name) const noexcept;

  /* ---------------------------- Function methods ---------------------------- */

  bool fnIsDefined(const Identifier& name) const noexcept;
  std::pair<FunctionTable::iterator, bool> defineFn(const Identifier& name,
                                                    Function&& func) noexcept;
  std::optional<Type> getFnSignature(const Identifier& name) const noexcept;
  std::optional<Value> getFunction(const Identifier& name) const noexcept;

  /* -------------------------- Flow control methods -------------------------- */

  void pushStackFrame(const Identifier& name);
  void popStackFrame();
  void enterScope();
  void exitScope();

  bool bindArguments(const std::vector<Value>& args);
  std::optional<TypeRef> getCurrentFnReturnType() const;

  std::optional<Value> getLastReturnValue();
  void setLastReturnValue(const Value& value);

  std::optional<Value> getLastExpressionValue();
  void setLastExpressionValue(const Value& value);

  std::optional<TypeRef> getLastExpressionType();
  void setLastExpressionType(const TypeRef& value);

  FlowControlStatus& flowControlStatus();
  int& loopDepth();

 private:
  void initGlobalScope();

  std::optional<Value> m_lastReturnValue;
  std::optional<Value> m_lastExpressionValue;
  std::optional<TypeRef> m_lastExpressionType;

  FlowControlStatus m_flowControlStatus = FlowControlStatus::NORMAL;
  int m_loopDepth = 0;
  int m_fnCallDepth = 0;

  std::stack<StackFrame> m_stack;  // Holds contexts for each function call
  Scope m_globalScope;             // Holds tables of global variables and types
  FunctionTable m_functions;       // Table of declared or defined functions
};

}  // namespace Interpreter
