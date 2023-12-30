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
#include "Program.h"
#include "Statement.h"
#include "parser_utils.h"

class Parser {
 public:
  Parser() = delete;
  Parser(const Parser &) = delete;
  Parser(Parser &&) = delete;
  Parser &operator=(const Parser &) = delete;
  Parser &operator=(Parser &&) = delete;
  ~Parser() = default;

  Parser(Lexer &lexer, ErrorHandler &errorHandler);

  std::optional<Program> parseProgram();

 private:
  friend class ParserTest;

  void consumeToken();
  bool consumeIf(TokenType expectedType, ErrorType error);

  /* ASTNodes are returned by pointers, convenience methods return optional */

  // Identifiers
  std::optional<Identifier> parseIdentifier();
  std::optional<TypeIdentifier> parseTypeIdentifier();

  // Definitions
  std::unique_ptr<Definition> parseDefinition();
  std::unique_ptr<Definition> parseVarDef();
  std::unique_ptr<Definition> parseConstDef();
  std::unique_ptr<Definition> parseStructDef();
  std::optional<StructDef::Members> parseStructMembers();
  std::unique_ptr<StructMember> parseStructMember();
  std::unique_ptr<Definition> parseVariantDef();
  std::optional<VariantDef::Types> parseVariantTypes();
  std::unique_ptr<Definition> parseFnDef();
  std::optional<FnDef::Params> parseFnParams();
  std::unique_ptr<FnParam> parseFnParam();

  // Expressions
  std::unique_ptr<Expression> parseExpression();
  std::unique_ptr<Expression> parseBinaryExpression(
      std::unique_ptr<Expression> (Parser::*parseSubExpr)(), std::vector<TokenType> &&operators);
  std::unique_ptr<Expression> parseUnaryExpression(
      std::unique_ptr<Expression> (Parser::*parseSubExpr)(), std::vector<TokenType> &&operators);
  std::unique_ptr<Expression> parseFunctionalExpression();
  std::unique_ptr<FunctionalPostfix> parseFunctionalExpressionPostfix();
  std::unique_ptr<FunctionalPostfix> parseMemberAccessPostfix();
  std::unique_ptr<FunctionalPostfix> parseVariantAccessPostfix();
  std::unique_ptr<FunctionalPostfix> parseFnCallPostfix();
  std::optional<FnCallArgs> parseFnCallArgs();
  std::unique_ptr<Expression> parseLogicOrExpr();
  std::unique_ptr<Expression> parseLogicAndExpr();
  std::unique_ptr<Expression> parseEqualityExpr();
  std::unique_ptr<Expression> parseRelationalExpr();
  std::unique_ptr<Expression> parseAdditiveExpr();
  std::unique_ptr<Expression> parseMultiplicativeExpr();
  std::unique_ptr<Expression> parseUnaryOpExpr();
  std::unique_ptr<Expression> parsePrimaryExpression();
  std::unique_ptr<Expression> parseIdentifierExpr();
  std::unique_ptr<Expression> parseObject();
  std::optional<Object::Members> parseObjectMembers();
  std::unique_ptr<ObjectMember> parseObjectMember();
  std::unique_ptr<Expression> parseParenExpr();
  std::unique_ptr<Expression> parseCastExpr();
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
      m_errorHandler(ErrorType::TOKEN_INVARIANT_VIOLATION, position);
      return nullptr;
    }

    consumeToken();
    return std::make_unique<Literal<T>>(std::move(position), std::move(value));
  }

  // Statements
  std::unique_ptr<Statement> parseStatement();
  std::unique_ptr<Statement> parseBlockStmt();
  std::unique_ptr<Statement> parseExpressionOrAssignmentStmt();
  std::unique_ptr<Statement> parseStdinExtractionStmt();
  std::unique_ptr<Statement> parseStdoutInsertionStmt();
  std::unique_ptr<Statement> parseVariantMatchStmt();
  std::optional<VariantMatchStmt::Cases> parseVariantMatchCases();
  std::unique_ptr<VariantMatchCase> parseVariantMatchCase();
  std::unique_ptr<Statement> parseIfStmt();
  std::optional<IfStmt::Elifs> parseElifs();
  std::unique_ptr<Elif> parseElif();
  std::unique_ptr<Else> parseElse();
  std::unique_ptr<Statement> parseForStmt();
  std::unique_ptr<Range> parseRange();
  std::unique_ptr<Statement> parseWhileStmt();
  std::unique_ptr<Statement> parseContinueStmt();
  std::unique_ptr<Statement> parseBreakStmt();
  std::unique_ptr<Statement> parseReturnStmt();

 private:
  void initParserMaps();

  Lexer &m_lexer;
  ErrorHandler &m_errorHandler;
  Token m_token;

  std::unordered_map<TokenType, std::function<std::unique_ptr<Definition>()>> m_definitionParsers;
  std::unordered_map<TokenType, std::function<std::unique_ptr<Expression>()>> m_primaryExprParsers;
  std::unordered_map<TokenType, std::function<std::unique_ptr<FunctionalPostfix>()>>
      m_functionalPostfixParsers;
  std::unordered_map<TokenType, std::function<std::unique_ptr<Statement>()>> m_statementParsers;
  std::unordered_map<TokenType, Operator> m_tokenTypeToOperator;
  std::unordered_map<TokenType, PrimitiveType> m_tokenTypeToPrimitiveType;
};
