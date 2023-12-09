#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "ASTNode.h"
#include "Definition.h"

using ProgramDefinitions = std::unordered_map<Identifier, std::unique_ptr<Definition>>;

/*
 * Program
 *     = { Definition }, MainFunction;
 */
struct Program : public ASTNode {
 public:
  Program(ProgramDefinitions&& definitions) : definitions{std::move(definitions)} {}

  ProgramDefinitions definitions;
};
