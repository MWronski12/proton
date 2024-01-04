#pragma once

#include <map>
#include <optional>
#include <set>

#include "Type.h"
#include "Value.h"
#include "Variable.h"

namespace Interpreter {

/**
 * @brief Scope is a table of declared or defined types and variables.
 */
class Scope {
 public:
  using VariableTable = std::map<Identifier, Variable>;
  using TypeTable = std::map<TypeIdentifier, std::optional<const Type>>;

  Scope() = default;

  bool nameConflict(const Identifier& name) const noexcept;

  bool varIsDeclared(const Identifier& name) const noexcept;
  bool varIsDefined(const Identifier& name) const noexcept;
  std::pair<VariableTable::iterator, bool> declareVar(
      const Identifier& name, const TypeIdentifier& type,
      const std::set<Modifier>& modifiers = {}) noexcept;
  std::pair<VariableTable::iterator, bool> defineVar(
      const Identifier& name, const TypeIdentifier& type, const Value& value,
      const std::set<Modifier>& modifiers = {}) noexcept;
  bool assignVar(const Identifier& name, const Value& value) noexcept;
  std::optional<std::reference_wrapper<Variable>> getVar(const Identifier& name) noexcept;

  bool typeIsDeclared(const TypeIdentifier& name) const noexcept;
  bool typeIsDefined(const TypeIdentifier& name) const noexcept;
  std::pair<TypeTable::iterator, bool> declareType(const TypeIdentifier& name) noexcept;
  std::pair<TypeTable::iterator, bool> defineType(const TypeIdentifier& name, Type&& type) noexcept;
  std::optional<TypeRef> getType(const TypeIdentifier& name) const noexcept;

 private:
  // Variables are declared by creating Variable object with no value and adding it to the map.
  // Types are declared by adding <Identifier, nullopt> entry to the map.
  // Thats because variables are mutable (they can be assigned a new value), but types are not.

  VariableTable m_variables;
  TypeTable m_types;
};

}  // namespace Interpreter
