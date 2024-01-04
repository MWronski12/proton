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
  using FunctionTableEntry = std::pair<FnSignature, std::optional<Function>>;
  using FunctionTable = std::map<Identifier, FunctionTableEntry>;

  Environment();

  bool nameConflict(const Identifier& name) const noexcept;

  /* ---------------------------- Variable methods ---------------------------- */

  bool varIsDeclared(const Identifier& name) const noexcept;
  bool varIsDefined(const Identifier& name) const noexcept;
  std::pair<Scope::VariableTable::iterator, bool> declareVar(const Identifier& name,
                                                             const TypeIdentifier& type,
                                                             std::set<Modifier>&& modifiers = {});
  std::pair<Scope::VariableTable::iterator, bool> defineVar(const Identifier& name,
                                                            const TypeIdentifier& type,
                                                            const Value& value,
                                                            std::set<Modifier>&& modifiers = {});
  void assignVar(const Identifier& name, const Value& value) noexcept;
  std::optional<std::reference_wrapper<Variable>> getVar(const Identifier& name) noexcept;

  /* ------------------------------ Type methods ------------------------------ */

  bool typeIsDeclared(const TypeIdentifier& name) const noexcept;
  bool typeIsDefined(const TypeIdentifier& name) const noexcept;
  std::pair<Scope::TypeTable::iterator, bool> declareType(const TypeIdentifier& name);
  std::pair<Scope::TypeTable::iterator, bool> defineType(const TypeIdentifier& name, Type&& type);
  std::optional<TypeRef> getType(const TypeIdentifier& name) const noexcept;

  /* ---------------------------- Function methods ---------------------------- */

  bool fnIsDeclared(const Identifier& name) const noexcept;
  bool fnIsDefined(const Identifier& name) const noexcept;
  std::pair<FunctionTable::iterator, bool> declareFn(
      const Identifier& name, const TypeRef& returnType,
      std::vector<FnSignature::Arg>&& args = {}) noexcept;
  std::pair<FunctionTable::iterator, bool> defineFn(
      const Identifier& name, const TypeRef& returnType, const Function::BodyRef& body,
      std::vector<Function::Param>&& params = {}) noexcept;
  std::optional<Type> getFnSignature(const Identifier& name) const noexcept;
  std::optional<Value> getFunction(const Identifier& name) const noexcept;

  /* -------------------------- Flow control methods -------------------------- */

  void pushStackFrame(const Identifier& name, std::vector<Value>&& args = {});
  void popStackFrame();
  void enterScope();
  void exitScope();

  std::optional<Value> getLastReturn();
  void setLastReturn(const Value& value);
  std::optional<Value> getLastResult();
  void setLastResult(const Value& value);

  FlowControlStatus& flowControlStatus();
  int& loopDepth();

 private:
  void initGlobalScope();

  std::optional<Value> m_lastReturn;
  std::optional<Value> m_lastResult;

  FlowControlStatus m_flowControlStatus = FlowControlStatus::NORMAL;
  int m_loopDepth = 0;
  int m_fnCallDepth = 0;

  std::stack<StackFrame> m_stack;  // Holds contexts for each function call
  Scope m_globalScope;             // Holds tables of global variables and types
  FunctionTable m_functions;       // Table of declared or defined functions
};

}  // namespace Interpreter
