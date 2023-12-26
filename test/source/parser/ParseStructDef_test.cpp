#include "ParserTest.h"

/* -------------------------------------------------------------------------- */
/*                                StructMember                                */
/* -------------------------------------------------------------------------- */

TEST_F(ParserTest, ParserHandlesEmptyStructMember) {
  m_reader.load(L"");
  consumeToken();

  auto result = parseStructMember();
  EXPECT_EQ(result, std::nullopt);
}

TEST_F(ParserTest, ParserHandlesStructMember) {
  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  m_reader.load(L"x: int; y : int;");
  consumeToken();

  auto result = parseStructMember();
  ASSERT_TRUE(result != std::nullopt);
  EXPECT_EQ(result->name, Identifier(L"x"));
  EXPECT_EQ(result->type, TypeIdentifier(L"int"));
}

TEST_F(ParserTest, ParserHandlesStructMemberMissingName) {
  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  m_reader.load(L": int; y : int;");
  consumeToken();

  auto result = parseStructMember();
  // Without error recovery we cannot do anything more here
  ASSERT_TRUE(result == std::nullopt);
}

TEST_F(ParserTest, ParserHandlesStructMemberMissingColon) {
  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  m_reader.load(L"x int; y : int;");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::STRUCTMEMBER_EXPECTED_COLON, _)).Times(1);
  auto result = parseStructMember();
  ASSERT_TRUE(result == std::nullopt);
}

TEST_F(ParserTest, ParserHandlesStructMemberMissingType) {
  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  m_reader.load(L"x : ; y : int;");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::STRUCTMEMBER_EXPECTED_TYPE_IDENTIFIER, _))
      .Times(1);
  auto result = parseStructMember();
  ASSERT_TRUE(result == std::nullopt);
}

TEST_F(ParserTest, ParserHandlesStructMemberMissingSemicolon) {
  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  m_reader.load(L"x : int     y : int;}");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::STRUCTMEMBER_EXPECTED_SEMICOLON, _)).Times(1);
  auto result = parseStructMember();
  ASSERT_TRUE(result == std::nullopt);
}

/* -------------------------------------------------------------------------- */
/*                               Struct Members                               */
/* -------------------------------------------------------------------------- */

TEST_F(ParserTest, ParserHandlesEmptyStructMembers) {
  m_reader.load(L"");
  consumeToken();

  auto result = parseStructMembers();
  EXPECT_TRUE(result->empty());
}

TEST_F(ParserTest, ParserHandlesStructMembers) {
  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  m_reader.load(L" x: int; y : int; z : int; }");
  consumeToken();

  auto result = parseStructMembers();
  auto members = result.value();

  ASSERT_TRUE(members.find(L"x") != members.end());
  EXPECT_EQ(members[L"x"].name, Identifier(L"x"));
  EXPECT_EQ(members[L"x"].type, TypeIdentifier(L"int"));

  ASSERT_TRUE(members.find(L"y") != members.end());
  EXPECT_EQ(members[L"y"].name, Identifier(L"y"));
  EXPECT_EQ(members[L"y"].type, TypeIdentifier(L"int"));

  ASSERT_TRUE(members.find(L"z") != members.end());
  EXPECT_EQ(members[L"z"].name, Identifier(L"z"));
  EXPECT_EQ(members[L"z"].type, TypeIdentifier(L"int"));
}

TEST_F(ParserTest, ParserHandlesStructMemberRedefinition) {
  EXPECT_CALL(m_errorHandler, handleError(ErrorType::STRUCTMEMBER_REDEFINITION, _)).Times(1);
  m_reader.load(L"x: int; x : float; }");
  consumeToken();

  auto result = parseStructMembers();
  ASSERT_TRUE(result == std::nullopt);
}

/* -------------------------------------------------------------------------- */
/*                                  StructDef                                 */
/* -------------------------------------------------------------------------- */

TEST_F(ParserTest, ParserHandlesEmptyStructDef) {
  m_reader.load(L"");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  auto result = parseStructDef();
  ASSERT_TRUE(result == nullptr);
}

TEST_F(ParserTest, ParserHandlesStructDef) {
  m_reader.load(L"struct Foo { bar: int; baz : int; qux : int; };");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  auto result = parseStructDef();
  ASSERT_TRUE(result != nullptr);
}

TEST_F(ParserTest, ParserHandlesStructDefMissingName) {
  m_reader.load(L"struct { x: int; }; next");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::STRUCTDEF_EXPECTED_IDENTIFIER, _)).Times(1);
  auto result = parseStructDef();
  ASSERT_TRUE(result == nullptr);
}

TEST_F(ParserTest, ParserHandlesStructDefMissingLBrace) {
  m_reader.load(L"struct Foo x: int; y : int; z : int; }; next");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::STRUCTDEF_EXPECTED_LBRACE, _)).Times(1);
  auto result = parseStructDef();
  ASSERT_TRUE(result == nullptr);
}

TEST_F(ParserTest, ParserHandlesStructDefMissingRBrace) {
  m_reader.load(L"struct Foo { x: int; y : int; z : int; ; next");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::STRUCTDEF_EXPECTED_RBRACE, _)).Times(1);
  auto result = parseStructDef();
  ASSERT_TRUE(result == nullptr);
}

TEST_F(ParserTest, ParserHandlesStructDefMissingSemicolon) {
  m_reader.load(L"struct Foo { x: int; y : int; z : int; }");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::STRUCTDEF_EXPECTED_SEMICOLON, _)).Times(1);
  auto result = parseStructDef();
  ASSERT_TRUE(result == nullptr);
}
