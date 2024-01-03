#pragma once

#include <optional>
#include <set>

#include "Type.h"
#include "Value.h"

namespace Interpreter {

/**
 * @brief Data structure representing a variable - its name, type and optional value.
 */
struct Variable {
  Variable(const Identifier& name, const TypeIdentifier& type,
           const std::optional<Value>& value = std::nullopt,
           std::set<Modifier>&& modifiers = {}) noexcept;

  bool isConst() const noexcept;
  bool isDefined() const noexcept;

  const Identifier name;
  const TypeIdentifier type;
  const std::set<Modifier> modifiers;
  std::optional<Value> value;
};

}  // namespace Interpreter
