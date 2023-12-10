#include <functional>

#include "Parser.h"

Parser::Parser(Lexer& lexer, ErrorHandlerBase& errorHandler)
    : m_lexer{lexer}, m_errorHandler{errorHandler}, m_token{m_lexer.getNextToken()} {}

void Parser::consumeToken() { m_lexer.getNextToken(); }

/*
 * @brief Recover from error by skipping tokens until semicolon or end of text. After calling
 * skipError m_token is set to first token of the next statement or end of text.
 */
void Parser::skipError() {
  do {
    consumeToken();
  } while (m_token.type != TokenType::SEMICOLON && m_token.type != TokenType::ETX);

  if (m_token.type == TokenType::SEMICOLON) {
    consumeToken();
  }
}

/*
 * @brief Utility function to check if current token is of expected type. If not, error is
 * signaled to errorHandler.
 *
 * @return true if current token is of expected type, false otherwise.
 */
bool Parser::expect(TokenType expectedToken, ErrorType error, const Position& position) {
  if (m_token.type != expectedToken) {
    m_errorHandler(error, position, m_lexer.getInputFilename());
    return false;
  }

  return true;
}

/* --------------------------------- Program -------------------------------- */

std::optional<Program> Parser::parseProgram() {
  Program::IdentifierToDefinitionPtrMap definitions;

  while (m_token.type != TokenType::ETX) {
    auto definition = parseDefinition();

    // Failed to parse definition
    if (definition == nullptr) {
      skipError();
    }
    // Definition already exists
    else if (definitions.find(definition->name) != definitions.end()) {
      m_errorHandler(ErrorType::FUNCTION_REDEFINITION, m_token.position,
                     m_lexer.getInputFilename());
      skipError();
    }
    // Success, add to map
    else {
      auto entry = std::make_pair(definition->name, std::move(definition));
      definitions.insert(std::move(entry));
    }
  }

  m_errorHandler.exitIfErrors();

  return Program{std::move(definitions)};
}

/* ------------------------------- Definition ------------------------------- */
std::unique_ptr<Definition> Parser::parseDefinition() {
  auto position = m_token.position;

  static const std::unordered_map<Identifier, std::function<std::unique_ptr<Definition>()>> map = {
      {std::wstring(L"var"), [this]() { return parseVarDef(); }},
      {std::wstring(L"const"), [this]() { return parseConstDef(); }},
      {std::wstring(L"struct"), [this]() { return parseStructDef(); }},
      {std::wstring(L"variant"), [this]() { return parseVariantDef(); }},
      {std::wstring(L"fn"), [this]() { return parseFnDef(); }},
  };

  auto it = map.find(m_token.value);
  if (it == map.end()) {
    m_errorHandler(ErrorType::EXPECTED_DEFINITION, position, m_lexer.getInputFilename());
    skipError();
    return nullptr;
  }

  return std::move(it->second());
}

/*
 * VarDef
 *     = "var", identifier, ":", typeIdentifier, "=", expression, ";";
 */
std::unique_ptr<Definition> Parser::parseVarDef() {
  auto position = m_token.position;

  if (m_token.type != TokenType::VAR_KWRD ||
      m_token.value != KEYWORDS.at(int(TokenType::VAR_KWRD) - KEYWORDS_OFFSET)) {
    throw std::logic_error("Parser::parseVarDef() called when m_token is not var keyword");
  }

  consumeToken();
  if (!expect(TokenType::SEMICOLON, ErrorType::VARDEF_MISSING_SEMICOLON, position)) {
    skipError();
    return nullptr;
  }

  consumeToken();

  return std::make_unique<VarDef>(L"foo", TypeIdentifier::String, std::make_unique<Expression>());
}

std::unique_ptr<Definition> Parser::parseConstDef() { return nullptr; }

std::unique_ptr<Definition> Parser::parseStructDef() { return nullptr; }
std::optional<StructDef::Members> Parser::parseStructMembers() { return std::nullopt; }
std::optional<StructDef::Member> Parser::parseStructMember() { return std::nullopt; }

std::unique_ptr<Definition> Parser::parseVariantDef() { return nullptr; }
std::optional<VariantDef::Types> Parser::parseVariantTypes() { return std::nullopt; }

std::unique_ptr<Definition> Parser::parseFnDef() { return nullptr; }
std::optional<FnDef::Params> Parser::parseFnParams() { return std::nullopt; }
std::optional<FnDef::Param> Parser::parseFnParam() { return std::nullopt; }
std::optional<FnDef::ReturnType> Parser::parseFnReturnType() { return std::nullopt; }
