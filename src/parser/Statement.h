#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "ASTNode.h"

/*
 * Statement
 *    = Definition
 *    | BlockStmt
 *    | ExpressionOrAssignmentStmt
 *    | StdinExtractionStmt
 *    | StdoutInsertionStmt
 *    | VariantMatchStmt
 *    | IfStmt
 *    | ForStmt
 *    | WhileStmt
 *    | ContinueStmt
 *    | BreakStmt
 *    | ReturnStmt;
 */
struct Statement : public ASTNode {
 public:
  Statement(Position &&position) : ASTNode{std::move(position)} {}
};

/*
 * BlockStmt
 *     = "{", { Statement }, "}";
 */
struct BlockStmt : public Statement {
 public:
  using Statements = std::vector<std::unique_ptr<Statement>>;

  BlockStmt(Position &&position, Statements &&statements)
      : Statement{std::move(position)}, statements{std::move(statements)} {}

  Statements statements;
};

/*
 * ExpressionOrAssignmentStatement
 *     = Expression, [ "=", Expression ], ";";
 */
struct ExpressionStmt : public Statement {
 public:
  ExpressionStmt(Position &&position, std::unique_ptr<Expression> &&expr)
      : Statement{std::move(position)}, expr{std::move(expr)} {}

  std::unique_ptr<Expression> expr;
};

/*
 * ExpressionOrAssignmentStatement
 *     = Expression, [ "=", Expression ], ";";
 */
struct AssignmentStmt : public Statement {
 public:
  AssignmentStmt(Position &&position, std::unique_ptr<Expression> &&left,
                 std::unique_ptr<Expression> &&right)
      : Statement{std::move(position)}, left{std::move(left)}, right{std::move(right)} {}

  std::unique_ptr<Expression> left;
  std::unique_ptr<Expression> right;
};

/*
 * StdinExtractionStmt
 *     = ">>", Expression, { ">>", Expression }, ";";
 */
struct StdinExtractionStmt : public Statement {
 public:
  StdinExtractionStmt(Position &&position, std::vector<std::unique_ptr<Expression>> &&expressions)
      : Statement{std::move(position)}, expressions{std::move(expressions)} {}

  std::vector<std::unique_ptr<Expression>> expressions;
};

/*
 * StdoutInsertionStmt
 *     = "<<", Expression, { "<<", Expression }, ";";
 */
struct StdoutInsertionStmt : public Statement {
 public:
  StdoutInsertionStmt(Position &&position, std::vector<std::unique_ptr<Expression>> &&expressions)
      : Statement{std::move(position)}, expressions{std::move(expressions)} {}

  std::vector<std::unique_ptr<Expression>> expressions;
};

/*
 * VariantMatch
 *     = "match", Expression, "{", variantMatchCases , "}";
 */
struct VariantMatchStmt : public Statement {
 public:
  struct Case : public ASTNode {
   public:
    /* variantCase
     *    = "case", typeIdentifier, "->", BlockStmt;
     */
    Case(Position &&position, TypeIdentifier &&variant, BlockStmt &&block)
        : ASTNode{std::move(position)}, variant{std::move(variant)}, block{std::move(block)} {}

    TypeIdentifier variant;
    BlockStmt block;
  };
  /*
   * variantCases
   *    = { variantCase };
   */
  using Cases = std::unordered_map<TypeIdentifier, Case>;

  VariantMatchStmt(Position &&position, std::unique_ptr<Expression> &&expr, Cases &&cases)
      : Statement{std::move(position)}, expr{std::move(expr)}, cases{std::move(cases)} {}

  std::unique_ptr<Expression> expr;
  Cases cases;
};

/*
 * IfStmt
 *     = "if", Expression, BlockStmt, { Elif }, [ Else ];
 */
struct IfStmt : public Statement {
 public:
  struct Elif : public ASTNode {
   public:
    /*
     * Elif
     *     = "elif", Expression, BlockStmt;
     */
    Elif(Position &&position, std::unique_ptr<Expression> &&condition, BlockStmt &&block)
        : ASTNode{std::move(position)}, condition{std::move(condition)}, block{std::move(block)} {}

    std::unique_ptr<Expression> condition;
    BlockStmt block;
  };

  /*
   * Else
   *     = "else", BlockStmt;
   */
  struct Else : public ASTNode {
   public:
    Else(Position &&position, BlockStmt &&block)
        : ASTNode{std::move(position)}, block{std::move(block)} {}

    BlockStmt block;
  };

  /*
   * Elifs
   *     = { Elif };
   */
  using Elifs = std::vector<Elif>;

  IfStmt(Position &&position, std::unique_ptr<Expression> &&condition, BlockStmt &&block,
         Elifs &&elifs, std::optional<Else> &&elseClause)
      : Statement{std::move(position)},
        condition{std::move(condition)},
        block{std::move(block)},
        elifs{std::move(elifs)},
        elseClause{std::move(elseClause)} {}

  std::unique_ptr<Expression> condition;
  BlockStmt block;
  Elifs elifs;
  std::optional<Else> elseClause;
};

/*
 * ForStmt
 *     = "for", Identifier, "in", Range, BlockStmt;
 */
struct ForStmt : public Statement {
 public:
  /*
   * Range
   *     = Expression, "until", Expression;
   */
  struct Range : public ASTNode {
   public:
    Range(Position &&position, std::unique_ptr<Expression> &&start,
          std::unique_ptr<Expression> &&end)
        : ASTNode{std::move(position)}, start{std::move(start)}, end{std::move(end)} {}

    std::unique_ptr<Expression> start;
    std::unique_ptr<Expression> end;
  };

  ForStmt(Position &&position, Identifier &&identifier, Range &&range, BlockStmt &&block)
      : Statement{std::move(position)},
        identifier{std::move(identifier)},
        range{std::move(range)},
        block{std::move(block)} {}

  Identifier identifier;
  Range range;
  BlockStmt block;
};

/*
 * WhileStmt
 *     = "while", Expression, BlockStmt;
 */
struct WhileStmt : public Statement {
 public:
  WhileStmt(Position &&position, std::unique_ptr<Expression> &&expr, BlockStmt &&block)
      : Statement{std::move(position)}, expr{std::move(expr)}, block{std::move(block)} {}

  std::unique_ptr<Expression> expr;
  BlockStmt block;
};

/*
 * ContinueStmt
 *     = "continue", ";";
 */
struct ContinueStmt : public Statement {
 public:
  ContinueStmt(Position &&position) : Statement{std::move(position)} {}
};

/*
 * BreakStmt
 *     = "break", ";";
 */
struct BreakStmt : public Statement {
 public:
  BreakStmt(Position &&position) : Statement{std::move(position)} {}
};

/*
 * ReturnStmt
 *     = "return", [ Expression ], ";";
 */
struct ReturnStmt : public Statement {
 public:
  ReturnStmt(Position &&position, std::unique_ptr<Expression> &&expr)
      : Statement{std::move(position)}, expr{std::move(expr)} {}

  std::unique_ptr<Expression> expr;
};
