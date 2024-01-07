#include "Scope.h"

namespace Interpreter {

bool Scope::nameConflict(const Identifier& name) const noexcept {
  return m_variables.contains(name) || m_types.contains(name);
}

/* -------------------------------------------------------------------------- */

bool Scope::containsVar(const Identifier& name) const noexcept {
  return m_variables.contains(name);
}

std::pair<Scope::VariableTable::iterator, bool> Scope::insertVar(const Identifier& name,
                                                                 Variable&& var) noexcept {
  if (nameConflict(name)) return {m_variables.end(), false};
  return m_variables.emplace(name, std::move(var));
}

std::optional<std::reference_wrapper<Variable>> Scope::getVar(const Identifier& name) noexcept {
  if (!m_variables.contains(name)) return std::nullopt;
  return std::ref(m_variables.at(name));
}

/* -------------------------------------------------------------------------- */

bool Scope::containsType(const TypeIdentifier& name) const noexcept {
  return m_types.contains(name);
}

std::pair<Scope::TypeTable::iterator, bool> Scope::insertType(const TypeIdentifier& name,
                                                              Type&& type) noexcept {
  if (nameConflict(name)) return {m_types.end(), false};
  auto typePtr = std::make_shared<Type>(std::move(type));
  return m_types.emplace(name, std::move(typePtr));
}

std::optional<TypePtr> Scope::getType(const TypeIdentifier& name) const noexcept {
  if (!containsType(name)) return std::nullopt;
  return m_types.at(name);
}

}  // namespace Interpreter
