#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "ASTNode.h"
#include "Definition.h"

/*
 * Program
 *     = { Definition };
 */
struct Program : public ASTNode {
 public:
  using IdentifierToDefinitionPtrMap = std::unordered_map<Identifier, std::unique_ptr<Definition>>;

  Program(Position&& position, IdentifierToDefinitionPtrMap&& definitions)
      : ASTNode{std::move(position)}, definitions{std::move(definitions)} {}

  IdentifierToDefinitionPtrMap definitions;
};
