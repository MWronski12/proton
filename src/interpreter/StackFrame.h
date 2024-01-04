#pragma once

#include <vector>

#include "Scope.h"

namespace Interpreter {

/**
 * @brief Data structure that allows for tracking current function context - function name,
 * parameters, stack of local scopes, their variables and types.
 */
class StackFrame {
 public:
  StackFrame(const Identifier& fnName, const Function& function);
  ~StackFrame() = default;

  StackFrame() = default;
  StackFrame(const StackFrame&) = delete;
  StackFrame(StackFrame&&) = delete;
  StackFrame& operator=(const StackFrame&) = delete;
  StackFrame& operator=(StackFrame&&) = delete;

  bool nameConflict(const Identifier& name) const noexcept;

  void enterScope() noexcept;
  void exitScope();

  bool varIsDeclared(const Identifier& name) const noexcept;
  bool varIsDefined(const Identifier& name) const noexcept;
  std::pair<Scope::VariableTable::iterator, bool> declareVar(const Identifier& name,
                                                             const TypeIdentifier& type,
                                                             std::set<Modifier>&& modifiers = {});
  std::pair<Scope::VariableTable::iterator, bool> defineVar(const Identifier& name,
                                                            const TypeIdentifier& type,
                                                            const Value& value,
                                                            std::set<Modifier>&& modifiers = {});
  bool assignVar(const Identifier& name, const Value& value) noexcept;
  std::optional<std::reference_wrapper<Variable>> getVar(const Identifier& name) noexcept;

  bool typeIsDeclared(const TypeIdentifier& name) const noexcept;
  bool typeIsDefined(const TypeIdentifier& name) const noexcept;
  std::pair<Scope::TypeTable::iterator, bool> declareType(const TypeIdentifier& name);
  std::pair<Scope::TypeTable::iterator, bool> defineType(const TypeIdentifier& name, Type&& type);
  std::optional<TypeRef> getType(const TypeIdentifier& name) const noexcept;

 private:
  const Identifier m_fnName;
  // const Function m_function;

  std::vector<Scope> m_scopes;
};

}  // namespace Interpreter
