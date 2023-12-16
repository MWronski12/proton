#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <type_traits>
#include <variant>

#include "Block.h"
#include "Definition.h"
#include "ErrorHandler.h"
#include "Expression.h"
#include "Lexer.h"
#include "Program.h"
#include "parser_utils.h"

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

  std::optional<Identifier> getIdentifier();
  std::optional<TypeIdentifier> getTypeIdentifier();

  /* ------------------------------- Definition ------------------------------- */

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

  /* ------------------------------- Expression ------------------------------- */
  std::unique_ptr<Expression> parseExpression();

  std::unique_ptr<Expression> parsePrimaryExpr();
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

  /* --------------------------------- Block ---------------------------------- */
  std::optional<Block> parseBlock() {
    if (m_token.type != TokenType::LBRACE) {
      return std::nullopt;
    }
    auto position = m_token.position;
    consumeToken();

    while (m_token.type != TokenType::RBRACE) {
      consumeToken();
    }
    consumeToken();

    return Block{std::move(position)};
  }

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
};
