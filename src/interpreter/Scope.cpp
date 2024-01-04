#include "Scope.h"

namespace Interpreter {

bool Scope::nameConflict(const Identifier& name) const noexcept {
  return m_variables.contains(name) || m_types.contains(name);
}

bool Scope::varIsDeclared(const Identifier& name) const noexcept {
  return m_variables.contains(name);
}

bool Scope::varIsDefined(const Identifier& name) const noexcept {
  return m_variables.contains(name) && m_variables.at(name).value != std::nullopt;
}

std::pair<Scope::VariableTable::iterator, bool> Scope::declareVar(
    const Identifier& name, const TypeIdentifier& type,
    const std::set<Modifier>& modifiers) noexcept {
  if (varIsDeclared(name) || !typeIsDeclared(type)) return {m_variables.end(), false};
  Variable var(name, type, std::nullopt, modifiers);
  return m_variables.emplace(var.name, std::move(var));
}

std::pair<Scope::VariableTable::iterator, bool> Scope::defineVar(
    const Identifier& name, const TypeIdentifier& type, const Value& value,
    const std::set<Modifier>& modifiers) noexcept {
  // type must be defined
  // Value must be assignable to type
  // variable must not be defined
  // if variable is declared, then type must match
  Variable var(name, type, value, modifiers);
  return m_variables.emplace(var.name, std::move(var));
}

bool Scope::assignVar(const Identifier& name, const Value& value) noexcept {
  // variable must be defined
  // variable cannot be const
  // value must be assignable to variable type
  if (varIsDeclared(name) && !m_variables.at(name).isConst()) return false;
  m_variables.at(name).value = value;
  return true;
}

std::optional<std::reference_wrapper<Variable>> Scope::getVar(const Identifier& name) noexcept {
  if (!m_variables.contains(name)) return std::nullopt;
  return std::ref(m_variables.at(name));
}

/* -------------------------------------------------------------------------- */

bool Scope::typeIsDeclared(const TypeIdentifier& name) const noexcept {
  return m_types.contains(name);
}

bool Scope::typeIsDefined(const TypeIdentifier& name) const noexcept {
  return m_types.contains(name) && m_types.at(name) != std::nullopt;
}

std::pair<Scope::TypeTable::iterator, bool> Scope::declareType(
    const TypeIdentifier& name) noexcept {
  return m_types.emplace(name, std::nullopt);
}

std::pair<Scope::TypeTable::iterator, bool> Scope::defineType(const TypeIdentifier& name,
                                                              Type&& type) noexcept {
  if (typeIsDefined(name)) return {m_types.end(), false};
  if (typeIsDeclared(name)) {
    m_types.at(name).emplace(std::move(type));
    return {m_types.find(name), true};
  }
  return m_types.emplace(name, std::move(type));
}

std::optional<TypeRef> Scope::getType(const TypeIdentifier& name) const noexcept {
  if (!typeIsDefined(name)) return std::nullopt;
  return std::ref(m_types.at(name).value());
}

}  // namespace Interpreter
