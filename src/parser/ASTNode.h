#pragma once

#include <string>

#include "Position.h"

using Identifier = std::wstring;
using TypeIdentifier = std::wstring;

/*
 * @brief Main struct for all the AST nodes.
 */
struct ASTNode {
 public:
  virtual ~ASTNode() = default;

  std::string filename;
  Position position;
};
