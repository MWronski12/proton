#pragma once

#include <functional>
#include <variant>

#include "ASTVisitor.h"

struct BlockStmt;

namespace Interpreter {

struct Value;

struct FunctionBody {
  virtual void accept(::ASTVisitor& visitor) = 0;
};

struct UserFunction : public FunctionBody {
  UserFunction(::BlockStmt& body) : body{body} {}

  void accept(::ASTVisitor& visitor) override { visitor.visit(body); }

  ::BlockStmt& body;
};

struct BuiltinFunction : public FunctionBody {
  void accept(::ASTVisitor& visitor) override { visitor.visit(*this); }

  BuiltinFunction(std::vector<std::wstring>&& args,
                  std::function<Value(std::vector<Value>&)>&& body)
      : argNames{std::move(args)}, body{std::move(body)} {}

  std::vector<std::wstring> argNames;
  std::function<Value(std::vector<Value>&)> body;
};

}  // namespace Interpreter
