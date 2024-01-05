#include "Scope.h"

namespace Interpreter {

bool Scope::nameConflict(const Identifier& name) const noexcept {
  return m_variables.contains(name) || m_types.contains(name);
}

bool Scope::varIsDeclared(const Identifier& name) const noexcept {
  return m_variables.contains(name);
}

bool Scope::varIsDefined(const Identifier& name) const noexcept {
  return m_variables.contains(name) && m_variables.at(name).isDefined();
}

/**
 * @brief Only for declaring arguments when visiting FnDef body.
 */
std::pair<Scope::VariableTable::iterator, bool> Scope::declareVar(
    const Identifier& name, const TypeRef& type, const std::set<Modifier>& modifiers) noexcept {
  if (nameConflict(name)) return {m_variables.end(), false};
  return m_variables.emplace(name, Variable{name, type, modifiers});
}

std::pair<Scope::VariableTable::iterator, bool> Scope::defineVar(const Identifier& name,
                                                                 Variable&& var) noexcept {
  if (nameConflict(name)) return {m_variables.end(), false};
  if (varIsDeclared(name)) {
    return (assignVar(name, var.value) ? std::make_pair(m_variables.end(), false)
                                       : std::make_pair(m_variables.find(name), true));
  }
  return m_variables.emplace(name, std::move(var));
}

bool Scope::assignVar(const Identifier& name, const Value& value) noexcept {
  // Must be defined
  if (!varIsDefined(name)) return false;

  // Must be mutable and value type must match
  auto var = m_variables.at(name);
  if (var.isConst() || !valueTypeMatch(value, var.type)) return false;

  var.value = value;
  return true;
}

std::optional<std::reference_wrapper<Variable>> Scope::getVar(const Identifier& name) noexcept {
  if (!m_variables.contains(name)) return std::nullopt;
  return std::ref(m_variables.at(name));
}

/* -------------------------------------------------------------------------- */

bool Scope::typeIsDefined(const TypeIdentifier& name) const noexcept {
  return m_types.contains(name);
}

std::pair<Scope::TypeTable::iterator, bool> Scope::defineType(const TypeIdentifier& name,
                                                              Type&& type) noexcept {
  if (nameConflict(name)) return {m_types.end(), false};
  return m_types.emplace(name, std::move(type));
}

std::optional<TypeRef> Scope::getType(const TypeIdentifier& name) const noexcept {
  if (!typeIsDefined(name)) return std::nullopt;
  return std::ref(m_types.at(name));
}

}  // namespace Interpreter
