#pragma once

#include <memory>
#include <optional>
#include <unordered_map>
#include <vector>

#include "ASTNode.h"
#include "Expression.h"
#include "Statement.h"

/*
 * Definition
 *     = VarDef
 *     | ConstDef
 *     | StructDef
 *     | VariantDef
 *     | FnDef;
 */
struct Definition : public Statement {
 public:
  Definition(Position&& position, Identifier&& defName)
      : Statement{std::move(position)}, name{std::move(defName)} {}

  Identifier name;
};

/* --------------------------------- VarDef --------------------------------- */

/*
 * VarDef
 *     = "var", identifier, ":", typeIdentifier, "=", expression, ";";
 */
struct VarDef : public Definition {
 public:
  VarDef(Position&& position, Identifier&& varName, TypeIdentifier&& varType,
         std::unique_ptr<Expression>&& varValue)
      : Definition{std::move(position), std::move(varName)},
        type{std::move(varType)},
        value{std::move(varValue)} {}

  TypeIdentifier type;
  std::unique_ptr<Expression> value;
};

/* -------------------------------- ConstDef -------------------------------- */

/*
 * ConstDef
 *     = "const", identifier, ":", typeIdentifier, "=", expression, ";";
 */
struct ConstDef : public Definition {
 public:
  ConstDef(Position&& position, Identifier&& varName, TypeIdentifier&& varType,
           std::unique_ptr<Expression>&& varValue)
      : Definition{std::move(position), std::move(varName)},
        type{std::move(varType)},
        value{std::move(varValue)} {}

  TypeIdentifier type;
  std::unique_ptr<Expression> value;
};

/* -------------------------------- StructDef ------------------------------- */

/*
 * StructDef
 *     = "struct", identifier, "{", [ structMembers ], "}", ";";
 */
struct StructDef : public Definition {
 public:
  /* structMember
   *  = identifier, ":", typeIdentifier, ";";
   */
  struct Member {
    Identifier name;
    TypeIdentifier type;
  };
  /*
   * structMembers
   *  = structMember, { structMember };
   */
  using Members = std::unordered_map<Identifier, Member>;

  StructDef(Position&& position, Identifier&& structName, Members&& structMembers)
      : Definition{std::move(position), std::move(structName)}, members{std::move(structMembers)} {}

  Members members;
};

/* ------------------------------- VariantDef ------------------------------- */

/*
 * VariantDef
 *     = "variant", identifier, "{", [ variantTypes ], "}", ";";
 */
struct VariantDef : public Definition {
 public:
  /*
   * variantType
   *     = typeIdentifier
   */
  using Type = TypeIdentifier;
  /*
   * variantTypes
   *     = variantType, { ",", variantType };
   */
  using Types = std::vector<Type>;

  VariantDef(Position&& position, Identifier&& variantName, Types&& variantTypes)
      : Definition{std::move(position), std::move(variantName)}, types{std::move(variantTypes)} {}

  Types types;
};

/* --------------------------------- FnDef ---------------------------------- */

/*
 * FnDef
 *    = "fn", identifier, "(", [ fnParams ], ")", returnTypeAnnotation, BlockStmt;
 */
struct FnDef : public Definition {
 public:
  /*
   * fnParam
   *    = [ "const" ], identifier, ":", typeIdentifier;
   */
  struct Param {
    Param(bool paramIsConst, Identifier&& paramName, TypeIdentifier&& paramType)
        : isConst{paramIsConst}, name{std::move(paramName)}, type{std::move(paramType)} {}

    bool isConst;
    Identifier name;
    TypeIdentifier type;
  };

  /*
   * fnParams
   *    = fnParam, { ",", fnParam };
   */
  using Params = std::unordered_map<Identifier, Param>;

  /*
   * fnReturnType
   *    = "->", typeIdentifier;
   */
  using ReturnType = TypeIdentifier;

  FnDef(Position&& position, Identifier&& fnName, Params&& fnParams, TypeIdentifier&& fnReturnType,
        BlockStmt&& fnBody)
      : Definition{std::move(position), std::move(fnName)},
        parameters{std::move(fnParams)},
        returnType{std::move(fnReturnType)},
        body{std::move(fnBody)} {}

  Params parameters;
  ReturnType returnType;
  BlockStmt body;
};
