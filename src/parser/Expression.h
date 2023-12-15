#pragma once

#include "ASTNode.h"

/* ---------------------------------- Base ---------------------------------- */

/*
 * expression
 *    = logicOrExpr;
 */
struct Expression : public ASTNode {
 public:
  virtual ~Expression() = default;

  Expression(Position&& position) : ASTNode{std::move(position)} {}
};

/* -------------------------------- Abstract -------------------------------- */

struct BinaryExpression : public Expression {
 public:
  BinaryExpression(Position&& position, std::unique_ptr<Expression>&& left,
                   std::unique_ptr<Expression>&& right)
      : Expression{std::move(position)}, left{std::move(left)}, right{std::move(right)} {}

  std::unique_ptr<Expression> left;
  std::unique_ptr<Expression> right;
};

struct UnaryExpression : public Expression {
 public:
  UnaryExpression(Position&& position, std::unique_ptr<Expression>&& expr)
      : Expression{std::move(position)}, expr{std::move(expr)} {}

  std::unique_ptr<Expression> expr;
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

/* --------------------------------- UnaryOp -------------------------------- */

/*
 * unaryOpExpr
 *    = [ unaryOp ], functionalExpr;
 */

/* ! */
struct UnaryOpLogicNegExpr : public UnaryExpression {
 public:
  using UnaryExpression::UnaryExpression;
};

/* - */
struct UnaryOpArithmNegExpr : public UnaryExpression {
 public:
  using UnaryExpression::UnaryExpression;
};

/* ------------------------------- Functional ------------------------------- */

/*
 * functionalExpr
 *    = primaryExpr, { functionalOpExpr };
 *
 * functionalOpExpr
 *    = fnCallOp
 *    | memberAccessOp
 *    | variantAccessOp;
 */

/* foo() */
struct FnCallExpr : public Expression {
 public:
  FnCallExpr(Position&& position, std::unique_ptr<Expression>&& expr,
             std::unique_ptr<FnCallOp>&& fnCallOp)
      : Expression{std::move(position)}, fnCallOp{std::move(fnCallOp)} {}

  std::unique_ptr<Expression> expr;
  std::unique_ptr<FnCallOp> fnCallOp;
};

/*
 * fnCallOp
 *    = "(", [ expression, { ",", expression } ], ")";
 */
struct FnCallOp : public Expression {
 public:
  FnCallOp(Position&& position, std::vector<std::unique_ptr<Expression>>&& args)
      : Expression{std::move(position)}, args{std::move(args)} {}

  std::vector<std::unique_ptr<Expression>> args;
};

/* foo.bar */
struct MemberAccessExpr : public BinaryExpression {
 public:
  using BinaryExpression::BinaryExpression;
};

/* foo as int */
struct VariantAccessExpr : public BinaryExpression {
 public:
  using BinaryExpression::BinaryExpression;
};

/*
 * functionalOpExpr
 *    = fnCallOpExpr
 *    | memberAccessOpExpr
 *    | variantAccessOpExpr;
 */

struct FunctionalOpExpr : public Expression {
 public:
  FunctionalOpExpr(Position&& position, std::unique_ptr<Expression>&& expr)
      : Expression{std::move(position)}, expr{std::move(expr)} {}

  std::unique_ptr<Expression> expr;
};

/*
 * memberAccessOp
 *    = ".", identifier;
 */
struct MemberAccessOp : public Expression {
 public:
  MemberAccessOp(Position&& position, Identifier&& member)
      : Expression{std::move(position)}, member{std::move(member)} {}

  Identifier member;
};

/*
 * variantAccessOp
 *    = "as", typeIdentifier;
 */
struct VariantAccessOp : public Expression {
 public:
  VariantAccessOp(Position&& position, TypeIdentifier&& variant)
      : Expression{std::move(position)}, variant{std::move(variant)} {}

  TypeIdentifier variant;
};

/* --------------------------------- Primary -------------------------------- */

/*
 * primaryExpr
 *    = identifier
 *    | literal
 *    | parenExpr
 *    | castExpr;
 */

struct PrimaryExpr : public Expression {
 public:
  PrimaryExpr(Position&& position, std::unique_ptr<Expression>&& expr)
      : Expression{std::move(position)}, expr{std::move(expr)} {}

  std::unique_ptr<Expression> expr;
};

/*
 * identifier
 *    = [_a-zA-Z][_a-zA-Z0-9]*;
 */
struct IdentifierExpr : public Expression {
 public:
  IdentifierExpr(Position&& position, Identifier&& name)
      : Expression{std::move(position)}, name{std::move(name)} {}

  Identifier name;
};

/*
 * parenExpr
 *    = "(", expression, ")";
 */
struct ParenExpr : public Expression {
 public:
  ParenExpr(Position&& position, std::unique_ptr<Expression>&& expr)
      : Expression{std::move(position)}, expr{std::move(expr)} {}

  std::unique_ptr<Expression> expr;
};

/*
 * castExpr
 *    = primitiveType, "(", expression, ")";
 */
struct CastExpr : public Expression {
 public:
  CastExpr(Position&& position, TypeIdentifier&& type, std::unique_ptr<Expression>&& expr)
      : Expression{std::move(position)}, type{std::move(type)}, expr{std::move(expr)} {}

  TypeIdentifier type;
  std::unique_ptr<Expression> expr;
};
