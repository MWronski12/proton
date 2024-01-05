#pragma once

#include <memory>
#include <optional>
#include <type_traits>
#include <unordered_map>
#include <vector>

#include "ASTNode.h"
#include "Position.h"

/* -------------------------------- Abstract -------------------------------- */

enum class Operator { Add, Sub, Mul, Div, Mod, And, Or, Not, Eq, Neq, Lt, Gt, Leq, Geq };

/*
 * Expression
 *    = BinaryExpression
 *    | UnaryExpression
 *    | FunctionalExpression;
 */
struct Expression : public ASTNode {
  virtual ~Expression() = default;

  virtual void accept(ASTVisitor& visitor) = 0;

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
struct BinaryExpression : public Expression, public VisitableNode {
  BinaryExpression(Position&& position, std::unique_ptr<Expression>&& lhs,
                   std::optional<Operator> op, std::unique_ptr<Expression>&& rhs)
      : Expression{std::move(position)}, lhs{std::move(lhs)}, op{op}, rhs{std::move(rhs)} {}

  void accept(ASTVisitor& visitor) override { visitor.visit(*this); };

  std::unique_ptr<Expression> lhs;
  std::optional<Operator> op;
  std::unique_ptr<Expression> rhs;
};

/*
 * UnaryExpression
 *    = UnaryOpExpr;
 */
struct UnaryExpression : public Expression, public VisitableNode {
  UnaryExpression(Position&& position, std::optional<Operator> op,
                  std::unique_ptr<Expression>&& expr)
      : Expression{std::move(position)}, op{op}, expr{std::move(expr)} {}

  void accept(ASTVisitor& visitor) override { visitor.visit(*this); };

  std::optional<Operator> op;
  std::unique_ptr<Expression> expr;
};

/* FunctionalPostfix
 *    = FnCallPostfix
 *    | MemberAccessPostfix
 *    | VariantAccessPostfix;
 */
struct FunctionalPostfix : public ASTNode {
 protected:
  FunctionalPostfix(Position&& position) : ASTNode{std::move(position)} {}
};

/*
 * FunctionalExpression
 *    = FnCall
 *    | MemberAccess
 *    | VariantAccess;
 */
struct FunctionalExpression : public Expression, public VisitableNode {
  FunctionalExpression(Position&& position, std::unique_ptr<Expression>&& expr,
                       std::unique_ptr<FunctionalPostfix>&& postfix)
      : Expression{std::move(position)}, expr{std::move(expr)}, postfix{std::move(postfix)} {}

  void accept(ASTVisitor& visitor) override { visitor.visit(*this); };

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
 protected:
  PrimaryExpression(Position&& position) : Expression{std::move(position)} {}
};

/* --------------------------- FunctionalPostfixes -------------------------- */

using FnCallArgs = std::vector<std::unique_ptr<Expression>>;

/*
 * FnCallPostfix
 *    = "(", [ Expression, { ",", Expression } ], ")";
 */
struct FnCallPostfix : public FunctionalPostfix, public VisitableNode {
  FnCallPostfix(Position&& position, FnCallArgs&& args)
      : FunctionalPostfix{std::move(position)}, args{std::move(args)} {}

  void accept(ASTVisitor& visitor) override { visitor.visit(*this); };

  FnCallArgs args;
};

/*
 * MemberAccessPostfix
 *    = ".", Identifier;
 */
struct MemberAccessPostfix : public FunctionalPostfix, public VisitableNode {
  MemberAccessPostfix(Position&& position, Identifier&& member)
      : FunctionalPostfix{std::move(position)}, member{std::move(member)} {}

  void accept(ASTVisitor& visitor) override { visitor.visit(*this); };

  Identifier member;
};

/*
 * VariantAccessPostfix
 *    = "as", typeIdentifier;
 */
struct VariantAccessPostfix : public FunctionalPostfix, public VisitableNode {
  VariantAccessPostfix(Position&& position, TypeIdentifier&& variant)
      : FunctionalPostfix{std::move(position)}, variant{std::move(variant)} {}

  void accept(ASTVisitor& visitor) override { visitor.visit(*this); };

  TypeIdentifier variant;
};

/* --------------------------------- Primary -------------------------------- */

/*
 * PrimaryExpression
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
struct IdentifierExpr : public PrimaryExpression, public VisitableNode {
  IdentifierExpr(Position&& position, Identifier&& name)
      : PrimaryExpression{std::move(position)}, name{std::move(name)} {}

  void accept(ASTVisitor& visitor) override { visitor.visit(*this); };

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
struct Literal : public PrimaryExpression, public VisitableNode {
  static_assert(
      std::disjunction_v<std::is_same<T, int>, std::is_same<T, float>, std::is_same<T, bool>,
                         std::is_same<T, wchar_t>, std::is_same<T, std::wstring>>,
      "T must be one of the following types: int, float, bool, wchar_t, std::wstring");

  void accept(ASTVisitor& visitor) override { visitor.visit(*this); };

  Literal(Position&& position, T&& value)
      : PrimaryExpression{std::move(position)}, value(std::move(value)) {}
  T value;
};

/*
 * ObjectMember
 *    = Identifier, ":", Expression;
 */
struct ObjectMember : public VisitableNode {
  ObjectMember(Identifier&& name, std::unique_ptr<Expression>&& value)
      : name{std::move(name)}, value{std::move(value)} {}

  void accept(ASTVisitor& visitor) override { visitor.visit(*this); };

  Identifier name;
  std::unique_ptr<Expression> value;
};

/*
 * Object
 *    = "{", { ObjectMember }, "}";
 */
struct Object : public PrimaryExpression, public VisitableNode {
  using Members = std::unordered_map<Identifier, ObjectMember>;

  Object(Position&& position, Members&& members)
      : PrimaryExpression{std::move(position)}, members{std::move(members)} {}

  void accept(ASTVisitor& visitor) override { visitor.visit(*this); };

  Members members;
};

/*
 * ParenExpr
 *    = "(", Expression, ")";
 */
struct ParenExpr : public PrimaryExpression, public VisitableNode {
  ParenExpr(Position&& position, std::unique_ptr<Expression>&& expr)
      : PrimaryExpression{std::move(position)}, expr{std::move(expr)} {}

  void accept(ASTVisitor& visitor) override { visitor.visit(*this); };

  std::unique_ptr<Expression> expr;
};

enum class PrimitiveType { Int, Float, Bool, Char, String };

/*
 * CastExpr
 *    = primitiveType, "(", Expression, ")";
 */
struct CastExpr : public PrimaryExpression, public VisitableNode {
  CastExpr(Position&& position, PrimitiveType type, std::unique_ptr<Expression>&& expr)
      : PrimaryExpression{std::move(position)}, type{type}, expr{std::move(expr)} {}

  void accept(ASTVisitor& visitor) override { visitor.visit(*this); };

  PrimitiveType type;
  std::unique_ptr<Expression> expr;
};
