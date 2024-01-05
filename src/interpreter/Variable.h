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
  Variable(const Identifier& name, const TypeRef& type, const std::set<Modifier>& modifiers = {},
           const Value& value = Value{std::monostate{}}) noexcept
      : name{name}, type{type}, modifiers{modifiers}, value{value} {
    // Todo: prevent mismatch between type and value
  }

  bool isConst() const noexcept { return modifiers.contains(Modifier::CONST); }
  bool isDefined() const noexcept { return std::holds_alternative<std::monostate>(value.value); }

  const Identifier name;
  const TypeRef type;
  const std::set<Modifier> modifiers;
  Value value;
};

}  // namespace Interpreter
