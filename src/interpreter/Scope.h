#pragma once

#include <map>
#include <optional>
#include <set>

#include "Type.h"
#include "Value.h"
#include "Variable.h"

namespace Interpreter {

/**
 * @brief Scope owns variables and types accessible within it.
 *
 * @note 1) Scope performs only name conflict checks.
 * @note 2) Types should not be changed once inserted.
 * @note 3) Variables are mutable, user has to ensure their integrity.
 */
class Scope {
 public:
  using VariableTable = std::map<Identifier, VariablePtr>;
  using TypeTable = std::map<TypeIdentifier, TypePtr>;

  Scope() = default;
  ~Scope() = default;
  Scope(Scope&&) = default;
  Scope& operator=(Scope&&) = default;

  Scope(const Scope&) = delete;
  Scope& operator=(const Scope&) = delete;

  bool nameConflict(const Identifier& name) const noexcept;

  bool containsVar(const Identifier& name) const noexcept;
  std::pair<VariableTable::iterator, bool> insertVar(const Identifier& name,
                                                     Variable&& variable) noexcept;
  std::optional<VariablePtr> getVar(const Identifier& name) noexcept;

  bool containsType(const TypeIdentifier& name) const noexcept;
  std::pair<TypeTable::iterator, bool> insertType(const TypeIdentifier& name, Type&& type) noexcept;
  std::optional<TypePtr> getType(const TypeIdentifier& name) const noexcept;

 private:
  VariableTable m_variables;
  TypeTable m_types;
};

}  // namespace Interpreter
