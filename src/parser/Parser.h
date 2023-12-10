#pragma once

#include <memory>
#include <optional>

#include "Block.h"
#include "Definition.h"
#include "ErrorHandlerBase.h"
#include "Expression.h"
#include "Lexer.h"
#include "Program.h"

class Parser {
 public:
  Parser(Lexer& lexer, ErrorHandlerBase& errorHandler);
  std::optional<Program> parseProgram();

 private:
  void consumeToken();
  void skipError();
  bool expect(TokenType expectedToken, ErrorType error, const Position& position);

  /* --------------------------------- Program -------------------------------- */

  /* ------------------------------- Definition ------------------------------- */

  std::unique_ptr<Definition> parseDefinition();

  std::unique_ptr<Definition> parseVarDef();

  std::unique_ptr<Definition> parseConstDef();

  std::unique_ptr<Definition> parseStructDef();
  std::optional<StructDef::Members> parseStructMembers();
  std::optional<StructDef::Member> parseStructMember();

  std::unique_ptr<Definition> parseVariantDef();
  std::optional<VariantDef::Types> parseVariantTypes();

  std::unique_ptr<Definition> parseFnDef();
  std::optional<FnDef::Params> parseFnParams();
  std::optional<FnDef::Param> parseFnParam();
  std::optional<FnDef::ReturnType> parseFnReturnType();

  /* ------------------------------- Expression ------------------------------- */
  std::optional<std::unique_ptr<Expression>> parseExpression() { return std::nullopt; }

  /* --------------------------------- Block ---------------------------------- */
  std::optional<Block> parseBlock() { return std::nullopt; }

 private:
  Lexer& m_lexer;
  ErrorHandlerBase& m_errorHandler;

  Token& m_token;
};
