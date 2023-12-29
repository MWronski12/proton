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
 *     = "var", identifier, ":", typeIdentifier, "=", Expression, ";";
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
 *     = "const", identifier, ":", typeIdentifier, "=", Expression, ";";
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

/* StructMember
 *  = identifier, ":", typeIdentifier, ";";
 */
struct StructMember : public ASTNode {
  StructMember(Position&& position, Identifier&& memberName, TypeIdentifier&& memberType)
      : ASTNode{std::move(position)}, name{std::move(memberName)}, type{std::move(memberType)} {}

  Identifier name;
  TypeIdentifier type;
};

/*
 * StructDef
 *     = "struct", identifier, "{", [ StructMember ], "}", ";";
 */
struct StructDef : public Definition {
 public:
  using Members = std::unordered_map<Identifier, StructMember>;

  StructDef(Position&& position, Identifier&& structName, Members&& structMembers)
      : Definition{std::move(position), std::move(structName)}, members{std::move(structMembers)} {}

  Members members;
};

/* ------------------------------- VariantDef ------------------------------- */

/*
 * VariantDef
 *     = "variant", identifier, "{", { typeIdentifier } ], "}", ";";
 */
struct VariantDef : public Definition {
 public:
  using Types = std::vector<TypeIdentifier>;

  VariantDef(Position&& position, Identifier&& variantName, Types&& variantTypes)
      : Definition{std::move(position), std::move(variantName)}, types{std::move(variantTypes)} {}

  Types types;
};

/* --------------------------------- FnDef ---------------------------------- */

/*
 * FnParam
 *    = [ "const" ], identifier, ":", typeIdentifier;
 */
struct FnParam : public ASTNode {
  FnParam(Position&& position, bool paramIsConst, Identifier&& paramName,
          TypeIdentifier&& paramType)
      : ASTNode{std::move(position)},
        isConst{paramIsConst},
        name{std::move(paramName)},
        type{std::move(paramType)} {}

  bool isConst;
  Identifier name;
  TypeIdentifier type;
};

/*
 * FnDef
 *    = "fn", identifier, "(", { FnParam }, ")", "->", typeIdentifier, BlockStmt;
 */
struct FnDef : public Definition {
 public:
  using Params = std::unordered_map<Identifier, FnParam>;

  FnDef(Position&& position, Identifier&& fnName, Params&& fnParams, TypeIdentifier&& fnReturnType,
        BlockStmt&& fnBody)
      : Definition{std::move(position), std::move(fnName)},
        parameters{std::move(fnParams)},
        returnType{std::move(fnReturnType)},
        body{std::move(fnBody)} {}

  Params parameters;
  TypeIdentifier returnType;
  BlockStmt body;
};
