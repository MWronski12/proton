#pragma once

#include <string>

#include "Position.h"

using Identifier = std::wstring;

enum class TypeIdentifier {
  Unknown = 0,
  Int,
  Float,
  Bool,
  Char,
  String,
  Void,
};

/*
 * @brief Main struct for all the AST nodes.
 */
struct ASTNode {
 public:
  virtual ~ASTNode() = default;

  std::string filename;
  Position position;
};
