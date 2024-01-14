#pragma once

#include <optional>
#include <set>

#include "Type.h"
#include "Value.h"
#include "interpreter_utils.h"

namespace Interpreter {

struct Variable;
using VariablePtr = std::shared_ptr<Variable>;

/**
 * @brief Data structure representing a variable - its name, type and optional value.
 */
struct Variable {
  // For semantic analysis
  Variable(const Identifier& name, const TypePtr& type, bool isConst = false) noexcept
      : name{name},
        type{type},
        modifiers(isConst ? std::vector<Modifier>{Modifier::CONST} : std::vector<Modifier>{}),
        value{nullptr} {}

  // For interpretation
  Variable(const Identifier& name, ValuePtr&& value,
           const std::vector<Modifier>& modifiers = {}) noexcept
      : name{name}, type{nullptr}, modifiers(modifiers), value{std::move(value)} {}

  bool isConst() const noexcept {
    return std::find(modifiers.cbegin(), modifiers.cend(), Modifier::CONST) != modifiers.cend();
  }

  bool isDefined() const noexcept { return value != nullptr; }

  const Identifier name;
  const TypePtr type;
  const std::vector<Modifier> modifiers;
  ValuePtr value;
};

}  // namespace Interpreter
