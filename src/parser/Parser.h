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
  bool extractAndConsume(Identifier &out);
  bool extractAndConsumeIf(Identifier &out,
                           const std::function<bool(TokenType tokenType)> &predicate,
                           ErrorType err);
  bool extractAndConsumeIf(Identifier &out, TokenType expectedType, ErrorType error);

  /* ------------------------------- Identifier ------------------------------- */
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
  std::unique_ptr<Expression> parseExpression() {
    auto position = m_token.position;
    consumeToken();
    return std::make_unique<Expression>(std::move(position));
  }

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
};
