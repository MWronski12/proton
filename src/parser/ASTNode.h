#pragma once

#include <string>

#include "ASTVisitor.h"
#include "Position.h"

using Identifier = std::wstring;
using TypeIdentifier = std::wstring;

/**
 * @brief Base struct for all the AST nodes.
 */
struct ASTNode {
  Position position;

 protected:
  ASTNode(const ASTNode &) = delete;
  ASTNode &operator=(const ASTNode &) = delete;
  ASTNode() = delete;

  ASTNode(ASTNode &&) = default;
  ASTNode &operator=(ASTNode &&) = default;
  virtual ~ASTNode() = default;

  virtual void accept(ASTVisitor &visitor) = 0;

  ASTNode(Position &&pos) : position{pos} {}
};

struct VisitableNode {
  virtual void accept(ASTVisitor &visitor) = 0;
};
