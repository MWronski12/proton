#pragma once

#include "ASTNode.h"

/*
 * Block
 *     = "{", { Statement }, "}";
 */
struct Block : public ASTNode {
 public:
  Block(Position &&position) : ASTNode{std::move(position)} {}
};
