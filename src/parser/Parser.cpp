#include <cassert>

#include "Parser.h"

Parser::Parser(Lexer& lexer, ErrorHandler& errorHandler)
    : m_lexer{lexer}, m_errorHandler{errorHandler} {
  consumeToken();
}

/* -------------------------------------------------------------------------- */
/*                                   Program                                  */
/* -------------------------------------------------------------------------- */

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
/*                               Utility Methods                              */
/* -------------------------------------------------------------------------- */

void Parser::consumeToken() { m_token = m_lexer.getNextToken(); }

void Parser::skipError(const TokenType delimiter) {
  while (m_token.type != TokenType::ETX && m_token.type != delimiter) {
    consumeToken();
  }
  consumeToken();
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

std::optional<Identifier> Parser::parseIdentifier() {
  if (m_token.type != TokenType::IDENTIFIER) {
    return std::nullopt;
  }
  Identifier identifier = m_token.representation;
  consumeToken();
  return identifier;
}

std::optional<TypeIdentifier> Parser::parseTypeIdentifier() {
  if (!isTypeIdentifier(m_token.type)) {
    return std::nullopt;
  }
  TypeIdentifier identifier = m_token.representation;
  consumeToken();
  return identifier;
}

/* -------------------------------------------------------------------------- */
/*                                 Definitions                                */
/* -------------------------------------------------------------------------- */

std::unique_ptr<Definition> Parser::parseDefinition() {
  auto it = m_definitionParsers.find(m_token.type);
  if (it == m_definitionParsers.end()) {
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

  std::optional<Identifier> name;
  std::optional<TypeIdentifier> type;
  std::unique_ptr<Expression> expr;

  std::vector<std::function<bool()>> steps = {
      [this, &name]() {
        if ((name = parseIdentifier()) == std::nullopt) {
          m_errorHandler(ErrorType::VARDEF_EXPECTED_IDENTIFIER, m_token.position);
          return false;
        }
        return true;
      },
      [this]() { return consumeIf(TokenType::COLON, ErrorType::VARDEF_EXPECTED_COLON); },
      [this, &type]() {
        if ((type = parseTypeIdentifier()) == std::nullopt) {
          m_errorHandler(ErrorType::VARDEF_EXPECTED_TYPE_IDENTIFIER, m_token.position);
          return false;
        }
        return true;
      },
      [this]() { return consumeIf(TokenType::ASSIGNMENT, ErrorType::VARDEF_EXPECTED_ASSIGNMENT); },
      [this, &expr]() {
        if ((expr = parseExpression()) == nullptr) {
          m_errorHandler(ErrorType::VARDEF_EXPECTED_EXPRESSION, m_token.position);
          return false;
        }
        return true;
      },
      [this]() { return consumeIf(TokenType::SEMICOLON, ErrorType::VARDEF_EXPECTED_SEMICOLON); },
  };

  if (!std::all_of(steps.begin(), steps.end(),
                   [](const std::function<bool()>& step) { return step(); })) {
    return nullptr;
  }

  return std::make_unique<VarDef>(std::move(position), std::move(*name), std::move(*type),
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

  std::optional<Identifier> name;
  std::optional<TypeIdentifier> type;
  std::unique_ptr<Expression> expr;

  std::vector<std::function<bool()>> steps = {
      [this, &name]() {
        if ((name = parseIdentifier()) == std::nullopt) {
          m_errorHandler(ErrorType::CONSTDEF_EXPECTED_IDENTIFIER, m_token.position);
          return false;
        }
        return true;
      },
      [this]() { return consumeIf(TokenType::COLON, ErrorType::CONSTDEF_EXPECTED_COLON); },
      [this, &type]() {
        if ((type = parseTypeIdentifier()) == std::nullopt) {
          m_errorHandler(ErrorType::CONSTDEF_EXPECTED_TYPE_IDENTIFIER, m_token.position);
          return false;
        }
        return true;
      },
      [this]() {
        return consumeIf(TokenType::ASSIGNMENT, ErrorType::CONSTDEF_EXPECTED_ASSIGNMENT);
      },
      [this, &expr]() {
        if ((expr = parseExpression()) == nullptr) {
          m_errorHandler(ErrorType::CONSTDEF_EXPECTED_EXPRESSION, m_token.position);
          return false;
        }
        return true;
      },
      [this]() { return consumeIf(TokenType::SEMICOLON, ErrorType::CONSTDEF_EXPECTED_SEMICOLON); },
  };

  if (!std::all_of(steps.begin(), steps.end(),
                   [](const std::function<bool()>& step) { return step(); })) {
    return nullptr;
  }

  return std::make_unique<ConstDef>(std::move(position), std::move(*name), std::move(*type),
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

  std::optional<Identifier> name;
  std::optional<StructDef::Members> members;

  std::vector<std::function<bool()>> steps = {
      [this, &name]() {
        if ((name = parseIdentifier()) == std::nullopt) {
          m_errorHandler(ErrorType::STRUCTDEF_EXPECTED_IDENTIFIER, m_token.position);
          return false;
        }
        return true;
      },
      [this]() { return consumeIf(TokenType::LBRACE, ErrorType::STRUCTDEF_EXPECTED_LBRACE); },
      [this, &members]() { return ((members = parseStructMembers()) != std::nullopt); },
      [this]() { return consumeIf(TokenType::RBRACE, ErrorType::STRUCTDEF_EXPECTED_RBRACE); },
      [this]() { return consumeIf(TokenType::SEMICOLON, ErrorType::STRUCTDEF_EXPECTED_SEMICOLON); },
  };

  if (!std::all_of(steps.begin(), steps.end(),
                   [](const std::function<bool()>& step) { return step(); })) {
    return nullptr;
  }

  return std::make_unique<StructDef>(std::move(position), std::move(*name), std::move(*members));
}

/*
 * structMembers
 *  = structMember, { structMember };
 */
std::optional<StructDef::Members> Parser::parseStructMembers() {
  StructDef::Members members{};

  auto member = parseStructMember();
  while (member != std::nullopt) {
    if (members.find(member->name) != members.end()) {
      m_errorHandler(ErrorType::STRUCTMEMBER_REDEFINITION, m_token.position);
      return std::nullopt;
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

  std::optional<Identifier> name;
  std::optional<TypeIdentifier> type;

  std::vector<std::function<bool()>> steps = {
      [this, &name]() {
        if ((name = parseIdentifier()) == std::nullopt) {
          return false;
        }
        return true;
      },
      [this]() { return consumeIf(TokenType::COLON, ErrorType::STRUCTMEMBER_EXPECTED_COLON); },
      [this, &type]() {
        if ((type = parseTypeIdentifier()) == std::nullopt) {
          m_errorHandler(ErrorType::STRUCTMEMBER_EXPECTED_TYPE_IDENTIFIER, m_token.position);
          return false;
        }
        return true;
      },
      [this]() {
        return consumeIf(TokenType::SEMICOLON, ErrorType::STRUCTMEMBER_EXPECTED_SEMICOLON);
      },
  };

  if (!std::all_of(steps.begin(), steps.end(),
                   [](const std::function<bool()>& step) { return step(); })) {
    return std::nullopt;
  }

  return StructDef::Member{std::move(*name), std::move(*type)};
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

  std::optional<Identifier> name;
  std::optional<VariantDef::Types> types;

  std::vector<std::function<bool()>> steps = {
      [this, &name]() {
        if ((name = parseIdentifier()) == std::nullopt) {
          m_errorHandler(ErrorType::VARIANTDEF_EXPECTED_IDENTIFIER, m_token.position);
          return false;
        }
        return true;
      },
      [this]() { return consumeIf(TokenType::LBRACE, ErrorType::VARIANTDEF_EXPECTED_LBRACE); },
      [this, &types]() { return ((types = parseVariantTypes()) != std::nullopt); },
      [this]() { return consumeIf(TokenType::RBRACE, ErrorType::VARIANTDEF_EXPECTED_RBRACE); },
      [this]() {
        return consumeIf(TokenType::SEMICOLON, ErrorType::VARIANTDEF_EXPECTED_SEMICOLON);
      },
  };

  if (!std::all_of(steps.begin(), steps.end(),
                   [](const std::function<bool()>& step) { return step(); })) {
    return nullptr;
  }

  return std::make_unique<VariantDef>(std::move(position), std::move(*name), std::move(*types));
}

/*
 * variantTypes
 *     = variantType, { ",", variantType };
 */
std::optional<VariantDef::Types> Parser::parseVariantTypes() {
  VariantDef::Types types{};

  auto type = parseVariantType();
  if (type == std::nullopt) return types;
  types.push_back(std::move(*type));

  while (m_token.type == TokenType::COMMA) {
    consumeToken();
    type = parseVariantType();
    if (type == std::nullopt) return types;
    if (std::find(types.begin(), types.end(), *type) != types.end()) {
      m_errorHandler(ErrorType::VARIANTTYPE_REDEFINITION, m_token.position);
      return std::nullopt;
    }
    types.push_back(std::move(*type));
  }

  return types;
}

/*
 * variantType
 *     = typeIdentifier
 */
std::optional<VariantDef::Type> Parser::parseVariantType() { return parseTypeIdentifier(); }

/*
 * FnDef
 *    = "fn", identifier, "(", [ fnParams ], ")", returnTypeAnnotation, BlockStmt;
 */
std::unique_ptr<Definition> Parser::parseFnDef() {
  if (m_token.type != TokenType::FN_KWRD) {
    return nullptr;
  }
  consumeToken();
  auto position = m_token.position;

  std::optional<Identifier> name;
  std::optional<FnDef::Params> params;
  std::optional<FnDef::ReturnType> returnType;
  std::unique_ptr<Statement> body;

  std::vector<std::function<bool()>> steps = {
      [this, &name]() {
        if ((name = parseIdentifier()) == std::nullopt) {
          m_errorHandler(ErrorType::FNDEF_EXPECTED_IDENTIFIER, m_token.position);
          return false;
        }
        return true;
      },
      [this]() { return consumeIf(TokenType::LPAREN, ErrorType::FNDEF_EXPECTED_LPAREN); },
      [this, &params]() { return ((params = parseFnParams()) != std::nullopt); },
      [this]() { return consumeIf(TokenType::RPAREN, ErrorType::FNDEF_EXPECTED_RPAREN); },
      [this, &returnType]() {
        if ((returnType = parseFnReturnType()) == std::nullopt) {
          m_errorHandler(ErrorType::FNDEF_EXPECTED_RETURN_TYPE, m_token.position);
          return false;
        }
        return true;
      },
      [this, &body]() {
        if ((body = parseBlockStmt()) == nullptr) {
          m_errorHandler(ErrorType::FNDEF_EXPECTED_BLOCK, m_token.position);
          return false;
        }
        return true;
      },
  };

  if (!std::all_of(steps.begin(), steps.end(),
                   [](const std::function<bool()>& step) { return step(); })) {
    return nullptr;
  }

  BlockStmt* block = dynamic_cast<BlockStmt*>(body.get());
  assert(block != nullptr && "Expected body to be a BlockStmt");
  body.release();

  return std::make_unique<FnDef>(std::move(position), std::move(*name), std::move(*params),
                                 std::move(*returnType), std::move(*block));
}

/*
 * fnParams
 *    = fnParam, { ",", fnParam };
 */
std::optional<FnDef::Params> Parser::parseFnParams() {
  FnDef::Params params{};

  auto param = parseFnParam();
  if (param == std::nullopt) return params;

  params.insert(std::make_pair(param->name, std::move(*param)));
  while (m_token.type == TokenType::COMMA) {
    consumeToken();
    param = parseFnParam();
    if (param == std::nullopt) return params;
    if (params.find(param->name) != params.end()) {
      m_errorHandler(ErrorType::FNPARAM_REDEFINITION, m_token.position);
      return std::nullopt;
    }
    params.insert(std::make_pair(param->name, std::move(*param)));
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
  std::optional<Identifier> name;
  std::optional<TypeIdentifier> type;

  std::vector<std::function<bool()>> steps = {
      [this, &isConst]() {
        if (m_token.type == TokenType::CONST_KWRD) {
          isConst = true;
          consumeToken();
        }
        return true;
      },
      [this, &name]() {
        if ((name = parseIdentifier()) == std::nullopt) {
          m_errorHandler(ErrorType::FNPARAM_EXPECTED_IDENTIFIER, m_token.position);
          return false;
        }
        return true;
      },
      [this]() { return consumeIf(TokenType::COLON, ErrorType::FNPARAM_EXPECTED_COLON); },
      [this, &type]() {
        if ((type = parseTypeIdentifier()) == std::nullopt) {
          m_errorHandler(ErrorType::FNPARAM_EXPECTED_TYPE_IDENTIFIER, m_token.position);
          return false;
        }
        return true;
      },
  };

  if (!std::all_of(steps.begin(), steps.end(),
                   [](const std::function<bool()>& step) { return step(); })) {
    return std::nullopt;
  }

  return FnDef::Param{isConst, std::move(*name), std::move(*type)};
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
  auto type = parseTypeIdentifier();
  if (type == std::nullopt) {
    m_errorHandler(ErrorType::FNRETURNTYPE_EXPECTED_TYPE_IDENTIFIER, m_token.position);
    return std::nullopt;
  }
  return type;
}

/* -------------------------------------------------------------------------- */
/*                                 Expressions                                */
/* -------------------------------------------------------------------------- */

std::unique_ptr<Expression> Parser::parseExpression() {
  if (m_token.type == TokenType::ETX || m_token.type == TokenType::RPAREN) return nullptr;
  auto position = m_token.position;
  consumeToken();
  return std::make_unique<Expression>(std::move(position));
}

/*
 * primaryExpr
 *    = identifierExpr
 *    | literal
 *    | object
 *    | parenExpr
 *    | castExpr;
 */
std::unique_ptr<Expression> Parser::parsePrimaryExpression() {
  auto it = m_primaryExprParsers.find(m_token.type);
  if (it == m_primaryExprParsers.end()) {
    return nullptr;
  }

  return std::move(it->second());
}

/*
 * identifierExpr
 *    = identifier;
 */
std::unique_ptr<Expression> Parser::parseIdentifierExpr() {
  if (m_token.type != TokenType::IDENTIFIER) {
    return nullptr;
  }
  auto position = m_token.position;
  Identifier identifier = m_token.representation;
  consumeToken();
  return std::make_unique<IdentifierExpr>(std::move(position), std::move(identifier));
}

/*
 * object
 *    = "{", [ memberValues ], "}";
 */
std::unique_ptr<Expression> Parser::parseObject() {
  if (m_token.type != TokenType::LBRACE) {
    return nullptr;
  }
  auto position = m_token.position;
  consumeToken();

  std::optional<Object::Members> members;

  if ((members = parseObjectMembers()) == std::nullopt) return nullptr;
  if (!consumeIf(TokenType::RBRACE, ErrorType::OBJECT_EXPECTED_RBRACE)) return nullptr;

  return std::make_unique<Object>(std::move(position), std::move(*members));
}

/*
 * memberValues
 *    = memberValue, { ",", memberValue };
 */
std::optional<Object::Members> Parser::parseObjectMembers() {
  Object::Members members{};

  auto member = parseObjectMember();
  if (member == std::nullopt) return members;
  members.insert(std::make_pair(member->name, std::move(*member)));

  while (m_token.type == TokenType::COMMA) {
    consumeToken();
    member = parseObjectMember();
    if (member == std::nullopt) return members;
    if (members.find(member->name) != members.end()) {
      m_errorHandler(ErrorType::OBJECTMEMBER_REDEFINITION, m_token.position);
      return std::nullopt;
    }
    members.insert(std::make_pair(member->name, std::move(*member)));
  }

  return members;
}

/*
 * memberValue
 *    = identifier, ":", expression;
 */
std::optional<Object::Member> Parser::parseObjectMember() {
  if (m_token.type != TokenType::IDENTIFIER) {
    return std::nullopt;
  }

  auto position = m_token.position;
  std::unique_ptr<Expression> expr;
  Identifier name = m_token.representation;
  consumeToken();

  if (!consumeIf(TokenType::COLON, ErrorType::OBJECTMEMBER_EXPECTED_COLON)) return std::nullopt;
  if ((expr = parseExpression()) == nullptr) return std::nullopt;

  return Object::Member{std::move(name), std::move(expr)};
}

/*
 * parenExpr
 *    = "(", expression, ")";
 */
std::unique_ptr<Expression> Parser::parseParenExpr() {
  if (m_token.type != TokenType::LPAREN) {
    return nullptr;
  }
  auto position = m_token.position;
  consumeToken();

  std::unique_ptr<Expression> expr;
  if ((expr = parseExpression()) == nullptr) return nullptr;
  if (!consumeIf(TokenType::RPAREN, ErrorType::PARENEXPR_EXPECTED_RPAREN)) return nullptr;

  return std::make_unique<ParenExpr>(std::move(position), std::move(expr));
}

/*
 * castExpr
 *    = primitiveType, "(", expression, ")";
 */
std::unique_ptr<Expression> Parser::parseCastExpr() {
  if (!isPrimitiveType(m_token.type)) {
    return nullptr;
  }

  auto position = m_token.position;
  std::unique_ptr<Expression> expr;
  auto type = m_token.type;
  consumeToken();

  if (!consumeIf(TokenType::LPAREN, ErrorType::CASTEXPR_EXPECTED_LPAREN)) return nullptr;
  if ((expr = parseExpression()) == nullptr) return nullptr;
  if (!consumeIf(TokenType::RPAREN, ErrorType::CASTEXPR_EXPECTED_RPAREN)) return nullptr;

  return std::make_unique<CastExpr>(std::move(position), type, std::move(expr));
}

/*
 * functionalExpression
 *   = primaryExpression, { functionalPostfix };
 */
std::unique_ptr<Expression> Parser::parseFunctionalExpression() {
  std::unique_ptr<Expression> expr = nullptr;
  std::unique_ptr<FunctionalExpression::Postfix> postfix = nullptr;
  auto exprPosition = m_token.position;

  if ((expr = parsePrimaryExpression()) == nullptr) return nullptr;

  auto postfixPosition = m_token.position;
  while ((postfix = parseFunctionalExpressionPostfix()) != nullptr) {
    expr = std::make_unique<FunctionalExpression>(std::move(postfixPosition), std::move(expr),
                                                  std::move(postfix));
    postfixPosition = m_token.position;
  }

  return std::make_unique<FunctionalExpression>(std::move(exprPosition), std::move(expr),
                                                std::move(postfix));
}

/* FunctionalPostfix
 *    = FnCallPostfix
 *    | MemberAccessPostfix
 *    | VariantAccessPostfix;
 */
std::unique_ptr<FunctionalExpression::Postfix> Parser::parseFunctionalExpressionPostfix() {
  auto it = m_functionalExprPostfixParsers.find(m_token.type);
  if (it == m_functionalExprPostfixParsers.end()) {
    return nullptr;
  }

  return std::move(it->second());
}

/*
 * fnCallPostfix
 *    = "(", [ fnCallArgs ], ")";
 */
std::unique_ptr<FunctionalExpression::Postfix> Parser::parseFnCallPostfix() {
  if (m_token.type != TokenType::LPAREN) {
    return nullptr;
  }

  auto position = m_token.position;
  consumeToken();

  auto args = parseFnCallArgs();
  if (args == std::nullopt) return nullptr;
  if (!consumeIf(TokenType::RPAREN, ErrorType::FNCALL_EXPECTED_RPAREN)) return nullptr;

  return std::make_unique<FnCall::Postfix>(std::move(position), std::move(*args));
}

/*
 * fnCallArgs
 *    = expression, { ",", expression };
 */
std::optional<FnCall::Args> Parser::parseFnCallArgs() {
  std::vector<std::unique_ptr<Expression>> args{};

  auto arg = parseExpression();
  if (arg == nullptr) return args;
  args.push_back(std::move(arg));

  while (m_token.type == TokenType::COMMA) {
    consumeToken();
    arg = parseExpression();
    if (arg == nullptr) {
      m_errorHandler(ErrorType::FNCALL_EXPECTED_ARGUMENT, m_token.position);
      return std::nullopt;
    }
    args.push_back(std::move(arg));
  }

  return args;
}

/*
 * memberAccessPostfix
 *    = ".", identifier;
 */
std::unique_ptr<FunctionalExpression::Postfix> Parser::parseMemberAccessPostfix() {
  if (m_token.type != TokenType::DOT) {
    return nullptr;
  }
  auto position = m_token.position;
  consumeToken();

  std::optional<Identifier> name;
  if ((name = parseIdentifier()) == std::nullopt) {
    m_errorHandler(ErrorType::MEMBERACCESS_EXPECTED_IDENTIFIER, m_token.position);
    return nullptr;
  }

  return std::make_unique<MemberAccess::Postfix>(std::move(position), std::move(*name));
}

/*
 * variantAccessPostfix
 *    = "as", typeIdentifier;
 */
std::unique_ptr<FunctionalExpression::Postfix> Parser::parseVariantAccessPostfix() {
  if (m_token.type != TokenType::AS_KWRD) {
    return nullptr;
  }

  auto position = m_token.position;
  consumeToken();

  std::optional<TypeIdentifier> type;
  if ((type = parseTypeIdentifier()) == std::nullopt) {
    m_errorHandler(ErrorType::VARIANTACCESS_EXPECTED_TYPE_IDENTIFIER, m_token.position);
    return nullptr;
  }

  return std::make_unique<VariantAccess::Postfix>(std::move(position), std::move(*type));
}

/* -------------------------------------------------------------------------- */
/*                                 Statements                                 */
/* -------------------------------------------------------------------------- */

std::unique_ptr<Statement> Parser::parseStatement() {
  return nullptr;
  // auto it = m_statementParsers.find(m_token.type);
  // if (it == m_statementParsers.end()) {
  //   return nullptr;
  // }

  // return std::move(it->second());
}

std::unique_ptr<Statement> Parser::parseBlockStmt() {
  if (m_token.type != TokenType::LBRACE) {
    return nullptr;
  }
  auto position = m_token.position;
  consumeToken();

  BlockStmt::Statements statements{};

  while (m_token.type != TokenType::RBRACE) {
    // if (auto statement = parseStatement(); statement != nullptr) {
    //   statements.push_back(std::move(statement));
    // } else {
    //   return nullptr;
    // }
    consumeToken();
  }

  consumeToken();

  return std::make_unique<BlockStmt>(std::move(position), std::move(statements));
}
