#include <functional>

#include "Parser.h"
#include "parser_utils.h"

Parser::Parser(Lexer& lexer, ErrorHandler& errorHandler)
    : m_lexer{lexer}, m_errorHandler{errorHandler} {
  consumeToken();
}

/* ----------------------------- Utility methods ---------------------------- */

void Parser::consumeToken() { m_token = m_lexer.getNextToken(); }

void Parser::skipError(const TokenType delimiter) {
  while (m_token.type != TokenType::ETX && m_token.type != delimiter) {
    consumeToken();
  }
  consumeToken();
}

bool Parser::extractAndConsume(Identifier& out) {
  try {
    out = m_token.representation;
    consumeToken();
    return true;
  } catch (std::exception& e) {
    m_errorHandler(ErrorType::INTERNAL_ERROR, m_token.position);
    return false;
  }
}

bool Parser::consumeIf(TokenType expectedType, ErrorType error) {
  if (m_token.type == expectedType) {
    consumeToken();
    return true;
  }
  m_errorHandler(error, m_token.position);
  return false;
}

bool Parser::consumeIf(const std::function<bool(TokenType tokenType)>& predicate, ErrorType err) {
  if (predicate(m_token.type)) {
    consumeToken();
    return true;
  }
  m_errorHandler(err, m_token.position);
  return false;
}

bool Parser::extractAndConsumeIf(Identifier& out,
                                 const std::function<bool(TokenType tokenType)>& predicate,
                                 ErrorType err) {
  if (predicate(m_token.type)) {
    out = m_token.representation;
    consumeToken();
    return true;
  }
  m_errorHandler(err, m_token.position);
  return false;
}

bool Parser::extractAndConsumeIf(Identifier& out, TokenType expectedType, ErrorType error) {
  if (m_token.type == expectedType) {
    out = m_token.representation;
    consumeToken();
    return true;
  }
  m_errorHandler(error, m_token.position);
  return false;
}

std::optional<Program> Parser::parseProgram() {
  auto position = m_token.position;

  Program::IdentifierToDefinitionPtrMap definitions;

  auto definition = parseDefinition();
  while (definition != nullptr) {
    // Redefinition
    if (definitions.find(definition->name) != definitions.end()) {
      m_errorHandler(ErrorType::REDEFINITION, definition->position);
    }
    // Success, add to map
    else {
      auto entry = std::make_pair(definition->name, std::move(definition));
      definitions.insert(std::move(entry));
    }

    definition = parseDefinition();
  }

  return Program{std::move(position), std::move(definitions)};
}

/* -------------------------------------------------------------------------- */
/*                                 Definitions                                */
/* -------------------------------------------------------------------------- */

std::unique_ptr<Definition> Parser::parseDefinition() {
  auto position = m_token.position;

  static const std::unordered_map<TokenType, std::function<std::unique_ptr<Definition>()>> map = {
      {TokenType::VAR_KWRD, [this]() { return parseVarDef(); }},
      {TokenType::CONST_KWRD, [this]() { return parseConstDef(); }},
      {TokenType::STRUCT_KWRD, [this]() { return parseStructDef(); }},
      {TokenType::VARIANT_KWRD, [this]() { return parseVariantDef(); }},
      {TokenType::FN_KWRD, [this]() { return parseFnDef(); }},
  };

  auto it = map.find(m_token.type);
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
  if (m_token.type != TokenType::VAR_KWRD) {
    return nullptr;
  }
  auto position = m_token.position;
  consumeToken();
  std::wstring name;
  std::wstring type;
  std::unique_ptr<Expression> expr;

  std::vector<std::function<bool()>> steps = {
      [this, &name]() {
        return extractAndConsumeIf(name, TokenType::IDENTIFIER,
                                   ErrorType::VARDEF_EXPECTED_IDENTIFIER);
      },
      [this]() { return consumeIf(TokenType::COLON, ErrorType::VARDEF_EXPECTED_COLON); },
      [this, &type]() {
        return extractAndConsumeIf(type, isTypeIdentifier,
                                   ErrorType::VARDEF_EXPECTED_TYPE_IDENTIFIER);
      },
      [this]() { return consumeIf(TokenType::ASSIGNMENT, ErrorType::VARDEF_EXPECTED_ASSIGNMENT); },
      [this, &expr]() {
        expr = parseExpression();
        return expr == nullptr ? false : true;
      },
      [this]() { return consumeIf(TokenType::SEMICOLON, ErrorType::VARDEF_EXPECTED_SEMICOLON); },
  };

  if (!std::all_of(steps.begin(), steps.end(), [](auto&& step) { return step(); })) {
    return nullptr;
  }

  return std::make_unique<VarDef>(std::move(position), std::move(name), std::move(type),
                                  std::move(expr));
}

/*
 * ConstDef
 *     = "const", identifier, ":", typeIdentifier, "=", expression, ";";
 */
std::unique_ptr<Definition> Parser::parseConstDef() {
  if (m_token.type != TokenType::CONST_KWRD) {
    return nullptr;
  }
  auto position = m_token.position;
  consumeToken();

  Identifier name;
  TypeIdentifier type;
  std::unique_ptr<Expression> expr;

  std::vector<std::function<bool()>> steps = {
      [this, &name]() {
        return extractAndConsumeIf(name, TokenType::IDENTIFIER,
                                   ErrorType::CONSTDEF_EXPECTED_IDENTIFIER);
      },
      [this]() { return consumeIf(TokenType::COLON, ErrorType::CONSTDEF_EXPECTED_COLON); },
      [this, &type]() {
        return extractAndConsumeIf(type, isTypeIdentifier,
                                   ErrorType::CONSTDEF_EXPECTED_TYPE_IDENTIFIER);
      },
      [this]() {
        return consumeIf(TokenType::ASSIGNMENT, ErrorType::CONSTDEF_EXPECTED_ASSIGNMENT);
      },
      [this, &expr]() { return (expr = parseExpression()) != nullptr; },
      [this]() { return consumeIf(TokenType::SEMICOLON, ErrorType::CONSTDEF_EXPECTED_SEMICOLON); },
  };

  if (!std::all_of(steps.begin(), steps.end(), [](auto&& step) { return step(); })) {
    return nullptr;
  }

  return std::make_unique<ConstDef>(std::move(position), std::move(name), std::move(type),
                                    std::move(expr));
}

/*
 * StructDef
 *     = "struct", identifier, "{", [ structMembers ], "}", ";";
 */
std::unique_ptr<Definition> Parser::parseStructDef() {
  if (m_token.type != TokenType::STRUCT_KWRD) {
    return nullptr;
  }
  auto position = m_token.position;
  consumeToken();

  Identifier name;
  StructDef::Members members;

  std::vector<std::function<bool()>> steps = {
      [this, &name]() {
        return extractAndConsumeIf(name, TokenType::IDENTIFIER,
                                   ErrorType::STRUCTDEF_EXPECTED_IDENTIFIER);
      },
      [this]() { return consumeIf(TokenType::LBRACE, ErrorType::STRUCTDEF_EXPECTED_LBRACE); },
      [this, &members]() {
        // If something goes wrong, we'll just end up with an empty members list
        members = parseStructMembers();
        return true;
      },
      [this]() { return consumeIf(TokenType::RBRACE, ErrorType::STRUCTDEF_EXPECTED_RBRACE); },
      [this]() { return consumeIf(TokenType::SEMICOLON, ErrorType::STRUCTDEF_EXPECTED_SEMICOLON); },
  };

  if (!std::all_of(steps.begin(), steps.end(), [](auto&& step) { return step(); })) {
    return nullptr;
  }

  return std::make_unique<StructDef>(std::move(position), std::move(name), std::move(members));
}

/*
 * structMembers
 *  = structMember, { structMember };
 */
StructDef::Members Parser::parseStructMembers() {
  StructDef::Members members{};

  auto member = parseStructMember();
  while (member != std::nullopt) {
    if (members.find(member->name) != members.end()) {
      m_errorHandler(ErrorType::STRUCTMEMBER_REDEFINITION, m_token.position);
    } else {
      members.insert(std::make_pair(member->name, std::move(*member)));
    }
    member = parseStructMember();
  }

  return members;
}

/* structMember
 *  = identifier, ":", typeIdentifier, ";";
 */
std::optional<StructDef::Member> Parser::parseStructMember() {
  if (m_token.type != TokenType::IDENTIFIER) {
    return std::nullopt;
  }

  auto position = m_token.position;

  Identifier name;
  TypeIdentifier type;

  std::vector<std::function<bool()>> steps = {
      [this, &name]() {
        name = m_token.representation;
        consumeToken();
        return true;
      },
      [this]() { return consumeIf(TokenType::COLON, ErrorType::STRUCTMEMBER_EXPECTED_COLON); },
      [this, &type]() {
        return extractAndConsumeIf(type, isTypeIdentifier,
                                   ErrorType::STRUCTMEMBER_EXPECTED_TYPE_IDENTIFIER);
      },
      [this]() {
        return consumeIf(TokenType::SEMICOLON, ErrorType::STRUCTMEMBER_EXPECTED_SEMICOLON);
      },
  };

  if (!std::all_of(steps.begin(), steps.end(), [](auto&& step) { return step(); })) {
    return std::nullopt;
  }

  return StructDef::Member{std::move(name), std::move(type)};
}

/*
 *VariantDef
 *     = "variant", identifier, "{", [ variantTypes ], "}", ";";
 */
std::unique_ptr<Definition> Parser::parseVariantDef() {
  if (m_token.type != TokenType::VARIANT_KWRD) {
    return nullptr;
  }
  auto position = m_token.position;
  consumeToken();

  Identifier name;
  VariantDef::Types types;

  std::vector<std::function<bool()>> steps = {
      [this, &name]() {
        return extractAndConsumeIf(name, TokenType::IDENTIFIER,
                                   ErrorType::VARIANTDEF_EXPECTED_IDENTIFIER);
      },
      [this]() { return consumeIf(TokenType::LBRACE, ErrorType::VARIANTDEF_EXPECTED_LBRACE); },
      [this, &types]() {
        // If something goes wrong, we'll just end up with an empty types list
        types = parseVariantTypes();
        return true;
      },
      [this]() { return consumeIf(TokenType::RBRACE, ErrorType::VARIANTDEF_EXPECTED_RBRACE); },
      [this]() {
        return consumeIf(TokenType::SEMICOLON, ErrorType::VARIANTDEF_EXPECTED_SEMICOLON);
      },
  };

  if (!std::all_of(steps.begin(), steps.end(), [](auto&& step) { return step(); })) {
    return nullptr;
  }

  return std::make_unique<VariantDef>(std::move(position), std::move(name), std::move(types));
}

/*
 * variantTypes
 *     = variantType, { ",", variantType };
 */
VariantDef::Types Parser::parseVariantTypes() {
  VariantDef::Types types{};

  auto type = parseVariantType();
  if (type == std::nullopt) return types;
  types.push_back(std::move(*type));

  while (m_token.type == TokenType::COMMA) {
    consumeToken();
    type = parseVariantType();
    if (type == std::nullopt) return types;
    types.push_back(std::move(*type));
  }

  return types;
}

/*
 * variantType
 *     = typeIdentifier
 */
std::optional<VariantDef::Type> Parser::parseVariantType() {
  if (!isTypeIdentifier(m_token.type)) {
    return std::nullopt;
  }
  TypeIdentifier type;
  extractAndConsume(type);
  return type;
}

/*
 * FnDef
 *    = "fn", identifier, "(", [ fnParams ], ")", returnTypeAnnotation, Block;
 */
std::unique_ptr<Definition> Parser::parseFnDef() {
  if (m_token.type != TokenType::FN_KWRD) {
    return nullptr;
  }
  consumeToken();
  auto position = m_token.position;

  Identifier name;
  FnDef::Params params;
  std::optional<FnDef::ReturnType> returnType;
  std::optional<Block> body;

  std::vector<std::function<bool()>> steps = {
      [this, &name]() {
        return extractAndConsumeIf(name, TokenType::IDENTIFIER,
                                   ErrorType::FNDEF_EXPECTED_IDENTIFIER);
      },
      [this]() { return consumeIf(TokenType::LPAREN, ErrorType::FNDEF_EXPECTED_LPAREN); },
      [this, &params]() {
        params = parseFnParams();
        return true;
      },
      [this]() { return consumeIf(TokenType::RPAREN, ErrorType::FNDEF_EXPECTED_RPAREN); },
      [this, &returnType]() {
        auto result = parseFnReturnType();
        if (result == std::nullopt) {
          m_errorHandler(ErrorType::FNDEF_EXPECTED_RETURN_TYPE, m_token.position);
          return false;
        }
        returnType = std::move(*result);
        return true;
      },
      [this, &body]() {
        auto result = parseBlock();
        if (result == std::nullopt) {
          m_errorHandler(ErrorType::FNDEF_EXPECTED_BLOCK, m_token.position);
          return false;
        }
        body = std::move(*result);
        return true;
      },
  };

  if (!std::all_of(steps.begin(), steps.end(), [](auto&& step) { return step(); })) {
    return nullptr;
  }

  return std::make_unique<FnDef>(std::move(position), std::move(name), std::move(params),
                                 std::move(*returnType), std::move(*body));
}

/*
 * fnParams
 *    = fnParam, { ",", fnParam };
 */
FnDef::Params Parser::parseFnParams() {
  FnDef::Params params{};

  auto param = parseFnParam();
  if (param == std::nullopt) return params;

  params.push_back(std::move(*param));
  while (m_token.type == TokenType::COMMA) {
    consumeToken();
    param = parseFnParam();
    if (param == std::nullopt) return params;
    params.push_back(std::move(*param));
  }

  return params;
}

/*
 * fnParam
 *    = [ "const" ], identifier, ":", typeIdentifier;
 */
std::optional<FnDef::Param> Parser::parseFnParam() {
  if (m_token.type != TokenType::CONST_KWRD && m_token.type != TokenType::IDENTIFIER) {
    return std::nullopt;
  }

  bool isConst = false;
  Identifier name;
  TypeIdentifier type;

  std::vector<std::function<bool()>> steps = {
      [this, &isConst]() {
        if (m_token.type == TokenType::CONST_KWRD) {
          isConst = true;
          consumeToken();
        }
        return true;
      },
      [this, &name]() {
        return extractAndConsumeIf(name, TokenType::IDENTIFIER,
                                   ErrorType::FNPARAM_EXPECTED_IDENTIFIER);
      },
      [this]() { return consumeIf(TokenType::COLON, ErrorType::FNPARAM_EXPECTED_COLON); },
      [this, &type]() {
        return extractAndConsumeIf(type, isTypeIdentifier,
                                   ErrorType::FNPARAM_EXPECTED_TYPE_IDENTIFIER);
      },
  };

  if (!std::all_of(steps.begin(), steps.end(), [](auto&& step) { return step(); })) {
    return std::nullopt;
  }

  return FnDef::Param{isConst, std::move(name), std::move(type)};
}

/*
 * fnReturnType
 *    = "->", typeIdentifier;
 */
std::optional<FnDef::ReturnType> Parser::parseFnReturnType() {
  if (m_token.type != TokenType::ARROW) {
    return std::nullopt;
  }
  consumeToken();
  TypeIdentifier type;
  if (!extractAndConsumeIf(type, isTypeIdentifier,
                           ErrorType::FNRETURNTYPE_EXPECTED_TYPE_IDENTIFIER)) {
    return std::nullopt;
  };
  return type;
}
