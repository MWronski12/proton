#pragma once

#include <memory>
#include <optional>
#include <vector>

#include "ASTNode.h"
#include "Block.h"
#include "Expression.h"

/*
 * Definition
 *    = VarDef
 *    | StructDef
 *    | VariantDef
 *    | FnDef;
 */
struct Definition : public ASTNode {
 public:
  Definition() = delete;
  Definition(const Definition&) = delete;
  Definition& operator=(const Definition&) = delete;

  Definition(Definition&&) = default;
  Definition& operator=(Definition&&) = default;
  virtual ~Definition() override = default;

  Definition(Identifier&& defName) : name{std::move(defName)} {}

  Identifier name;
};

/* --------------------------------- VarDef --------------------------------- */

/*
 * VarDef
 *     = [ "const" ], identifier, ":", typeIdentifier, "=", expression, ";";
 */
struct VarDef : public Definition {
 public:
  VarDef(bool varIsConst, Identifier&& varName, TypeIdentifier&& varType,
         std::unique_ptr<Expression>&& varValue)
      : Definition{std::move(varName)},
        isConst{varIsConst},
        type{std::move(varType)},
        value{std::move(varValue)} {}

  bool isConst;
  TypeIdentifier type;
  std::unique_ptr<Expression> value;
};

/* -------------------------------- StructDef ------------------------------- */

/*
 * structMembers
 *     = typeIdentifier, { typeIdentifier };
 */
using StructMembers = std::vector<std::unique_ptr<VarDecl>>;

/*
 * StructDef
 *    = "struct", identifier, "{", [ members ], "}", ";";
 */
struct StructDef : public Definition {
 public:
  StructDef(TypeIdentifier&& structName, StructMembers&& structMembers)
      : Definition{std::move(structName)}, members{std::move(structMembers)} {}

  StructMembers members;
};

/* ------------------------------- VariantDef ------------------------------- */

/*
 * variantTypes
 *    = typeIdentifier, { typeIdentifier };
 */
using VariantTypes = std::vector<TypeIdentifier>;

/*
 * VariantDef
 *    = "variant", identifier, "{", { typeIdentifier }, "}", ";";
 */
struct VariantDef : public Definition {
 public:
  VariantDef(TypeIdentifier&& variantName, VariantTypes&& variantTypes)
      : Definition{std::move(variantName)}, types{std::move(variantTypes)} {
    if (types.empty()) {
      throw std::invalid_argument("A variant must have at least one type");
    }
  }

  VariantTypes types;
};

/* --------------------------------- FnDef ---------------------------------- */

/*
 * fnParameter
 *    = [ "const" ], identifier, ":", typeIdentifier;
 */
struct FnParameter {
  FnParameter(bool paramIsConst, Identifier&& paramName, TypeIdentifier&& paramType)
      : isConst{paramIsConst}, name{std::move(paramName)}, type{std::move(paramType)} {}

  bool isConst;
  Identifier name;
  TypeIdentifier type;
};

/*
 * fnParameters
 *    = fnParameter, { ",", fnParameter };
 */
using FnParameters = std::vector<FnParameter>;

/*
 * fnReturnType
 *    = "->", typeIdentifier;
 */
using FnReturnType = TypeIdentifier;

/*
 * FnDef
 *    = "fn", identifier, "(", [ fnParameters ], ")", returnTypeAnnotation, Block;
 */
struct FnDef : public Definition {
 public:
  FnDef(Identifier&& fnName, FnParameters&& fnParameters, TypeIdentifier&& fnReturnType,
        std::unique_ptr<Block>&& fnBody)
      : Definition{std::move(fnName)},
        parameters{std::move(fnParameters)},
        returnType{std::move(fnReturnType)},
        body{std::move(fnBody)} {}

  FnParameters parameters;
  FnReturnType returnType;
  std::unique_ptr<Block> body;
};
