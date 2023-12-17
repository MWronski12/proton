#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "ASTNode.h"
#include "TokenType.h"

/* -------------------------------- Abstract -------------------------------- */

/*
 * Expression
 *    = ? Any Expression ?;
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
                   std::optional<TokenType> op, std::unique_ptr<Expression>&& right)
      : Expression{std::move(position)}, left{std::move(left)}, op{op}, right{std::move(right)} {}

  std::unique_ptr<Expression> left;
  std::optional<TokenType> op;
  std::unique_ptr<Expression> right;
};

/*
 * UnaryExpression
 *    = UnaryOpExpr;
 */
struct UnaryExpression : public Expression {
 public:
  UnaryExpression(Position&& position, std::optional<TokenType> op,
                  std::unique_ptr<Expression>&& expr)
      : Expression{std::move(position)}, op{op}, expr{std::move(expr)} {}

  std::optional<TokenType> op;
  std::unique_ptr<Expression> expr;
};

/*
 * FunctionalExpression
 *    = FnCall
 *    | MemberAccess
 *    | VariantAccess;
 */
struct FunctionalExpression : public Expression {
 public:
  /* FunctionalPostfix
   *    = FnCallPostfix
   *    | MemberAccessPostfix
   *    | VariantAccessPostfix;
   */
  struct Postfix : public ASTNode {
   public:
    Postfix(Position&& position) : ASTNode{std::move(position)} {}
  };

  FunctionalExpression(Position&& position, std::unique_ptr<Expression>&& expr,
                       std::unique_ptr<Postfix>&& postfix)
      : Expression{std::move(position)}, expr{std::move(expr)}, postfix{std::move(postfix)} {}

  std::unique_ptr<Expression> expr;
  std::unique_ptr<Postfix> postfix;
};

/*
 * PrimaryExpression
 *    = IdentifierExpr
 *    | Literal
 *    | Object
 *    | ParenExpr
 *    | CastExpr;
 */
struct PrimaryExpression : public Expression {
 public:
  PrimaryExpression(Position&& position) : Expression{std::move(position)} {}
};

/* ------------------------------- Functional ------------------------------- */

/*
 * FunctionalExpr
 *   = PrimaryExpr, { funcExprPostfix };
 */

/* foo(x, y, z) */
struct FnCall : public FunctionalExpression {
 public:
  /*
   * fnCallArgs
   *    = Expression, { ",", Expression };
   */
  using Args = std::vector<std::unique_ptr<Expression>>;

  /*
   * fnCallPostfix
   *    = "(", [ Args ], ")";
   */
  struct Postfix : public FunctionalExpression::Postfix {
   public:
    Postfix(Position&& position, Args&& args)
        : FunctionalExpression::Postfix{std::move(position)}, args{std::move(args)} {}

    Args args;
  };

  FnCall(Position&& position, std::unique_ptr<Expression>&& expr,
         std::unique_ptr<Postfix>&& fnCallOp)
      : FunctionalExpression{std::move(position), std::move(expr), std::move(fnCallOp)} {}
};

/* foo.bar */
struct MemberAccess : public FunctionalExpression {
 public:
  /*
   * memberAccessPostfix
   *    = ".", Identifier;
   */
  struct Postfix : public FunctionalExpression::Postfix {
   public:
    Postfix(Position&& position, Identifier&& member)
        : FunctionalExpression::Postfix{std::move(position)}, member{std::move(member)} {}

    Identifier member;
  };

  MemberAccess(Position&& position, std::unique_ptr<Expression>&& expr,
               std::unique_ptr<Postfix>&& postfix)
      : FunctionalExpression{std::move(position), std::move(expr), std::move(postfix)} {}
};

/* foo as int */
struct VariantAccess : public FunctionalExpression {
 public:
  /*
   * variantAccessPostfix
   *    = "as", typeIdentifier;
   */
  struct Postfix : public FunctionalExpression::Postfix {
   public:
    Postfix(Position&& position, TypeIdentifier&& variant)
        : FunctionalExpression::Postfix{std::move(position)}, variant{std::move(variant)} {}

    TypeIdentifier variant;
  };

  VariantAccess(Position&& position, std::unique_ptr<Expression>&& expr,
                std::unique_ptr<Postfix>&& postfix)
      : FunctionalExpression{std::move(position), std::move(expr), std::move(postfix)} {}
};

/* --------------------------------- Primary -------------------------------- */

/*
 * PrimaryExpr
 *    = IdentifierExpr
 *    | literal
 *    | Object
 *    | ParenExpr
 *    | CastExpr;
 */

/*
 * IdentifierExpr
 *    = Identifier;
 */
struct IdentifierExpr : public PrimaryExpression {
 public:
  IdentifierExpr(Position&& position, Identifier&& name)
      : PrimaryExpression{std::move(position)}, name{std::move(name)} {}

  Identifier name;
};

/*
 * Literal
 *    = Int
 *    | Float
 *    | Bool
 *    | Char
 *    | String;
 */

template <typename T>
struct Literal : public PrimaryExpression {
 public:
  Literal(Position&& position, T&& value)
      : PrimaryExpression{std::move(position)}, value(std::move(value)) {}
  T value;
};

/*
 * Object
 *    = "{", [ memberValues ], "}";
 */
struct Object : public PrimaryExpression {
 public:
  struct Member {
    /*
     * memberValue
     *    = Identifier, ":", Expression;
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
 * ParenExpr
 *    = "(", Expression, ")";
 */
struct ParenExpr : public PrimaryExpression {
 public:
  ParenExpr(Position&& position, std::unique_ptr<Expression>&& expr)
      : PrimaryExpression{std::move(position)}, expr{std::move(expr)} {}

  std::unique_ptr<Expression> expr;
};

/*
 * CastExpr
 *    = primitiveType, "(", Expression, ")";
 */
struct CastExpr : public PrimaryExpression {
 public:
  CastExpr(Position&& position, TokenType type, std::unique_ptr<Expression>&& expr)
      : PrimaryExpression{std::move(position)}, type{type}, expr{std::move(expr)} {}

  TokenType type;
  std::unique_ptr<Expression> expr;
};
