#pragma once

#include <unordered_map>

#include "ASTNode.h"
#include "Definition.h"

/*
 * Program
 *     = { Definition };
 */
struct Program : public ASTNode, public VisitableNode {
  using Definitions = std::vector<std::unique_ptr<Definition>>;

  Program(Position &&position, Definitions &&definitions)
      : ASTNode{std::move(position)}, definitions{std::move(definitions)} {}

  void accept(ASTVisitor &visitor) override { visitor.visit(*this); };

  Definitions definitions;
};
