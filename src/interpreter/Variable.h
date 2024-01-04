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
           const std::set<Modifier>& modifiers = {}) noexcept
      : name{name}, type{type}, modifiers{modifiers}, value{value} {}

  bool isConst() const noexcept { return modifiers.contains(Modifier::CONST); }
  bool isDefined() const noexcept { return value != std::nullopt; }

  const Identifier name;
  const TypeIdentifier type;
  const std::set<Modifier> modifiers;
  std::optional<Value> value;
};

}  // namespace Interpreter
