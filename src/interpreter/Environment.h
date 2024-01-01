#pragma once

#include <optional>
#include <stack>

#include "StackFrame.h"

namespace Interpreter {

struct Environment {
 public:
  Environment() { initGlobalScope(); }

  bool defined(const Identifier& name) const {
    if (!stack.empty()) {
      if (stack.top().contains(name)) {
        return true;
      }
    }
    return globalScope.contains(name);
  }

  bool conflict(const Identifier& name) const {
    return stack.empty() ? globalScope.contains(name) : stack.top().contains(name);
  }

  /** @TODO: Assumes checks were performed */
  void declare(const Identifier& name, const Identifier& typeName) {
    if (stack.empty()) {
      auto type = globalScope.find(typeName)->second;
      globalScope.emplace(name, Variable{name, typeName, std::nullopt});
    } else {
      stack.top().declare(name, typeName);
    }
  }

  Variable lastReturn;
  Variable lastResult;

  std::stack<StackFrame> stack;
  Scope globalScope;

 public:
  void initGlobalScope() { initBuiltinTypes(); }

  void initBuiltinTypes() {
    globalScope.emplace(std::wstring(L"void"), Variable{std::wstring(L"void"), Void{}, std::nullopt});
    globalScope.emplace(std::wstring(L"int"), Int{});
    globalScope.emplace(std::wstring(L"float"), Float{});
    globalScope.emplace(std::wstring(L"bool"), Bool{});
    globalScope.emplace(std::wstring(L"char"), Char{});
    globalScope.emplace(std::wstring(L"string"), String{});
  }
};

}  // namespace Interpreter
