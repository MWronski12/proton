#pragma once

#include <map>
#include <string>

#include "Type.h"
#include "Value.h"

namespace Interpreter {

/**
 * @brief Data structure representing variable - its name, type and value
 */
struct Variable {
  Identifier name;
  Identifier type;
  std::optional<Value> value;
};

}  // namespace Interpreter
