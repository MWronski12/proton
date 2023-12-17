#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <type_traits>
#include <variant>

#include "Definition.h"
#include "ErrorHandler.h"
#include "Expression.h"
#include "Lexer.h"
#include "Statement.h"
#include "parser_utils.h"

// todo remove
#include "ErrorType.h"
#include "TokenType.h"

/*
 * Program
 *     = { Definition };
 */
struct Program : public ASTNode {
 public:
  using IdentifierToDefinitionPtrMap = std::unordered_map<Identifier, std::unique_ptr<Definition>>;

  Program(Position &&position, IdentifierToDefinitionPtrMap &&definitions)
      : ASTNode{std::move(position)}, definitions{std::move(definitions)} {}

  IdentifierToDefinitionPtrMap definitions;
};

class Parser {
 public:
  Parser(Lexer &lexer, ErrorHandler &errorHandler);

  std::optional<Program> parseProgram();

  friend class ParserTest;

 private:
  /* ----------------------------- Utility methods ---------------------------- */

  void consumeToken();
  void skipError(const TokenType delimiter);
  bool consumeIf(TokenType expectedType, ErrorType error);
  bool consumeIf(const std::function<bool(TokenType tokenType)> &predicate, ErrorType err);

  std::optional<Identifier> parseIdentifier();
  std::optional<TypeIdentifier> parseTypeIdentifier();

  /* ------------------------------- Definitions ------------------------------ */

  std::unique_ptr<Definition> parseDefinition();
  std::unique_ptr<Definition> parseVarDef();
  std::unique_ptr<Definition> parseConstDef();

  std::unique_ptr<Definition> parseStructDef();
  std::optional<StructDef::Members> parseStructMembers();
  std::optional<StructDef::Member> parseStructMember();

  std::unique_ptr<Definition> parseVariantDef();
  std::optional<VariantDef::Types> parseVariantTypes();
  std::optional<VariantDef::Type> parseVariantType();

  std::unique_ptr<Definition> parseFnDef();
  std::optional<FnDef::Params> parseFnParams();
  std::optional<FnDef::Param> parseFnParam();
  std::optional<FnDef::ReturnType> parseFnReturnType();

  /* ------------------------------- Expressions ------------------------------ */

  std::unique_ptr<Expression> parseExpression() { return parseLogicOrExpr(); }

  /*
   * BinaryExpression
   *    = LogicalOrExpr
   *    | LogicAndExpr
   *    | EqualityExpr
   *    | RelationalExpr
   *    | AdditiveExpr
   *    | MultiplicativeExpr;
   */
  std::unique_ptr<Expression> parseBinaryExpression(
      std::unique_ptr<Expression> (Parser::*parseSubExpr)(), std::vector<TokenType> &&operators) {
    // Parse left subexpression
    std::unique_ptr<Expression> lhs;
    if ((lhs = (this->*parseSubExpr)()) == nullptr) return nullptr;

    // While the next token is a binary operator, consume it and parse right subexpression
    while (std::find(operators.cbegin(), operators.cend(), m_token.type) != operators.cend()) {
      consumeToken();
      std::unique_ptr<Expression> rhs;
      if ((rhs = (this->*parseSubExpr)()) == nullptr) {
        m_errorHandler.handleError(ErrorType::BINARYEXPRESSION_EXPECTED_RHS, m_token.position);
        return nullptr;
      };

      // Merge rhs into lhs
      auto position = lhs->position;
      lhs = std::make_unique<BinaryExpression>(std::move(position), std::move(lhs), std::move(rhs));
    }

    return lhs;
  }

  /*
   * UnaryExpression
   *    = UnaryOpExpr;
   */
  std::unique_ptr<Expression> parseUnaryExpression(
      std::unique_ptr<Expression> (Parser::*parseSubExpr)(), std::vector<TokenType> &&operators) {
    auto position = m_token.position;

    if (std::find(operators.cbegin(), operators.cend(), m_token.type) == operators.cend()) {
      return nullptr;
    }
    consumeToken();

    auto expr = (this->*parseSubExpr)();
    if (expr == nullptr) {
      m_errorHandler.handleError(ErrorType::UNARYEXPRESSION_EXPECTED_EXPR, m_token.position);
      return nullptr;
    }

    return std::make_unique<UnaryExpression>(std::move(position), std::move(expr));
  }

  /*
   * LogicOrExpr
   *     = LogicAndExpr, { logicOrOp, LogicAndExpr };
   */
  std::unique_ptr<Expression> parseLogicOrExpr() {
    return parseBinaryExpression(&Parser::parseLogicAndExpr, {TokenType::LOGIC_OR});
  }

  /*
   * LogicAndExpr
   *     = EqualityExpr, { logicAndOp, EqualityExpr };
   */
  std::unique_ptr<Expression> parseLogicAndExpr() {
    return parseBinaryExpression(&Parser::parseEqualityExpr, {TokenType::LOGIC_AND});
  }

  /*
   * EqualityExpr
   *     = RelationalExpr, { equalityOp, RelationalExpr };
   */
  std::unique_ptr<Expression> parseEqualityExpr() {
    return parseBinaryExpression(&Parser::parseRelationalExpr,
                                 {TokenType::EQUALITY, TokenType::INEQUALITY});
  }

  /*
   * RelationalExpr
   *     = AdditiveExpr, { relationalOp, AdditiveExpr };
   */
  std::unique_ptr<Expression> parseRelationalExpr() {
    return parseBinaryExpression(&Parser::parseAdditiveExpr,
                                 {TokenType::LESS_THAN, TokenType::GREATER_THAN,
                                  TokenType::LESS_OR_EQUAL, TokenType::GREATER_OR_EQUAL});
  }

  /*
   * AdditiveExpr
   *     = MultiplicativeExpr, { additiveOp, MultiplicativeExpr };
   */
  std::unique_ptr<Expression> parseAdditiveExpr() {
    return parseBinaryExpression(&Parser::parseMultiplicativeExpr,
                                 {TokenType::PLUS, TokenType::MINUS});
  }

  /*
   * MultiplicativeExpr
   *     = UnaryExpr, { multiplicativeOp, UnaryExpr };
   */
  std::unique_ptr<Expression> parseMultiplicativeExpr() {
    return parseBinaryExpression(&Parser::parseUnaryOpExpr,
                                 {TokenType::ASTERISK, TokenType::SLASH, TokenType::PERCENT});
  }

  /*
   * UnaryOpExpr
   *     = [ unaryOp ], PrimaryExpr;
   */
  std::unique_ptr<Expression> parseUnaryOpExpr() {
    return parseUnaryExpression(&Parser::parseFunctionalExpression,
                                {TokenType::PLUS, TokenType::MINUS, TokenType::LOGIC_NOT});
  }

  std::unique_ptr<Expression> parsePrimaryExpression();
  std::unique_ptr<Expression> parseIdentifierExpr();
  template <typename T>
  std::unique_ptr<Expression> parseLiteral() {
    if (!isLiteralT<T>(m_token.type)) {
      return nullptr;
    }

    auto position = m_token.position;
    T value;
    try {
      value = std::get<T>(m_token.value);
    } catch (const std::bad_variant_access &) {
      m_errorHandler.handleError(ErrorType::TOKEN_INVARIANT_VIOLATION, position);
      return nullptr;
    }

    consumeToken();
    return std::make_unique<Literal<T>>(std::move(position), std::move(value));
  }

  std::unique_ptr<Expression> parseObject();
  std::optional<Object::Members> parseObjectMembers();
  std::optional<Object::Member> parseObjectMember();

  std::unique_ptr<Expression> parseParenExpr();
  std::unique_ptr<Expression> parseCastExpr();

  std::unique_ptr<Expression> parseFunctionalExpression();
  std::unique_ptr<FunctionalExpression::Postfix> parseFunctionalExpressionPostfix();
  std::unique_ptr<FunctionalExpression::Postfix> parseMemberAccessPostfix();
  std::unique_ptr<FunctionalExpression::Postfix> parseVariantAccessPostfix();
  std::unique_ptr<FunctionalExpression::Postfix> parseFnCallPostfix();
  std::optional<FnCall::Args> parseFnCallArgs();

  /* ------------------------------- Statements ------------------------------- */

  std::unique_ptr<Statement> parseStatement();
  std::unique_ptr<Statement> parseBlockStmt();
  std::unique_ptr<Statement> parseExpressionOrAssignmentStmt();
  std::unique_ptr<Statement> parseStdinExtractionStmt();
  std::unique_ptr<Statement> parseStdoutInsertionStmt();

  std::unique_ptr<Statement> parseVariantMatchStmt();
  std::optional<VariantMatchStmt::Cases> parseVariantMatchCases();
  std::optional<VariantMatchStmt::Case> parseVariantMatchCase();

  std::unique_ptr<Statement> parseIfStmt();
  IfStmt::Elifs parseElifs();
  std::optional<IfStmt::Elif> parseElif();
  std::optional<IfStmt::Else> parseElse();

  std::unique_ptr<Statement> parseForStmt();
  std::optional<ForStmt::Range> parseForRange();

  std::unique_ptr<Statement> parseWhileStmt();
  std::unique_ptr<Statement> parseContinueStmt();
  std::unique_ptr<Statement> parseBreakStmt();
  std::unique_ptr<Statement> parseReturnStmt();

 private:
  Lexer &m_lexer;
  ErrorHandler &m_errorHandler;

  Token m_token;

  std::unordered_map<TokenType, std::function<std::unique_ptr<Definition>()>> m_definitionParsers =
      {
          {TokenType::VAR_KWRD, [this] { return parseVarDef(); }},
          {TokenType::CONST_KWRD, [this] { return parseConstDef(); }},
          {TokenType::STRUCT_KWRD, [this] { return parseStructDef(); }},
          {TokenType::VARIANT_KWRD, [this] { return parseVariantDef(); }},
          {TokenType::FN_KWRD, [this] { return parseFnDef(); }},
  };

  std::unordered_map<TokenType, std::function<std::unique_ptr<Expression>()>> m_primaryExprParsers =
      {
          {TokenType::IDENTIFIER, [this] { return parseIdentifierExpr(); }},  // IdentifierExpr
          {TokenType::LBRACE, [this] { return parseObject(); }},              // Object
          {TokenType::LPAREN, [this] { return parseParenExpr(); }},           // ParenExpr
          {TokenType::INT_KWRD, [this] { return parseCastExpr(); }},          // CastExpr
          {TokenType::FLOAT_KWRD, [this] { return parseCastExpr(); }},
          {TokenType::BOOL_KWRD, [this] { return parseCastExpr(); }},
          {TokenType::CHAR_KWRD, [this] { return parseCastExpr(); }},
          {TokenType::STRING_KWRD, [this] { return parseCastExpr(); }},
          {TokenType::INTEGER, [this] { return parseLiteral<int>(); }},  // Literal
          {TokenType::FLOAT, [this] { return parseLiteral<float>(); }},
          {TokenType::BOOL, [this] { return parseLiteral<bool>(); }},
          {TokenType::CHAR, [this] { return parseLiteral<wchar_t>(); }},
          {TokenType::STRING, [this] { return parseLiteral<std::wstring>(); }},
  };

  std::unordered_map<TokenType, std::function<std::unique_ptr<FunctionalExpression::Postfix>()>>
      m_functionalExprPostfixParsers = {
          {TokenType::LPAREN, [this] { return parseFnCallPostfix(); }},          // FnCall
          {TokenType::DOT, [this] { return parseMemberAccessPostfix(); }},       // MemberAccess
          {TokenType::AS_KWRD, [this] { return parseVariantAccessPostfix(); }},  // VariantAccess
  };

  std::unordered_map<TokenType, std::function<std::unique_ptr<Statement>()>> m_statementParsers = {
      {TokenType::LBRACE, [this] { return parseBlockStmt(); }},
  };
};
