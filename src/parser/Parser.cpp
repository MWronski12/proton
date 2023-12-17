#include <cassert>

#include "ErrorType.h"
#include "Parser.h"
#include "TokenType.h"

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
      return std::nullopt;
    }
    // Success, add to map
    else {
      auto entry = std::make_pair(definition->name, std::move(definition));
      definitions.insert(std::move(entry));
    }

    definition = parseDefinition();
  }

  if (definitions.find(L"main") == definitions.end()) {
    m_errorHandler(ErrorType::EXPECTED_MAIN_FUNCTION_DEF, position);
    return std::nullopt;
  }

  return Program{std::move(position), std::move(definitions)};
}

/* -------------------------------------------------------------------------- */
/*                               Utility Methods                              */
/* -------------------------------------------------------------------------- */

void Parser::consumeToken() { m_token = m_lexer.getNextToken(); }

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

  if ((name = parseIdentifier()) == std::nullopt) {
    m_errorHandler(ErrorType::VARDEF_EXPECTED_IDENTIFIER, m_token.position);
    return nullptr;
  }
  if (!consumeIf(TokenType::COLON, ErrorType::VARDEF_EXPECTED_COLON)) return nullptr;
  if ((type = parseTypeIdentifier()) == std::nullopt) {
    m_errorHandler(ErrorType::VARDEF_EXPECTED_TYPE_IDENTIFIER, m_token.position);
    return nullptr;
  }
  if (!consumeIf(TokenType::ASSIGNMENT, ErrorType::VARDEF_EXPECTED_ASSIGNMENT)) return nullptr;
  if ((expr = parseExpression()) == nullptr) {
    m_errorHandler(ErrorType::VARDEF_EXPECTED_EXPRESSION, m_token.position);
    return nullptr;
  }
  if (!consumeIf(TokenType::SEMICOLON, ErrorType::VARDEF_EXPECTED_SEMICOLON)) return nullptr;

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

  if ((name = parseIdentifier()) == std::nullopt) {
    m_errorHandler(ErrorType::CONSTDEF_EXPECTED_IDENTIFIER, m_token.position);
    return nullptr;
  }
  if (!consumeIf(TokenType::COLON, ErrorType::CONSTDEF_EXPECTED_COLON)) return nullptr;
  if ((type = parseTypeIdentifier()) == std::nullopt) {
    m_errorHandler(ErrorType::CONSTDEF_EXPECTED_TYPE_IDENTIFIER, m_token.position);
    return nullptr;
  }
  if (!consumeIf(TokenType::ASSIGNMENT, ErrorType::CONSTDEF_EXPECTED_ASSIGNMENT)) return nullptr;
  if ((expr = parseExpression()) == nullptr) {
    m_errorHandler(ErrorType::CONSTDEF_EXPECTED_EXPRESSION, m_token.position);
    return nullptr;
  }
  if (!consumeIf(TokenType::SEMICOLON, ErrorType::CONSTDEF_EXPECTED_SEMICOLON)) return nullptr;

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

  if ((name = parseIdentifier()) == std::nullopt) {
    m_errorHandler(ErrorType::STRUCTDEF_EXPECTED_IDENTIFIER, m_token.position);
    return nullptr;
  }
  if (!consumeIf(TokenType::LBRACE, ErrorType::STRUCTDEF_EXPECTED_LBRACE)) return nullptr;
  if ((members = parseStructMembers()) == std::nullopt) return nullptr;
  if (!consumeIf(TokenType::RBRACE, ErrorType::STRUCTDEF_EXPECTED_RBRACE)) return nullptr;
  if (!consumeIf(TokenType::SEMICOLON, ErrorType::STRUCTDEF_EXPECTED_SEMICOLON)) return nullptr;

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

  name = parseIdentifier();
  if (!consumeIf(TokenType::COLON, ErrorType::STRUCTMEMBER_EXPECTED_COLON)) return std::nullopt;
  if ((type = parseTypeIdentifier()) == std::nullopt) {
    m_errorHandler(ErrorType::STRUCTMEMBER_EXPECTED_TYPE_IDENTIFIER, m_token.position);
    return std::nullopt;
  }
  if (!consumeIf(TokenType::SEMICOLON, ErrorType::STRUCTMEMBER_EXPECTED_SEMICOLON)) {
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

  if ((name = parseIdentifier()) == std::nullopt) {
    m_errorHandler(ErrorType::VARIANTDEF_EXPECTED_IDENTIFIER, m_token.position);
    return nullptr;
  }
  if (!consumeIf(TokenType::LBRACE, ErrorType::VARIANTDEF_EXPECTED_LBRACE)) return nullptr;
  if ((types = parseVariantTypes()) == std::nullopt) return nullptr;
  if (!consumeIf(TokenType::RBRACE, ErrorType::VARIANTDEF_EXPECTED_RBRACE)) return nullptr;
  if (!consumeIf(TokenType::SEMICOLON, ErrorType::VARIANTDEF_EXPECTED_SEMICOLON)) return nullptr;

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

  if ((name = parseIdentifier()) == std::nullopt) {
    m_errorHandler(ErrorType::FNDEF_EXPECTED_IDENTIFIER, m_token.position);
    return nullptr;
  }
  if (!consumeIf(TokenType::LPAREN, ErrorType::FNDEF_EXPECTED_LPAREN)) return nullptr;
  if ((params = parseFnParams()) == std::nullopt) return nullptr;
  if (!consumeIf(TokenType::RPAREN, ErrorType::FNDEF_EXPECTED_RPAREN)) return nullptr;
  if ((returnType = parseFnReturnType()) == std::nullopt) {
    m_errorHandler(ErrorType::FNDEF_EXPECTED_RETURN_TYPE, m_token.position);
    return nullptr;
  }
  if ((body = parseBlockStmt()) == nullptr) {
    m_errorHandler(ErrorType::FNDEF_EXPECTED_BLOCK, m_token.position);
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

  if (m_token.type == TokenType::CONST_KWRD) {
    isConst = true;
    consumeToken();
  }
  if ((name = parseIdentifier()) == std::nullopt) {
    m_errorHandler(ErrorType::FNPARAM_EXPECTED_IDENTIFIER, m_token.position);
    return std::nullopt;
  }
  if (!consumeIf(TokenType::COLON, ErrorType::FNPARAM_EXPECTED_COLON)) return std::nullopt;
  if ((type = parseTypeIdentifier()) == std::nullopt) {
    m_errorHandler(ErrorType::FNPARAM_EXPECTED_TYPE_IDENTIFIER, m_token.position);
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

/*
 * BinaryExpression
 *    = LogicalOrExpr
 *    | LogicAndExpr
 *    | EqualityExpr
 *    | RelationalExpr
 *    | AdditiveExpr
 *    | MultiplicativeExpr;
 */
std::unique_ptr<Expression> Parser::parseBinaryExpression(
    std::unique_ptr<Expression> (Parser::*parseSubExpr)(), std::vector<TokenType>&& operators) {
  auto lhs = (this->*parseSubExpr)();
  if (lhs == nullptr) return nullptr;

  while (std::find(operators.cbegin(), operators.cend(), m_token.type) != operators.cend()) {
    auto op = m_token.type;
    consumeToken();

    auto rhs = (this->*parseSubExpr)();
    if (rhs == nullptr) {
      m_errorHandler.handleError(ErrorType::BINARYEXPRESSION_EXPECTED_RHS, m_token.position);
      return nullptr;
    };

    auto position = lhs->position;
    lhs =
        std::make_unique<BinaryExpression>(std::move(position), std::move(lhs), op, std::move(rhs));
  }

  return lhs;
}

/*
 * UnaryExpression
 *    = UnaryOpExpr;
 */
std::unique_ptr<Expression> Parser::parseUnaryExpression(
    std::unique_ptr<Expression> (Parser::*parseSubExpr)(), std::vector<TokenType>&& operators) {
  if (std::find(operators.cbegin(), operators.cend(), m_token.type) == operators.cend()) {
    // No operator - just return the subexpression
    return (this->*parseSubExpr)();
  }

  auto position = m_token.position;
  auto op = m_token.type;
  consumeToken();

  auto expr = (this->*parseSubExpr)();
  if (expr == nullptr) {
    // No expression after the operator
    m_errorHandler.handleError(ErrorType::UNARYEXPRESSION_EXPECTED_EXPR, m_token.position);
    return nullptr;
  }

  return std::make_unique<UnaryExpression>(std::move(position), op, std::move(expr));
}

/*
 * Expression
 *     = LogicOrExpr;
 */
std::unique_ptr<Expression> Parser::parseExpression() { return parseLogicOrExpr(); }

/*
 * LogicOrExpr
 *     = LogicAndExpr, { logicOrOp, LogicAndExpr };
 */
std::unique_ptr<Expression> Parser::parseLogicOrExpr() {
  return parseBinaryExpression(&Parser::parseLogicAndExpr, {TokenType::LOGIC_OR});
}

/*
 * LogicAndExpr
 *     = EqualityExpr, { logicAndOp, EqualityExpr };
 */
std::unique_ptr<Expression> Parser::parseLogicAndExpr() {
  return parseBinaryExpression(&Parser::parseEqualityExpr, {TokenType::LOGIC_AND});
}

/*
 * EqualityExpr
 *     = RelationalExpr, { equalityOp, RelationalExpr };
 */
std::unique_ptr<Expression> Parser::parseEqualityExpr() {
  return parseBinaryExpression(&Parser::parseRelationalExpr,
                               {TokenType::EQUALITY, TokenType::INEQUALITY});
}

/*
 * RelationalExpr
 *     = AdditiveExpr, { relationalOp, AdditiveExpr };
 */
std::unique_ptr<Expression> Parser::parseRelationalExpr() {
  return parseBinaryExpression(&Parser::parseAdditiveExpr,
                               {TokenType::LESS_THAN, TokenType::GREATER_THAN,
                                TokenType::LESS_OR_EQUAL, TokenType::GREATER_OR_EQUAL});
}

/*
 * AdditiveExpr
 *     = MultiplicativeExpr, { additiveOp, MultiplicativeExpr };
 */
std::unique_ptr<Expression> Parser::parseAdditiveExpr() {
  return parseBinaryExpression(&Parser::parseMultiplicativeExpr,
                               {TokenType::PLUS, TokenType::MINUS});
}

/*
 * MultiplicativeExpr
 *     = UnaryExpr, { multiplicativeOp, UnaryExpr };
 */
std::unique_ptr<Expression> Parser::parseMultiplicativeExpr() {
  return parseBinaryExpression(&Parser::parseUnaryOpExpr,
                               {TokenType::ASTERISK, TokenType::SLASH, TokenType::PERCENT});
}

/*
 * UnaryOpExpr
 *     = [ unaryOp ], PrimaryExpr;
 */
std::unique_ptr<Expression> Parser::parseUnaryOpExpr() {
  return parseUnaryExpression(&Parser::parseFunctionalExpression,
                              {TokenType::LOGIC_NOT, TokenType::MINUS});
}

/*
 * functionalExpr
 *   = primaryExpression, { functionalPostfix };
 */
std::unique_ptr<Expression> Parser::parseFunctionalExpression() {
  std::unique_ptr<Expression> expr = nullptr;
  std::unique_ptr<FunctionalExpression::Postfix> postfix = nullptr;

  if ((expr = parsePrimaryExpression()) == nullptr) return nullptr;

  auto postfixPosition = m_token.position;
  while ((postfix = parseFunctionalExpressionPostfix()) != nullptr) {
    expr = std::make_unique<FunctionalExpression>(std::move(postfixPosition), std::move(expr),
                                                  std::move(postfix));
    postfixPosition = m_token.position;
  }

  return expr;
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
  if ((expr = parseExpression()) == nullptr) {
    m_errorHandler(ErrorType::OBJECTMEMBER_EXPECTED_EXPRESSION, m_token.position);
    return std::nullopt;
  };

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
  if ((expr = parseExpression()) == nullptr) {
    m_errorHandler(ErrorType::PARENEXPR_EXPECTED_EXPRESSION, m_token.position);
    return nullptr;
  }
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
  if ((expr = parseExpression()) == nullptr) {
    m_errorHandler(ErrorType::CASTEXPR_EXPECTED_EXPRESSION, m_token.position);
    return nullptr;
  }
  if (!consumeIf(TokenType::RPAREN, ErrorType::CASTEXPR_EXPECTED_RPAREN)) return nullptr;

  return std::make_unique<CastExpr>(std::move(position), type, std::move(expr));
}

/* -------------------------------------------------------------------------- */
/*                                 Statements                                 */
/* -------------------------------------------------------------------------- */

/*
 * Statement
 *    = Definition
 *    | BlockStmt
 *    | ExpressionOrAssignmentStmt
 *    | StdinExtractionStmt
 *    | StdoutInsertionStmt
 *    | VariantMatchStmt
 *    | IfStmt
 *    | ForStmt
 *    | WhileStmt
 *    | ContinueStmt
 *    | BreakStmt
 *    | ReturnStmt;
 */
std::unique_ptr<Statement> Parser::parseStatement() {
  auto defParserPtr = m_definitionParsers.find(m_token.type);
  if (defParserPtr != m_definitionParsers.end()) {
    return std::move(defParserPtr->second());
  }

  auto stmtParserPtr = m_statementParsers.find(m_token.type);
  if (stmtParserPtr != m_statementParsers.end()) {
    return std::move(stmtParserPtr->second());
  }

  return parseExpressionOrAssignmentStmt();
}

/*
 * BlockStmt
 *     = "{", { Statement }, "}";
 */
std::unique_ptr<Statement> Parser::parseBlockStmt() {
  if (m_token.type != TokenType::LBRACE) {
    return nullptr;
  }
  auto position = m_token.position;
  consumeToken();

  BlockStmt::Statements statements{};

  while (m_token.type != TokenType::RBRACE) {
    if (auto statement = parseStatement(); statement != nullptr) {
      statements.push_back(std::move(statement));
    } else {
      return nullptr;
    }
  }

  consumeToken();

  return std::make_unique<BlockStmt>(std::move(position), std::move(statements));
}

/*
 * ExpressionOrAssignmentStatement
 *     = Expression, [ "=", Expression ], ";";
 */
std::unique_ptr<Statement> Parser::parseExpressionOrAssignmentStmt() {
  std::unique_ptr<Expression> expr;
  std::unique_ptr<Expression> value;
  auto position = m_token.position;

  if ((expr = parseExpression()) == nullptr) return nullptr;

  // Parse AssignmentStatement
  if (m_token.type == TokenType::ASSIGNMENT) {
    consumeToken();
    if ((value = parseExpression()) == nullptr) {
      m_errorHandler(ErrorType::ASSIGNMENTSTMT_EXPECTED_EXPRESSION, m_token.position);
      return nullptr;
    };

    if (!consumeIf(TokenType::SEMICOLON, ErrorType::ASSIGNMENTSTMT_EXPECTED_SEMICOLON))
      return nullptr;

    return std::make_unique<AssignmentStmt>(std::move(position), std::move(expr), std::move(value));
  }

  // Parse ExpressionStatement
  if (!consumeIf(TokenType::SEMICOLON, ErrorType::EXPRESSIONSTMT_EXPECTED_SEMICOLON))
    return nullptr;

  return std::make_unique<ExpressionStmt>(std::move(position), std::move(expr));
}

/*
 * StdinExtractionStmt
 *     = ">>", Expression, ";";
 */
std::unique_ptr<Statement> Parser::parseStdinExtractionStmt() {
  if (m_token.type != TokenType::EXTRACTION_OP) {
    return nullptr;
  }
  auto position = m_token.position;
  consumeToken();

  std::unique_ptr<Expression> expr;
  if ((expr = parseExpression()) == nullptr) {
    m_errorHandler(ErrorType::STDINEXTRACTION_EXPECTED_EXPRESSION, m_token.position);
    return nullptr;
  };

  if (!consumeIf(TokenType::SEMICOLON, ErrorType::STDINEXTRACTION_EXPECTED_SEMICOLON))
    return nullptr;

  return std::make_unique<StdinExtractionStmt>(std::move(position), std::move(expr));
}

/*
 * StdoutInsertionStmt
 *     = "<<", Expression, ";";
 */
std::unique_ptr<Statement> Parser::parseStdoutInsertionStmt() {
  if (m_token.type != TokenType::INSERTION_OP) {
    return nullptr;
  }
  auto position = m_token.position;
  consumeToken();

  std::unique_ptr<Expression> expr;
  if ((expr = parseExpression()) == nullptr) {
    m_errorHandler(ErrorType::STDOUTINSERTION_EXPECTED_EXPRESSION, m_token.position);
    return nullptr;
  };

  if (!consumeIf(TokenType::SEMICOLON, ErrorType::STDOUTINSERTION_EXPECTED_SEMICOLON))
    return nullptr;

  return std::make_unique<StdoutInsertionStmt>(std::move(position), std::move(expr));
}

/*
 * VariantMatchStmt
 *     = "match", Expression, "{", VariantMatchCases, "}";
 */
std::unique_ptr<Statement> Parser::parseVariantMatchStmt() {
  if (m_token.type != TokenType::MATCH_KWRD) {
    return nullptr;
  }
  auto position = m_token.position;
  consumeToken();

  std::unique_ptr<Expression> expr;
  std::optional<VariantMatchStmt::Cases> cases;

  if ((expr = parseExpression()) == nullptr) {
    m_errorHandler(ErrorType::VARIANTMATCH_EXPECTED_EXPRESSION, m_token.position);
    return nullptr;
  };

  if (!consumeIf(TokenType::LBRACE, ErrorType::VARIANTMATCH_EXPECTED_LBRACE)) return nullptr;
  if ((cases = parseVariantMatchCases()) == std::nullopt) return nullptr;
  if (!consumeIf(TokenType::RBRACE, ErrorType::VARIANTMATCH_EXPECTED_RBRACE)) return nullptr;

  return std::make_unique<VariantMatchStmt>(std::move(position), std::move(expr),
                                            std::move(*cases));
}

/*
 * VariantMatchCases
 *     = { variantMatchCases }
 */
std::optional<VariantMatchStmt::Cases> Parser::parseVariantMatchCases() {
  VariantMatchStmt::Cases cases{};

  auto position = m_token.position;

  auto variantCase = parseVariantMatchCase();
  while (variantCase != std::nullopt) {
    if (cases.find(variantCase->variant) != cases.end()) {
      m_errorHandler(ErrorType::VARIANTMATCHCASE_REDEFINITION, m_token.position);
      return std::nullopt;
    } else {
      cases.insert(std::make_pair(variantCase->variant, std::move(*variantCase)));
    }
    variantCase = parseVariantMatchCase();
  }

  return cases;
}

/*
 * variantMatchCase
 *     = "case", typeIdentifier, "->", BlockStmt;
 */
std::optional<VariantMatchStmt::Case> Parser::parseVariantMatchCase() {
  if (m_token.type != TokenType::CASE_KWRD) {
    return std::nullopt;
  }
  auto position = m_token.position;
  consumeToken();

  std::optional<TypeIdentifier> variant;
  std::unique_ptr<Statement> block;

  if ((variant = parseTypeIdentifier()) == std::nullopt) {
    m_errorHandler(ErrorType::VARIANTMATCHCASE_EXPECTED_TYPE, m_token.position);
    return std::nullopt;
  }

  if (!consumeIf(TokenType::ARROW, ErrorType::VARIANTMATCHCASE_EXPECTED_ARROW)) return std::nullopt;

  if ((block = parseBlockStmt()) == nullptr) {
    m_errorHandler(ErrorType::VARIANTMATCHCASE_EXPECTED_BLOCK, m_token.position);
    return std::nullopt;
  }

  auto blockStmt = dynamic_cast<BlockStmt*>(block.get());
  assert(blockStmt != nullptr && "Expected body to be a BlockStmt");
  block.release();

  return VariantMatchStmt::Case{std::move(position), std::move(*variant), std::move(*blockStmt)};
}

/*
 * IfStmt
 *     = "if", Expression, BlockStmt, { Elif }, [ Else ];
 */
std::unique_ptr<Statement> Parser::parseIfStmt() {
  if (m_token.type != TokenType::IF_KWRD) {
    return nullptr;
  }
  auto position = m_token.position;
  consumeToken();

  std::unique_ptr<Expression> condition;
  std::unique_ptr<Statement> body;
  IfStmt::Elifs elifs;
  std::optional<IfStmt::Else> elseClause;

  if ((condition = parseExpression()) == nullptr) {
    m_errorHandler(ErrorType::IF_EXPECTED_CONDITION, m_token.position);
    return nullptr;
  }

  if ((body = parseBlockStmt()) == nullptr) {
    m_errorHandler(ErrorType::IF_EXPECTED_BLOCK, m_token.position);
    return nullptr;
  }

  elifs = parseElifs();
  elseClause = parseElse();

  auto block = dynamic_cast<BlockStmt*>(body.get());
  assert(block != nullptr && "Expected body to be a BlockStmt");
  body.release();

  return std::make_unique<IfStmt>(std::move(position), std::move(condition), std::move(*block),
                                  std::move(elifs), std::move(elseClause));
}

/*
 * Elifs
 *     = { Elif };
 */
IfStmt::Elifs Parser::parseElifs() {
  IfStmt::Elifs elifs{};

  auto elif = parseElif();
  while (elif != std::nullopt) {
    elifs.push_back(std::move(*elif));
    elif = parseElif();
  }

  return elifs;
}

/*
 * Elif
 *     = "elif", Expression, BlockStmt;
 */
std::optional<IfStmt::Elif> Parser::parseElif() {
  if (m_token.type != TokenType::ELIF_KWRD) {
    return std::nullopt;
  }
  auto position = m_token.position;
  consumeToken();

  std::unique_ptr<Expression> condition;
  std::unique_ptr<Statement> body;

  if ((condition = parseExpression()) == nullptr) {
    m_errorHandler(ErrorType::ELIF_EXPECTED_CONDITION, m_token.position);
    return std::nullopt;
  }

  if ((body = parseBlockStmt()) == nullptr) {
    m_errorHandler(ErrorType::ELIF_EXPECTED_BLOCK, m_token.position);
    return std::nullopt;
  }

  auto block = dynamic_cast<BlockStmt*>(body.get());
  assert(block != nullptr && "Expected body to be a BlockStmt");
  body.release();

  return IfStmt::Elif{std::move(position), std::move(condition), std::move(*block)};
}

/*
 * Else
 *     = "else", BlockStmt;
 */
std::optional<IfStmt::Else> Parser::parseElse() {
  if (m_token.type != TokenType::ELSE_KWRD) {
    return std::nullopt;
  }
  auto position = m_token.position;
  consumeToken();

  std::unique_ptr<Statement> body;

  if ((body = parseBlockStmt()) == nullptr) {
    m_errorHandler(ErrorType::ELSE_EXPECTED_BLOCK, m_token.position);
    return std::nullopt;
  }

  auto block = dynamic_cast<BlockStmt*>(body.get());
  assert(block != nullptr && "Expected body to be a BlockStmt");
  body.release();

  return IfStmt::Else{std::move(position), std::move(*block)};
}

/*
 * ForStmt
 *     = "for", Identifier, "in", Range, BlockStmt;
 */
std::unique_ptr<Statement> Parser::parseForStmt() {
  if (m_token.type != TokenType::FOR_KWRD) {
    return nullptr;
  }
  auto position = m_token.position;
  consumeToken();

  std::optional<Identifier> identifier;
  std::optional<ForStmt::Range> range;
  std::unique_ptr<Statement> body;

  if ((identifier = parseIdentifier()) == std::nullopt) {
    m_errorHandler(ErrorType::FOR_EXPECTED_IDENTIFIER, m_token.position);
    return nullptr;
  }

  if (!consumeIf(TokenType::IN_KWRD, ErrorType::FOR_EXPECTED_IN)) return nullptr;

  if ((range = parseForRange()) == std::nullopt) {
    m_errorHandler(ErrorType::FOR_EXPECTED_RANGE, m_token.position);
    return nullptr;
  }

  if ((body = parseBlockStmt()) == nullptr) {
    m_errorHandler(ErrorType::FOR_EXPECTED_BLOCK, m_token.position);
    return nullptr;
  }

  auto block = dynamic_cast<BlockStmt*>(body.get());
  assert(block != nullptr && "Expected body to be a BlockStmt");
  body.release();

  return std::make_unique<ForStmt>(std::move(position), std::move(*identifier), std::move(*range),
                                   std::move(*block));
}

/*
 * Range
 *     = Expression, "until", Expression;
 */
std::optional<ForStmt::Range> Parser::parseForRange() {
  std::unique_ptr<Expression> from;
  std::unique_ptr<Expression> to;

  auto position = m_token.position;

  if ((from = parseExpression()) == nullptr) return std::nullopt;
  if (!consumeIf(TokenType::UNTIL_KWRD, ErrorType::FORRANGE_EXPECTED_UNTIL)) return std::nullopt;

  if ((to = parseExpression()) == nullptr) {
    m_errorHandler(ErrorType::FORRANGE_EXPECTED_END_EXPR, m_token.position);
    return std::nullopt;
  }

  return ForStmt::Range{std::move(position), std::move(from), std::move(to)};
}

/*
 * WhileStmt
 *     = "while", Expression, BlockStmt;
 */
std::unique_ptr<Statement> Parser::parseWhileStmt() {
  if (m_token.type != TokenType::WHILE_KWRD) {
    return nullptr;
  }
  auto position = m_token.position;
  consumeToken();

  std::unique_ptr<Expression> condition;
  std::unique_ptr<Statement> body;

  if ((condition = parseExpression()) == nullptr) {
    m_errorHandler(ErrorType::WHILE_EXPECTED_CONDITION, m_token.position);
    return nullptr;
  }

  if ((body = parseBlockStmt()) == nullptr) {
    m_errorHandler(ErrorType::WHILE_EXPECTED_BLOCK, m_token.position);
    return nullptr;
  }

  auto block = dynamic_cast<BlockStmt*>(body.get());
  assert(block != nullptr && "Expected body to be a BlockStmt");
  body.release();

  return std::make_unique<WhileStmt>(std::move(position), std::move(condition), std::move(*block));
}

/*
 * ContinueStmt
 *     = "continue", ";";
 */
std::unique_ptr<Statement> Parser::parseContinueStmt() {
  if (m_token.type != TokenType::CONTINUE_KWRD) {
    return nullptr;
  }
  auto position = m_token.position;
  consumeToken();

  if (!consumeIf(TokenType::SEMICOLON, ErrorType::CONTINUE_EXPECTED_SEMICOLON)) return nullptr;

  return std::make_unique<ContinueStmt>(std::move(position));
}

/*
 * BreakStmt
 *     = "break", ";";
 */
std::unique_ptr<Statement> Parser::parseBreakStmt() {
  if (m_token.type != TokenType::BREAK_KWRD) {
    return nullptr;
  }
  auto position = m_token.position;
  consumeToken();

  if (!consumeIf(TokenType::SEMICOLON, ErrorType::BREAK_EXPECTED_SEMICOLON)) return nullptr;

  return std::make_unique<BreakStmt>(std::move(position));
}

/*
 * ReturnStmt
 *     = "return", [ Expression ], ";";
 */
std::unique_ptr<Statement> Parser::parseReturnStmt() {
  if (m_token.type != TokenType::RETURN_KWRD) {
    return nullptr;
  }
  auto position = m_token.position;
  consumeToken();

  std::unique_ptr<Expression> expr = parseExpression();
  if (!consumeIf(TokenType::SEMICOLON, ErrorType::RETURN_EXPECTED_SEMICOLON)) return nullptr;

  return std::make_unique<ReturnStmt>(std::move(position), std::move(expr));
}
