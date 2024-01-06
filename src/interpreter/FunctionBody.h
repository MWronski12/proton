#pragma once

#include <functional>
#include <variant>

#include "ASTVisitor.h"

struct Value;
struct BlockStmt;

namespace Interpreter {

struct FunctionBody {
  virtual void accept(::ASTVisitor& visitor) = 0;
};

struct UserDefinedFunctionBody : public FunctionBody {
  void accept(::ASTVisitor& visitor) override { visitor.visit(*body); }

  ::BlockStmt* body;
};

struct BuiltinFunctionBody : public FunctionBody {
  void accept(::ASTVisitor& visitor) override { visitor.visit(*this); }

  std::function<Value(const std::vector<Value>&)> body;
};

}  // namespace Interpreter
