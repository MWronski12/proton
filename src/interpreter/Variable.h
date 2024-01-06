#pragma once

#include <optional>
#include <set>

#include "Type.h"
#include "Value.h"
#include "interpreter_utils.h"

namespace Interpreter {

/**
 * @brief Data structure representing a variable - its name, type and optional value.
 */
struct Variable {
  Variable(const Identifier& name, const TypeRef& type, bool isConst = false,
           const std::optional<Value>& value = std::nullopt) noexcept
      : name{name},
        type{type},
        modifiers{isConst ? std::set<Modifier>{Modifier::CONST} : std::set<Modifier>{}},
        value{value} {}

  bool isConst() const noexcept { return modifiers.contains(Modifier::CONST); }
  bool isDefined() const noexcept { return value != std::nullopt; }

  const Identifier name;
  const TypeRef type;
  const std::set<Modifier> modifiers;
  std::optional<Value> value;
};

}  // namespace Interpreter
