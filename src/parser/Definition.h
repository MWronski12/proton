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
  Identifier name;

 protected:
  Definition(Position&& position, Identifier&& defName)
      : Statement{std::move(position)}, name{std::move(defName)} {}
};

/* --------------------------------- VarDef --------------------------------- */

/*
 * VarDef
 *     = "var", identifier, ":", typeIdentifier, "=", Expression, ";";
 */
struct VarDef : public Definition, public VisitableNode {
  VarDef(Position&& position, Identifier&& varName, TypeIdentifier&& varType,
         std::unique_ptr<Expression>&& varValue)
      : Definition{std::move(position), std::move(varName)},
        type{std::move(varType)},
        value{std::move(varValue)} {}

  void accept(ASTVisitor& visitor) override { visitor.visit(*this); };

  TypeIdentifier type;
  std::unique_ptr<Expression> value;
};

/* -------------------------------- ConstDef -------------------------------- */

/*
 * ConstDef
 *     = "const", identifier, ":", typeIdentifier, "=", Expression, ";";
 */
struct ConstDef : public Definition, public VisitableNode {
  ConstDef(Position&& position, Identifier&& varName, TypeIdentifier&& varType,
           std::unique_ptr<Expression>&& varValue)
      : Definition{std::move(position), std::move(varName)},
        type{std::move(varType)},
        value{std::move(varValue)} {}

  void accept(ASTVisitor& visitor) override { visitor.visit(*this); };

  TypeIdentifier type;
  std::unique_ptr<Expression> value;
};

/* -------------------------------- StructDef ------------------------------- */

/* StructMember
 *  = identifier, ":", typeIdentifier, ";";
 */
struct StructMember : public ASTNode, public VisitableNode {
  StructMember(Position&& position, Identifier&& memberName, TypeIdentifier&& memberType)
      : ASTNode{std::move(position)}, name{std::move(memberName)}, type{std::move(memberType)} {}

  void accept(ASTVisitor& visitor) override { visitor.visit(*this); };

  Identifier name;
  TypeIdentifier type;
};

/*
 * StructDef
 *     = "struct", identifier, "{", [ StructMember ], "}", ";";
 */
struct StructDef : public Definition, public VisitableNode {
  using Members = std::unordered_map<Identifier, StructMember>;

  StructDef(Position&& position, Identifier&& structName, Members&& structMembers)
      : Definition{std::move(position), std::move(structName)}, members{std::move(structMembers)} {}

  void accept(ASTVisitor& visitor) override { visitor.visit(*this); };

  Members members;
};

/* ------------------------------- VariantDef ------------------------------- */

/*
 * VariantDef
 *     = "variant", identifier, "{", { typeIdentifier } ], "}", ";";
 */
struct VariantDef : public Definition, public VisitableNode {
  using Types = std::vector<TypeIdentifier>;

  VariantDef(Position&& position, Identifier&& variantName, Types&& variantTypes)
      : Definition{std::move(position), std::move(variantName)}, types{std::move(variantTypes)} {}

  void accept(ASTVisitor& visitor) override { visitor.visit(*this); };

  Types types;
};

/* --------------------------------- FnDef ---------------------------------- */

/*
 * FnParam
 *    = [ "const" ], identifier, ":", typeIdentifier;
 */
struct FnParam : public ASTNode, public VisitableNode {
  FnParam(Position&& position, bool paramIsConst, Identifier&& paramName,
          TypeIdentifier&& paramType)
      : ASTNode{std::move(position)},
        isConst{paramIsConst},
        name{std::move(paramName)},
        type{std::move(paramType)} {}

  void accept(ASTVisitor& visitor) override { visitor.visit(*this); };

  bool isConst;
  Identifier name;
  TypeIdentifier type;
};

/*
 * FnDef
 *    = "fn", identifier, "(", { FnParam }, ")", "->", typeIdentifier, BlockStmt;
 */
struct FnDef : public Definition, public VisitableNode {
  using Params = std::unordered_map<Identifier, FnParam>;

  FnDef(Position&& position, Identifier&& fnName, Params&& fnParams, TypeIdentifier&& fnReturnType,
        BlockStmt&& fnBody)
      : Definition{std::move(position), std::move(fnName)},
        parameters{std::move(fnParams)},
        returnType{std::move(fnReturnType)},
        body{std::move(fnBody)} {}

  void accept(ASTVisitor& visitor) override { visitor.visit(*this); };

  Params parameters;
  TypeIdentifier returnType;
  BlockStmt body;
};
