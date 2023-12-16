#include "Expression.h"
#include "ParserTest.h"

/* ----------------------------- IdentifierExpr ----------------------------- */

TEST_F(ParserTest, ParserHandlesIdentifierExpr) {
  m_reader.load(L"foo bar voidBaz 111");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  auto expr = parseIdentifierExpr();
  ASSERT_TRUE(expr != nullptr);
  expr = parseIdentifierExpr();
  ASSERT_TRUE(expr != nullptr);
  expr = parseIdentifierExpr();
  ASSERT_TRUE(expr != nullptr);
  expr = parseIdentifierExpr();
  ASSERT_TRUE(expr == nullptr);
}

/* --------------------------------- Literal -------------------------------- */

TEST_F(ParserTest, ParserHandlesValidLiterals) {
  m_reader.load(L"666 777.0 true false \'c\' \"hello\"");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);

  auto literal = parseLiteral<int>();
  ASSERT_TRUE(literal != nullptr);
  literal = parseLiteral<float>();
  ASSERT_TRUE(literal != nullptr);
  literal = parseLiteral<bool>();
  ASSERT_TRUE(literal != nullptr);
  literal = parseLiteral<bool>();
  ASSERT_TRUE(literal != nullptr);
  literal = parseLiteral<wchar_t>();
  ASSERT_TRUE(literal != nullptr);
  literal = parseLiteral<std::wstring>();
  ASSERT_TRUE(literal != nullptr);
}

TEST_F(ParserTest, ParserHandlesMismatchedLiterals) {
  m_reader.load(L"\"hello\"");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);

  auto literal = parseLiteral<float>();
  ASSERT_TRUE(literal == nullptr);
  literal = parseLiteral<int>();
  ASSERT_TRUE(literal == nullptr);
  literal = parseLiteral<bool>();
  ASSERT_TRUE(literal == nullptr);
}

/* ----------------------------- Object::Member ----------------------------- */

TEST_F(ParserTest, ParserHandlesObjectMember) {
  m_reader.load(L"foo: 666     777");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);

  auto member = parseObjectMember();
  ASSERT_TRUE(member != std::nullopt);
  EXPECT_EQ(member->name, L"foo");
  ASSERT_TRUE(member->value != nullptr);

  member = parseObjectMember();
  ASSERT_TRUE(member == std::nullopt);
}

TEST_F(ParserTest, ParserHandlesObjectMemberMissingColon) {
  m_reader.load(L"foo 666");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::OBJECTMEMBER_EXPECTED_COLON, _)).Times(1);

  auto member = parseObjectMember();
  ASSERT_TRUE(member == std::nullopt);
}

TEST_F(ParserTest, ParserHandlesObjectMemberMissingExpr) {
  m_reader.load(L"foo: ");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::OBJECTMEMBER_EXPECTED_EXPRESSION, _)).Times(1);

  auto member = parseObjectMember();
  ASSERT_TRUE(member == std::nullopt);
}

/* ----------------------------- Object::Members ---------------------------- */

TEST_F(ParserTest, ParserHandlesObjectEmptyMembers) {
  m_reader.load(L"");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);

  auto members = parseObjectMembers();
  ASSERT_TRUE(members != std::nullopt);
  ASSERT_EQ(members->size(), 0);
}

TEST_F(ParserTest, ParserHandlesObjectMembers) {
  m_reader.load(L"foo: 666, bar: 777");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);

  auto members = parseObjectMembers();
  ASSERT_TRUE(members != std::nullopt);
  ASSERT_EQ(members->size(), 2);
}

TEST_F(ParserTest, ParserAllowsForTrailingCommaInObjectMembers) {
  m_reader.load(L"foo: 666, bar: 777,  1   foo: \"fiu\", bar: \'b\'");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);

  auto members = parseObjectMembers();
  ASSERT_TRUE(members != std::nullopt);
  ASSERT_EQ(members->size(), 2);

  auto literal = parseLiteral<int>();
  ASSERT_TRUE(literal != nullptr);

  members = parseObjectMembers();
  ASSERT_TRUE(members != std::nullopt);
  ASSERT_EQ(members->size(), 2);
}

/* --------------------------------- Object --------------------------------- */

TEST_F(ParserTest, ParserHandlesEmptyObject) {
  m_reader.load(L"{}");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);

  auto objExpr = parseObject();
  Object* object = dynamic_cast<Object*>(objExpr.get());
  ASSERT_TRUE(object != nullptr);
  ASSERT_EQ(object->members.size(), 0);
}

TEST_F(ParserTest, ParserHandlesObject) {
  m_reader.load(
      L"{ integer: 1, floating: 2.0, truth: true, character: \'c\', stringified: \"s\", }");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);

  auto objExpr = parseObject();
  Object* object = dynamic_cast<Object*>(objExpr.get());
  ASSERT_TRUE(object != nullptr);
  ASSERT_EQ(object->members.size(), 5);
}

/* -------------------------------- ParenExpr ------------------------------- */

TEST_F(ParserTest, ParserHandlesEmptyParenExpr) {
  m_reader.load(L"");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);

  auto parenExpr = parseParenExpr();
  ASSERT_TRUE(parenExpr == nullptr);
}

TEST_F(ParserTest, ParserHandlesParenExpr) {
  m_reader.load(L"(666)");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);

  auto parenExpr = parseParenExpr();
  ASSERT_TRUE(parenExpr != nullptr);
}

TEST_F(ParserTest, ParserHandlesParenExprMissingExpr) {
  m_reader.load(L"()");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::PARENEXPR_EXPECTED_EXPRESSION, _)).Times(1);

  auto parenExpr = parseParenExpr();
  ASSERT_TRUE(parenExpr == nullptr);
}

TEST_F(ParserTest, ParserHandlesMissingClosingParenInParenExpr) {
  m_reader.load(L"(666");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::PARENEXPR_EXPECTED_RPAREN, _)).Times(1);

  auto parenExpr = parseParenExpr();
  ASSERT_TRUE(parenExpr == nullptr);
}

/* -------------------------------- CastExpr -------------------------------- */

TEST_F(ParserTest, ParserHandlesCastingToInt) {
  m_reader.load(L"int(1.0)");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);

  auto castExpr = parseCastExpr();
  ASSERT_TRUE(castExpr != nullptr);
}

TEST_F(ParserTest, ParserHandlesCastingToFloat) {
  m_reader.load(L"float(1)");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);

  auto castExpr = parseCastExpr();
  ASSERT_TRUE(castExpr != nullptr);
}

TEST_F(ParserTest, ParserHandlesCastingToBool) {
  m_reader.load(L"bool(1)");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);

  auto castExpr = parseCastExpr();
  ASSERT_TRUE(castExpr != nullptr);
}

TEST_F(ParserTest, ParserHandlesCastingToChar) {
  m_reader.load(L"char(1)");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);

  auto castExpr = parseCastExpr();
  ASSERT_TRUE(castExpr != nullptr);
}

TEST_F(ParserTest, ParserHandlesCastingToString) {
  m_reader.load(L"string(1)");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);

  auto castExpr = parseCastExpr();
  ASSERT_TRUE(castExpr != nullptr);
}

TEST_F(ParserTest, ParserHandlesMissingLParenInCastExpr) {
  m_reader.load(L"int 1");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::CASTEXPR_EXPECTED_LPAREN, _)).Times(1);

  auto castExpr = parseCastExpr();
  ASSERT_TRUE(castExpr == nullptr);
}

TEST_F(ParserTest, ParserHandlesCastExprMissingExpr) {
  m_reader.load(L"(1)");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::CASTEXPR_EXPECTED_EXPRESSION, _)).Times(1);

  auto castExpr = parseCastExpr();
  ASSERT_TRUE(castExpr == nullptr);
}

TEST_F(ParserTest, ParserHandlesMissingRParenInCastExpr) {
  m_reader.load(L"int(1");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::CASTEXPR_EXPECTED_RPAREN, _)).Times(1);

  auto castExpr = parseCastExpr();
  ASSERT_TRUE(castExpr == nullptr);
}

/* ---------------------------- PrimaryExpression --------------------------- */

TEST_F(ParserTest, ParserHandlesEmptyPrimaryExpr) {
  m_reader.load(L"");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);

  auto primaryExpr = parsePrimaryExpr();
  ASSERT_TRUE(primaryExpr == nullptr);
}

TEST_F(ParserTest, ParserHandlesPrimaryExpr) {
  m_reader.load(L"foo 1 1.0 true \'c\' \"hello\" { foo: 65 } (2) int(1.0)");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);

  auto idExpr = parsePrimaryExpr();
  ASSERT_TRUE(idExpr != nullptr);

  auto integer = parsePrimaryExpr();
  ASSERT_TRUE(integer != nullptr);

  auto floating = parsePrimaryExpr();
  ASSERT_TRUE(floating != nullptr);

  auto boolean = parsePrimaryExpr();
  ASSERT_TRUE(boolean != nullptr);

  auto character = parsePrimaryExpr();
  ASSERT_TRUE(character != nullptr);

  auto string = parsePrimaryExpr();
  ASSERT_TRUE(string != nullptr);

  auto object = parsePrimaryExpr();
  ASSERT_TRUE(object != nullptr);

  auto parenExpr = parsePrimaryExpr();
  ASSERT_TRUE(parenExpr != nullptr);

  auto castExpr = parsePrimaryExpr();
  ASSERT_TRUE(castExpr != nullptr);
}
