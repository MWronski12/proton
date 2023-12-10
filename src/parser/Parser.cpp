#include <functional>

#include "Parser.h"
#include "parser_utils.h"

Parser::Parser(Lexer& lexer, ErrorHandlerBase& errorHandler)
    : m_lexer{lexer}, m_errorHandler{errorHandler} {}

void Parser::consumeToken() { m_token = m_lexer.getNextToken(); }

/*
 * @brief Utility function to check if current token is of expected type. If not, error is
 * signaled to errorHandler.
 *
 * @return true if current token is of expected type, false otherwise.
 */
bool Parser::expect(TokenType expectedToken, ErrorType error) {
  if (m_token.type != expectedToken) {
    m_errorHandler(error, m_token.position);
    return false;
  }

  return true;
}

bool Parser::consumeAndExpect(TokenType expectedToken, ErrorType error) {
  consumeToken();
  if (m_token.type != expectedToken) {
    m_errorHandler(error, m_token.position);
    return false;
  }
  return true;
}

bool Parser::consumeAndExpect(std::function<bool(const Token& token)> predicate, ErrorType error) {
  consumeToken();
  if (predicate(m_token) == false) {
    m_errorHandler(error, m_token.position);
    return false;
  }
  return true;
}

std::optional<Program> Parser::parseProgram() {
  consumeToken();
  auto position = m_token.position;

  Program::IdentifierToDefinitionPtrMap definitions;
  while (m_token.type != TokenType::ETX) {
    auto definition = parseDefinition();

    // Parsing failed
    if (definition == nullptr) {
      // TODO: recover from error by skipping to start of the next definition instead of exiting
      m_errorHandler.exitIfErrors();
      return std::nullopt;
    }
    // Redefinition
    else if (definitions.find(definition->name) != definitions.end()) {
      m_errorHandler(ErrorType::FUNCTION_REDEFINITION, m_token.position);
      continue;
    }
    // Success, add to map
    else {
      auto entry = std::make_pair(definition->name, std::move(definition));
      definitions.insert(std::move(entry));
    }
  }

  m_errorHandler.exitIfErrors();

  return Program{position, std::move(definitions)};
}

/* -------------------------------------------------------------------------- */
/*                                 Definitions                                */
/* -------------------------------------------------------------------------- */

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

  Identifier name;
  TypeIdentifier type;
  std::unique_ptr<Expression> expr;

  std::vector<std::function<bool()>> steps = {
      // identifier
      [this, &name]() {
        if (consumeAndExpect(TokenType::IDENTIFIER, ErrorType::VARDEF_EXPECTED_IDENTIFIER)) {
          name = getIdentifier(m_token);
          return true;
        }
        return false;
      },
      // colon
      [this]() { return consumeAndExpect(TokenType::COLON, ErrorType::VARDEF_EXPECTED_COLON); },
      // typeIdentifier
      [this, &type]() {
        if (consumeAndExpect(isTypeIdentifier, ErrorType::VARDEF_EXPECTED_TYPE_IDENTIFIER)) {
          type = getTypeIdentifier(m_token);
          return true;
        }
        return false;
      },
      // assignment
      [this]() {
        return consumeAndExpect(TokenType::ASSIGNMENT, ErrorType::VARDEF_EXPECTED_ASSIGNMENT);
      },
      // expression
      [this, &expr]() {
        expr = parseExpression();
        return expr == nullptr ? false : true;
      },
      // semicolon
      [this]() {
        if (consumeAndExpect(TokenType::SEMICOLON, ErrorType::VARDEF_EXPECTED_SEMICOLON)) {
          consumeToken();
          return true;
        };
        return false;
      },
  };

  if (!std::all_of(steps.begin(), steps.end(), [](auto step) { return step(); })) {
    m_errorHandler(ErrorType::ERROR_PARSING_VARDEF, position);
    return nullptr;
  }

  return std::make_unique<VarDef>(position, std::move(name), std::move(type), std::move(expr));
}

/*
 * ConstDef
 *     = "const", identifier, ":", typeIdentifier, "=", expression, ";";
 */
std::unique_ptr<Definition> Parser::parseConstDef() {
  auto position = m_token.position;

  if (m_token.type != TokenType::CONST_KWRD ||
      m_token.value != KEYWORDS.at(int(TokenType::CONST_KWRD) - KEYWORDS_OFFSET)) {
    throw std::logic_error("Parser::parseVarDef() called when m_token is not const keyword");
  }

  Identifier name;
  TypeIdentifier type;
  std::unique_ptr<Expression> expr;

  std::vector<std::function<bool()>> steps = {
      // identifier
      [this, &name]() {
        if (consumeAndExpect(TokenType::IDENTIFIER, ErrorType::CONSTDEF_EXPECTED_IDENTIFIER)) {
          name = getIdentifier(m_token);
          return true;
        }
        return false;
      },
      // colon
      [this]() { return consumeAndExpect(TokenType::COLON, ErrorType::CONSTDEF_EXPECTED_COLON); },
      // typeIdentifier
      [this, &type]() {
        if (consumeAndExpect(isTypeIdentifier, ErrorType::CONSTDEF_EXPECTED_TYPE_IDENTIFIER)) {
          type = getTypeIdentifier(m_token);
          return true;
        }
        return false;
      },
      // assignment
      [this]() {
        return consumeAndExpect(TokenType::ASSIGNMENT, ErrorType::CONSTDEF_EXPECTED_ASSIGNMENT);
      },
      // expression
      [this, &expr]() {
        expr = parseExpression();
        return expr == nullptr ? false : true;
      },
      // semicolon
      [this]() {
        if (consumeAndExpect(TokenType::SEMICOLON, ErrorType::CONSTDEF_EXPECTED_SEMICOLON)) {
          consumeToken();
          return true;
        };
        return false;
      },
  };

  if (!std::all_of(steps.begin(), steps.end(), [](auto step) { return step(); })) {
    m_errorHandler(ErrorType::ERROR_PARSING_CONSTDEF, position);
    return nullptr;
  }

  consumeToken();

  return std::make_unique<ConstDef>(position, std::move(name), std::move(type), std::move(expr));
}

std::unique_ptr<Definition> Parser::parseStructDef() { return nullptr; }
std::optional<StructDef::Members> Parser::parseStructMembers() { return std::nullopt; }
std::optional<StructDef::Member> Parser::parseStructMember() { return std::nullopt; }

std::unique_ptr<Definition> Parser::parseVariantDef() { return nullptr; }
std::optional<VariantDef::Types> Parser::parseVariantTypes() { return std::nullopt; }

std::unique_ptr<Definition> Parser::parseFnDef() { return nullptr; }
std::optional<FnDef::Params> Parser::parseFnParams() { return std::nullopt; }
std::optional<FnDef::Param> Parser::parseFnParam() { return std::nullopt; }
std::optional<FnDef::ReturnType> Parser::parseFnReturnType() { return std::nullopt; }
