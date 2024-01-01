#pragma once

#include <vector>

#include "Scope.h"

namespace Interpreter {

/**
 * @brief Data structure that allows for tracking in which function call we currently are.
 */
class StackFrame {
 public:
  StackFrame(){};

  bool contains(const Identifier& name) const {
    for (const auto& scope : scopes) {
      if (scope.contains(name)) {
        return true;
      }
    }
    return false;
  }

  void declare(const Identifier& name, const Identifier& typeName) {
    scopes.back().emplace(name, Variable{name, typeName, std::nullopt});
  }

  Identifier functionName;
  Function function;  // has params and return type info

  std::vector<Scope> scopes;
};

}  // namespace Interpreter
