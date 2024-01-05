#pragma once

#include <vector>

#include "Scope.h"

namespace Interpreter {

/**
 * @brief Data structure for separating fn call context scopes.
 *
 * @note Class guarantees that there is always at least one scope (intended for args if any).
 */
class StackFrame {
 public:
  StackFrame(const Identifier& fnName);
  ~StackFrame() = default;

  StackFrame() = delete;
  StackFrame(const StackFrame&) = delete;
  StackFrame(StackFrame&&) = delete;
  StackFrame& operator=(const StackFrame&) = delete;
  StackFrame& operator=(StackFrame&&) = delete;

  bool nameConflict(const Identifier& name) const noexcept;
  const Identifier& getFnName() const noexcept { return m_fnName; }

  void enterScope() noexcept;
  void exitScope();

  bool varIsDeclared(const Identifier& name) const noexcept;
  bool varIsDefined(const Identifier& name) const noexcept;
  std::pair<Scope::VariableTable::iterator, bool> declareVar(const Identifier& name,
                                                             const TypeRef& type,
                                                             std::set<Modifier>&& modifiers = {});
  std::pair<Scope::VariableTable::iterator, bool> defineVar(const Identifier& name, Variable&& var);
  bool assignVar(const Identifier& name, const Value& value) noexcept;
  std::optional<std::reference_wrapper<Variable>> getVar(const Identifier& name) noexcept;

  bool typeIsDefined(const TypeIdentifier& name) const noexcept;
  std::pair<Scope::TypeTable::iterator, bool> defineType(const TypeIdentifier& name, Type&& type);
  std::optional<TypeRef> getType(const TypeIdentifier& name) const noexcept;

 private:
  const Identifier m_fnName;

  std::vector<Scope> m_scopes;
};

}  // namespace Interpreter
