#pragma once

#include "ASTNode.h"

/*
 * expression
 *    = logicOrExpr;
 */
struct Expression : public ASTNode {
 public:
  Expression() = delete;
  virtual ~Expression() = default;

  Expression(const Position& position) : ASTNode{position} {}
};
