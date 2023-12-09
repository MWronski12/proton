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

  void consumeToken();
  void skipSyntaxError();

  /* --------------------------------- Program -------------------------------- */
  std::optional<Program> parseProgram();

  /* ------------------------------- Definition ------------------------------- */

  std::optional<std::unique_ptr<Definition>> parseDefinition() { return std::nullopt; }

  std::optional<VarDef> parseVarDef() { return std::nullopt; }

  std::optional<ConstDef> parseConstDef() { return std::nullopt; }

  std::optional<StructDef> parseStructDef() { return std::nullopt; }
  std::optional<StructDef::Members> parseStructMembers() { return std::nullopt; }
  std::optional<StructDef::Member> parseStructMember() { return std::nullopt; }

  std::optional<VariantDef> parseVariantDef() { return std::nullopt; }
  std::optional<VariantDef::Types> parseVariantTypes() { return std::nullopt; }
  std::optional<VariantDef::Type> parseVariantType() { return std::nullopt; }

  std::optional<FnDef> parseFnDef() { return std::nullopt; }
  std::optional<FnDef::Params> parseFnParams() { return std::nullopt; }
  std::optional<FnDef::Param> parseFnParam() { return std::nullopt; }
  std::optional<FnDef::ReturnType> parseFnReturnType() { return std::nullopt; }

  /* ------------------------------- Expression ------------------------------- */
  std::optional<std::unique_ptr<Expression>> parseExpression() { return std::nullopt; }

  /* --------------------------------- Block ---------------------------------- */
  std::optional<Block> parseBlock() { return std::nullopt; }

 private:
  Lexer& m_lexer;
  ErrorHandlerBase& m_errorHandler;

  Token m_token;
};
