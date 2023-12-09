#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "ASTNode.h"
#include "Declaration.h"
#include "Definition.h"

using ProgramDeclarations = std::unordered_map<Identifier, std::unique_ptr<Declaration>>;
using ProgramDefinitions = std::unordered_map<Identifier, std::unique_ptr<Definition>>;

/*
 * Program
 *     = { Declaration | Definition }, MainFunction;
 */
struct Program : public ASTNode {
 public:
  Program(ProgramDeclarations&& declarations, ProgramDefinitions&& definitions)
      : declarations{std::move(declarations)}, definitions{std::move(definitions)} {}

  ProgramDeclarations declarations;
  ProgramDefinitions definitions;
};
