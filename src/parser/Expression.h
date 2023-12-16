#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "ASTNode.h"
#include "TokenType.h"

/* -------------------------------- Abstract -------------------------------- */

/*
 * Expression
 *    = ? Any expression ?;
 */
struct Expression : public ASTNode {
 public:
  virtual ~Expression() = default;

  Expression(Position&& position) : ASTNode{std::move(position)} {}
};

/*
 * BinaryExpression
 *    = LogicalOrExpr
 *    | LogicAndExpr
 *    | EqualityExpr
 *    | RelationalExpr
 *    | AdditiveExpr
 *    | MultiplicativeExpr;
 */
struct BinaryExpression : public Expression {
 public:
  BinaryExpression(Position&& position, std::unique_ptr<Expression>&& left,
                   std::unique_ptr<Expression>&& right)
      : Expression{std::move(position)}, left{std::move(left)}, right{std::move(right)} {}

  std::unique_ptr<Expression> left;
  std::unique_ptr<Expression> right;
};

/*
 * UnaryExpression
 *    = UnaryOpExpr;
 */
struct UnaryExpression : public Expression {
 public:
  UnaryExpression(Position&& position, std::unique_ptr<Expression>&& expr)
      : Expression{std::move(position)}, expr{std::move(expr)} {}

  std::unique_ptr<Expression> expr;
};

/* FunctionalPostfix
 *    = FnCallOp
 *    | MemberAccessOp
 *    | VariantAccessOp;
 */
struct FunctionalPostfix : public ASTNode {
 public:
  FunctionalPostfix(Position&& position) : ASTNode{std::move(position)} {}
};

/*
 * FunctionalExpression
 *    = FnCallExpr
 *    | MemberAccessExpr
 *    | VariantAccessExpr;
 */
struct FunctionalExpression : public Expression {
 public:
  FunctionalExpression(Position&& position, std::unique_ptr<Expression>&& expr,
                       std::unique_ptr<FunctionalPostfix>&& postfix)
      : Expression{std::move(position)}, expr{std::move(expr)}, postfix{std::move(postfix)} {}

  std::unique_ptr<Expression> expr;
  std::unique_ptr<FunctionalPostfix> postfix;
};

/*
 * primaryExpr
 *    = identifierExpr
 *    | literal
 *    | object
 *    | parenExpr
 *    | castExpr;
 */
struct PrimaryExpression : public Expression {
 public:
  PrimaryExpression(Position&& position) : Expression{std::move(position)} {}
};

/* ---------------------------------- Logic --------------------------------- */

/*
 * logicOrExpr
 *    = logicAndExpr, { logicOrOp, logicAndExpr };
 */

/* || */
struct LogicOrExpr : public BinaryExpression {
 public:
  using BinaryExpression::BinaryExpression;
};

/*
 * logicAndExpr
 *    = equalityExpr, { logicAndOp, equalityExpr };
 */

/* && */
struct LogicAndExpr : public BinaryExpression {
 public:
  using BinaryExpression::BinaryExpression;
};

/* -------------------------------- Equality -------------------------------- */

/*
 * equalityExpr
 *    = relationalExpr, { equalityOp, relationalExpr };
 */

/* == */
struct EqualityIsExpr : public BinaryExpression {
 public:
  using BinaryExpression::BinaryExpression;
};

/* != */
struct EqualityIsNotExpr : public BinaryExpression {
 public:
  using BinaryExpression::BinaryExpression;
};

/* ------------------------------- Relational ------------------------------- */

/*
 * relationalExpr
 *    = additiveExpr, { relationalOp, additiveExpr };
 */

/* < */
struct RelationalLessThanExpr : public BinaryExpression {
 public:
  using BinaryExpression::BinaryExpression;
};

/* <= */
struct RelationalLessThanOrEqualExpr : public BinaryExpression {
 public:
  using BinaryExpression::BinaryExpression;
};

/* > */
struct RelationalGreaterThanExpr : public BinaryExpression {
 public:
  using BinaryExpression::BinaryExpression;
};

/* >= */
struct RelationalGreaterThanOrEqualExpr : public BinaryExpression {
 public:
  using BinaryExpression::BinaryExpression;
};

/* -------------------------------- Additive -------------------------------- */

/*
 * additiveExpr
 *    = multiplicativeExpr, { additiveOp, multiplicativeExpr };
 */

/* + */
struct AdditivePlusExpr : public BinaryExpression {
 public:
  using BinaryExpression::BinaryExpression;
};

/* - */
struct AdditiveMinusExpr : public BinaryExpression {
 public:
  using BinaryExpression::BinaryExpression;
};

/* ----------------------------- Multiplicative ----------------------------- */

/*
 * multiplicativeExpr
 *    = unaryExpr, { multiplicativeOp, unaryExpr };
 */

/* * */
struct MultiplicativeMulExpr : public BinaryExpression {
 public:
  using BinaryExpression::BinaryExpression;
};

/* / */
struct MultiplicativeDivExpr : public BinaryExpression {
 public:
  using BinaryExpression::BinaryExpression;
};

/* % */
struct MultiplicativeModExpr : public BinaryExpression {
 public:
  using BinaryExpression::BinaryExpression;
};

/* ---------------------------------- Unary --------------------------------- */

/*
 * unaryOpExpr
 *    = [ unaryOp ], functionalExpr;
 */

/* ! */
struct LogicNegExpr : public UnaryExpression {
 public:
  using UnaryExpression::UnaryExpression;
};

/* - */
struct ArithmNegExpr : public UnaryExpression {
 public:
  using UnaryExpression::UnaryExpression;
};

/* ------------------------------- Functional ------------------------------- */

/*
 * functionalExpr
 *   = primaryExpr, { functionalPostfix };
 */

/*
 * fnCallOp
 *    = "(", [ expression, { ",", expression } ], ")";
 */
struct FnCallOp : public FunctionalPostfix {
 public:
  FnCallOp(Position&& position, std::vector<std::unique_ptr<Expression>>&& args)
      : FunctionalPostfix{std::move(position)}, args{std::move(args)} {}

  std::vector<std::unique_ptr<Expression>> args;
};

/* foo(x, y, z) */
struct FnCallExpr : public FunctionalExpression {
 public:
  FnCallExpr(Position&& position, std::unique_ptr<Expression>&& expr,
             std::unique_ptr<FnCallOp>&& fnCallOp)
      : FunctionalExpression{std::move(position), std::move(expr), std::move(fnCallOp)} {}
};

/*
 * memberAccessOp
 *    = ".", identifier;
 */
struct MemberAccessOp : public FunctionalPostfix {
 public:
  MemberAccessOp(Position&& position, Identifier&& member)
      : FunctionalPostfix{std::move(position)}, member{std::move(member)} {}

  Identifier member;
};

/* foo.bar */
struct MemberAccessExpr : public FunctionalExpression {
 public:
  MemberAccessExpr(Position&& position, std::unique_ptr<Expression>&& expr,
                   std::unique_ptr<MemberAccessOp>&& memberAccessOp)
      : FunctionalExpression{std::move(position), std::move(expr), std::move(memberAccessOp)} {}
};

/*
 * variantAccessOp
 *    = "as", typeIdentifier;
 */
struct VariantAccessOp : public FunctionalPostfix {
 public:
  VariantAccessOp(Position&& position, TypeIdentifier&& variant)
      : FunctionalPostfix{std::move(position)}, variant{std::move(variant)} {}

  TypeIdentifier variant;
};

/* foo as int */
struct VariantAccessExpr : public FunctionalExpression {
 public:
  VariantAccessExpr(Position&& position, std::unique_ptr<Expression>&& expr,
                    std::unique_ptr<VariantAccessOp>&& variantAccessOp)
      : FunctionalExpression{std::move(position), std::move(expr), std::move(variantAccessOp)} {}
};

/* --------------------------------- Primary -------------------------------- */

/*
 * primaryExpr
 *    = identifierExpr
 *    | literal
 *    | object
 *    | parenExpr
 *    | castExpr;
 */

/*
 * identifierExpr
 *    = identifier;
 */
struct IdentifierExpr : public PrimaryExpression {
 public:
  IdentifierExpr(Position&& position, Identifier&& name)
      : PrimaryExpression{std::move(position)}, name{std::move(name)} {}

  Identifier name;
};

/*
 * literal
 *    = int
 *    | float
 *    | bool
 *    | char
 *    | string;
 */

template <typename T>
struct Literal : public PrimaryExpression {
 public:
  Literal(Position&& position, T&& value)
      : PrimaryExpression{std::move(position)}, value(std::move(value)) {}
  T value;
};

/*
 * object
 *    = "{", [ memberValues ], "}";
 */
struct Object : public PrimaryExpression {
 public:
  struct Member {
    /*
     * memberValue
     *    = identifier, ":", expression;
     */
    Member(Identifier&& name, std::unique_ptr<Expression>&& value)
        : name{std::move(name)}, value{std::move(value)} {}

    Identifier name;
    std::unique_ptr<Expression> value;
  };

  /*
   * memberValues
   *    = memberValue, { ",", memberValue };
   */
  using Members = std::unordered_map<Identifier, Member>;

  Object(Position&& position, Members&& members)
      : PrimaryExpression{std::move(position)}, members{std::move(members)} {}

  Members members;
};

/*
 * parenExpr
 *    = "(", expression, ")";
 */
struct ParenExpr : public PrimaryExpression {
 public:
  ParenExpr(Position&& position, std::unique_ptr<Expression>&& expr)
      : PrimaryExpression{std::move(position)}, expr{std::move(expr)} {}

  std::unique_ptr<Expression> expr;
};

/*
 * castExpr
 *    = primitiveType, "(", expression, ")";
 */
struct CastExpr : public PrimaryExpression {
 public:
  CastExpr(Position&& position, TokenType type, std::unique_ptr<Expression>&& expr)
      : PrimaryExpression{std::move(position)}, type{type}, expr{std::move(expr)} {}

  TokenType type;
  std::unique_ptr<Expression> expr;
};
