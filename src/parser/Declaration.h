#pragma once

#include "ASTNode.h"

/*
 * Declaration = VarDecl;
 */
struct Declaration : public ASTNode {
 public:
  Declaration() = delete;
  Declaration(const Declaration&) = delete;
  Declaration& operator=(const Declaration&) = delete;

  Declaration(Declaration&&) = default;
  Declaration& operator=(Declaration&&) = default;
  virtual ~Declaration() = default;

  Declaration(Identifier&& declName, TypeIdentifier&& declType)
      : name{std::move(declName)}, type{std::move(declType)} {}

  Identifier name;
  TypeIdentifier type;
};

/* --------------------------------- VarDecl -------------------------------- */

/*
 * VarDecl = identifier, ":", typeIdentifier, ";";
 */
struct VarDecl : public Declaration {
 public:
  using Declaration::Declaration;
};
