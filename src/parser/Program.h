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
  using Definitions = std::unordered_map<Identifier, std::unique_ptr<Definition>>;

  Program(Definitions&& definitions) : definitions{std::move(definitions)} {}

  Definitions definitions;
};
