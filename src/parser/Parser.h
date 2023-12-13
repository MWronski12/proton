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
  bool expect(std::function<bool(const Token &token)> predicate, ErrorType err);
  bool expect(std::wstring &out, std::function<bool(const Token &token)> predicate, ErrorType err);
  bool expect(TokenType expectedType, ErrorType error);
  bool expect(std::wstring &out, TokenType expectedType, ErrorType error);

  /* ------------------------------- Definition ------------------------------- */

  std::unique_ptr<Definition> parseDefinition();
  std::unique_ptr<Definition> parseVarDef();
  std::unique_ptr<Definition> parseConstDef();

  std::unique_ptr<Definition> parseStructDef();
  StructDef::Members parseStructMembers();
  std::optional<StructDef::Member> parseStructMember();

  std::unique_ptr<Definition> parseVariantDef();
  std::optional<VariantDef::Types> parseVariantTypes();

  std::unique_ptr<Definition> parseFnDef();
  std::optional<FnDef::Params> parseFnParams();
  std::optional<FnDef::Param> parseFnParam();
  std::optional<FnDef::ReturnType> parseFnReturnType();

  /* ------------------------------- Expression ------------------------------- */
  std::unique_ptr<Expression> parseExpression() {
    auto position = m_token.position;
    consumeToken();
    return std::make_unique<Expression>(position);
  }

  /* --------------------------------- Block ---------------------------------- */
  std::optional<Block> parseBlock() { return std::nullopt; }

 private:
  Lexer &m_lexer;
  ErrorHandler &m_errorHandler;

  Token m_token;
};
