#include "ParserTest.h"

/* -------------------------------------------------------------------------- */
/*                                StructMember                                */
/* -------------------------------------------------------------------------- */

TEST_F(ParserTest, ParserHandlesEmptyStructMember) {
  EXPECT_CALL(m_errorHandler, handleError(ErrorType::STRUCTMEMBER_EXPECTED_IDENTIFIER, _)).Times(1);
  m_reader.load(L"");
  consumeToken();

  auto result = parseStructMember();
  EXPECT_EQ(result, std::nullopt);
  EXPECT_EQ(currentToken().type, TokenType::ETX);
}

TEST_F(ParserTest, ParserHandlesStructMember) {
  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  m_reader.load(L"x: int; y : int;");
  consumeToken();

  auto result = parseStructMember();
  ASSERT_TRUE(result != std::nullopt);
  EXPECT_EQ(result->first, Identifier(L"x"));
  EXPECT_EQ(result->second, TypeIdentifier(L"int"));
  EXPECT_EQ(currentToken().readValue, L"y");
}

TEST_F(ParserTest, ParserHandlesStructMemberMissingName) {
  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  m_reader.load(L": int; y : int;");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::STRUCTMEMBER_EXPECTED_IDENTIFIER, _)).Times(1);
  auto result = parseStructMember();
  ASSERT_TRUE(result == std::nullopt);
  EXPECT_EQ(currentToken().readValue, L"y");
}

TEST_F(ParserTest, ParserHandlesStructMemberMissingColon) {
  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  m_reader.load(L"x int; y : int;");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::STRUCTMEMBER_EXPECTED_COLON, _)).Times(1);
  auto result = parseStructMember();
  ASSERT_TRUE(result == std::nullopt);
  EXPECT_EQ(currentToken().readValue, L"y");
}

TEST_F(ParserTest, ParserHandlesStructMemberMissingType) {
  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  m_reader.load(L"x : ; y : int;");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::STRUCTMEMBER_EXPECTED_TYPE_IDENTIFIER, _))
      .Times(1);
  auto result = parseStructMember();
  ASSERT_TRUE(result == std::nullopt);
  EXPECT_EQ(currentToken().readValue, L"y");
}

TEST_F(ParserTest, ParserHandlesStructMemberMissingSemicolon) {
  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  m_reader.load(L"x : int     y : int;}");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::STRUCTMEMBER_EXPECTED_SEMICOLON, _)).Times(1);
  auto result = parseStructMember();
  ASSERT_TRUE(result == std::nullopt);
  // From parsing struct member we recover by skipping passed the next semicolon
  EXPECT_EQ(currentToken().readValue, L"}");
}

/* -------------------------------------------------------------------------- */
/*                               Struct Members                               */
/* -------------------------------------------------------------------------- */

TEST_F(ParserTest, ParserHandlesEmptyStructMembers) {
  EXPECT_CALL(m_errorHandler, handleError(ErrorType::STRUCTMEMBER_EXPECTED_IDENTIFIER, _)).Times(1);
  m_reader.load(L"");
  consumeToken();

  auto result = parseStructMembers().value();
  EXPECT_TRUE(result.empty());
  EXPECT_EQ(currentToken().type, TokenType::ETX);
}

TEST_F(ParserTest, ParserHandlesStructMembers) {
  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  m_reader.load(L" x: int; y : int; z : int; }");
  consumeToken();

  auto result = parseStructMembers().value();
  EXPECT_EQ(result.size(), 3);
  EXPECT_EQ(result.at(0).first, Identifier(L"x"));
  EXPECT_EQ(result.at(0).second, TypeIdentifier(L"int"));
  EXPECT_EQ(result.at(1).first, Identifier(L"y"));
  EXPECT_EQ(result.at(1).second, TypeIdentifier(L"int"));
  EXPECT_EQ(result.at(2).first, Identifier(L"z"));
  EXPECT_EQ(result.at(2).second, TypeIdentifier(L"int"));
  EXPECT_EQ(currentToken().readValue, L"}");
}

/* -------------------------------------------------------------------------- */
/*                                  StructDef                                 */
/* -------------------------------------------------------------------------- */

TEST_F(ParserTest, ParserHandlesEmptyStructDef) {
  m_reader.load(L"");
  consumeToken();
  EXPECT_ANY_THROW(parseStructDef());
}

TEST_F(ParserTest, ParserHandlesStructDefMissingName) {
  m_reader.load(L"struct { x: int; }; next");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::STRUCTDEF_EXPECTED_IDENTIFIER, _)).Times(1);
  auto result = parseStructDef();
  ASSERT_TRUE(result == nullptr);
  EXPECT_EQ(currentToken().readValue, L"next");
}

TEST_F(ParserTest, ParserHandlesStructDefMissingLBrace) {
  m_reader.load(L"struct Foo x: int; y : int; z : int; }; next");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::STRUCTDEF_EXPECTED_LBRACE, _)).Times(1);
  auto result = parseStructDef();
  ASSERT_TRUE(result == nullptr);
  EXPECT_EQ(currentToken().readValue, L"next");
}

TEST_F(ParserTest, ParserHandlesStructDefMissingRBrace) {
  m_reader.load(L"struct Foo { x: int; y : int; z : int; ; next");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::STRUCTDEF_EXPECTED_RBRACE, _)).Times(1);
  auto result = parseStructDef();
  ASSERT_TRUE(result == nullptr);
  // From parsing struct def we recover by skipping passed "};" which is StructDef delimitter
  EXPECT_EQ(currentToken().type, TokenType::ETX);
}
