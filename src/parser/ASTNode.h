#pragma once

#include <string>

#include "Position.h"

using Identifier = std::wstring;
using TypeIdentifier = std::wstring;

/*
 * @brief Base struct for all the AST nodes.
 */
struct ASTNode {
 public:
  ASTNode(const ASTNode &) = delete;
  ASTNode &operator=(const ASTNode &) = delete;
  ASTNode() = delete;

  ASTNode(ASTNode &&) = default;
  ASTNode &operator=(ASTNode &&) = default;
  virtual ~ASTNode() = default;

  ASTNode(Position &&pos) : position{pos} {}

  Position position;
};
