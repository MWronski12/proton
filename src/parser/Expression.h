#pragma once

#include <memory>
#include <optional>
#include <unordered_map>
#include <vector>

#include "ASTNode.h"

/* -------------------------------- Abstract -------------------------------- */

enum class Operator { Add, Sub, Mul, Div, Mod, And, Or, Not, Eq, Neq, Lt, Gt, Leq, Geq };

/*
 * Expression
 *    = BinaryExpression
 *    | UnaryExpression
 *    | FunctionalExpression;
 */
struct Expression : public ASTNode {
 public:
  virtual ~Expression() = default;

 protected:
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
  BinaryExpression(Position&& position, std::unique_ptr<Expression>&& lhs,
                   std::optional<Operator> op, std::unique_ptr<Expression>&& rhs)
      : Expression{std::move(position)}, lhs{std::move(lhs)}, op{op}, rhs{std::move(rhs)} {}

  std::unique_ptr<Expression> lhs;
  std::optional<Operator> op;
  std::unique_ptr<Expression> rhs;
};

/*
 * UnaryExpression
 *    = UnaryOpExpr;
 */
struct UnaryExpression : public Expression {
 public:
  UnaryExpression(Position&& position, std::optional<Operator> op,
                  std::unique_ptr<Expression>&& expr)
      : Expression{std::move(position)}, op{op}, expr{std::move(expr)} {}

  std::optional<Operator> op;
  std::unique_ptr<Expression> expr;
};

/* FunctionalPostfix
 *    = FnCallPostfix
 *    | MemberAccessPostfix
 *    | VariantAccessPostfix;
 */
struct FunctionalPostfix : public ASTNode {
 public:
  FunctionalPostfix(Position&& position) : ASTNode{std::move(position)} {}
};

/*
 * FunctionalExpression
 *    = FnCall
 *    | MemberAccess
 *    | VariantAccess;
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

/* --------------------------- FunctionalPostfixes -------------------------- */

using FnCallArgs = std::vector<std::unique_ptr<Expression>>;

/*
 * FnCallPostfix
 *    = "(", [ Expression, { ",", Expression } ], ")";
 */
struct FnCallPostfix : public FunctionalPostfix {
 public:
  FnCallPostfix(Position&& position, FnCallArgs&& args)
      : FunctionalPostfix{std::move(position)}, args{std::move(args)} {}

  FnCallArgs args;
};

/*
 * MemberAccessPostfix
 *    = ".", Identifier;
 */
struct MemberAccessPostfix : public FunctionalPostfix {
 public:
  MemberAccessPostfix(Position&& position, Identifier&& member)
      : FunctionalPostfix{std::move(position)}, member{std::move(member)} {}

  Identifier member;
};

/*
 * VariantAccessPostfix
 *    = "as", typeIdentifier;
 */
struct VariantAccessPostfix : public FunctionalPostfix {
 public:
  VariantAccessPostfix(Position&& position, TypeIdentifier&& variant)
      : FunctionalPostfix{std::move(position)}, variant{std::move(variant)} {}

  TypeIdentifier variant;
};

/* --------------------------------- Primary -------------------------------- */

/*
 * primaryExpression
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
 * ObjectMember
 *    = Identifier, ":", Expression;
 */
struct ObjectMember {
  ObjectMember(Identifier&& name, std::unique_ptr<Expression>&& value)
      : name{std::move(name)}, value{std::move(value)} {}

  Identifier name;
  std::unique_ptr<Expression> value;
};

/*
 * Object
 *    = "{", { ObjectMembers }, "}";
 */
struct Object : public PrimaryExpression {
 public:
  using Members = std::unordered_map<Identifier, ObjectMember>;

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

enum class PrimitiveType { Int, Float, Bool, Char, String };

/*
 * CastExpr
 *    = primitiveType, "(", Expression, ")";
 */
struct CastExpr : public PrimaryExpression {
 public:
  CastExpr(Position&& position, PrimitiveType type, std::unique_ptr<Expression>&& expr)
      : PrimaryExpression{std::move(position)}, type{type}, expr{std::move(expr)} {}

  PrimitiveType type;
  std::unique_ptr<Expression> expr;
};
