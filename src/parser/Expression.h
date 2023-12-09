#pragma once

#include "ASTNode.h"

/*
 * expression
 *    = logicOrExpr;
 */
struct Expression : public ASTNode {
 public:
  virtual ~Expression() = default;
};
