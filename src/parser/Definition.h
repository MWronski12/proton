#pragma once

#include <memory>
#include <optional>
#include <unordered_map>
#include <vector>

#include "ASTNode.h"
#include "Block.h"
#include "Expression.h"

/*
 * Definition
 *     = VarDef
 *     | ConstDef
 *     | StructDef
 *     | VariantDef
 *     | FnDef;
 */
struct Definition : public ASTNode {
 public:
  Definition() = delete;
  Definition(const Definition&) = delete;
  Definition& operator=(const Definition&) = delete;

  Definition(Definition&&) = default;
  Definition& operator=(Definition&&) = default;
  virtual ~Definition() override = default;

  Definition(const Position& position, Identifier&& defName)
      : ASTNode{position}, name{std::move(defName)} {}

  Identifier name;
};

/* --------------------------------- VarDef --------------------------------- */

/*
 * VarDef
 *     = "var", identifier, ":", typeIdentifier, "=", expression, ";";
 */
struct VarDef : public Definition {
 public:
  VarDef(const Position& position, Identifier&& varName, TypeIdentifier&& varType,
         std::unique_ptr<Expression>&& varValue)
      : Definition{position, std::move(varName)},
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
  ConstDef(const Position& position, Identifier&& varName, TypeIdentifier&& varType,
           std::unique_ptr<Expression>&& varValue)
      : Definition{position, std::move(varName)},
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

  StructDef(const Position& position, Identifier&& structName, Members&& structMembers)
      : Definition{position, std::move(structName)}, members{std::move(structMembers)} {}

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

  VariantDef(const Position& position, Identifier&& variantName, Types&& variantTypes)
      : Definition{position, std::move(variantName)}, types{std::move(variantTypes)} {}

  Types types;
};

/* --------------------------------- FnDef ---------------------------------- */

/*
 * FnDef
 *    = "fn", identifier, "(", [ fnParams ], ")", returnTypeAnnotation, Block;
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
  using Params = std::vector<Param>;

  /*
   * fnReturnType
   *    = "->", typeIdentifier;
   */
  using ReturnType = TypeIdentifier;

  FnDef(const Position& position, Identifier&& fnName, Params&& fnParams,
        TypeIdentifier&& fnReturnType, Block&& fnBody)
      : Definition{position, std::move(fnName)},
        parameters{std::move(fnParams)},
        returnType{std::move(fnReturnType)},
        body{std::move(fnBody)} {}

  Params parameters;
  ReturnType returnType;
  Block body;
};
