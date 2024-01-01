#pragma once

#include <map>

#include "Variable.h"

namespace Interpreter {

using Scope = std::map<Identifier, Variable>;

}  // namespace Interpreter
