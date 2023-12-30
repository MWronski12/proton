#include "ParserTest.h"

/* -------------------------------------------------------------------------- */
/*                                   FnParam                                  */
/* -------------------------------------------------------------------------- */

TEST_F(ParserTest, ParserHandlesEmptyFnParam) {
  m_reader.load(L"");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  auto result = parseFnParam();
  EXPECT_EQ(result, nullptr);
}

TEST_F(ParserTest, ParserHandlesFnParam) {
  m_reader.load(L"x : int");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  auto result = parseFnParam();
  ASSERT_TRUE(result != nullptr);
  EXPECT_EQ(result->isConst, false);
  EXPECT_EQ(result->name, Identifier(L"x"));
  EXPECT_EQ(result->type, TypeIdentifier(L"int"));
}

TEST_F(ParserTest, ParserHandlesConstFnParam) {
  m_reader.load(L"const x : int");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  auto result = parseFnParam();
  ASSERT_TRUE(result != nullptr);
  EXPECT_EQ(result->isConst, true);
  EXPECT_EQ(result->name, Identifier(L"x"));
  EXPECT_EQ(result->type, TypeIdentifier(L"int"));
}

TEST_F(ParserTest, ParserHandlesMissingIdentifierInFnParam) {
  m_reader.load(L"const : int");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::FNPARAM_EXPECTED_IDENTIFIER, _)).Times(1);
  auto result = parseFnParam();
  ASSERT_TRUE(result == nullptr);
}

TEST_F(ParserTest, ParserHandlesMissingColonInFnParam) {
  m_reader.load(L"const x int");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::FNPARAM_EXPECTED_COLON, _)).Times(1);
  auto result = parseFnParam();
  ASSERT_TRUE(result == nullptr);
}

TEST_F(ParserTest, ParserHandlesMissingTypeIdentifierInFnParam) {
  m_reader.load(L"const x : ");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::FNPARAM_EXPECTED_TYPE_IDENTIFIER, _)).Times(1);
  auto result = parseFnParam();
  ASSERT_TRUE(result == nullptr);
}

/* -------------------------------------------------------------------------- */
/*                                  FnParams                                  */
/* -------------------------------------------------------------------------- */

TEST_F(ParserTest, ParserHandlesEmptyFnParams) {
  m_reader.load(L"");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  auto result = parseFnParams();
  EXPECT_EQ(result->empty(), true);
}

TEST_F(ParserTest, ParserHandlesFnParams) {
  m_reader.load(L"x : int, const y : int, z : int");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  auto result = parseFnParams();
  ASSERT_EQ(result->size(), 3);
  EXPECT_EQ(result->at(L"x").isConst, false);
  EXPECT_EQ(result->at(L"x").name, Identifier(L"x"));
  EXPECT_EQ(result->at(L"x").type, TypeIdentifier(L"int"));
  EXPECT_EQ(result->at(L"y").isConst, true);
  EXPECT_EQ(result->at(L"y").name, Identifier(L"y"));
  EXPECT_EQ(result->at(L"y").type, TypeIdentifier(L"int"));
  EXPECT_EQ(result->at(L"z").isConst, false);
  EXPECT_EQ(result->at(L"z").name, Identifier(L"z"));
  EXPECT_EQ(result->at(L"z").type, TypeIdentifier(L"int"));
}

TEST_F(ParserTest, ParserHandlesFnParamRedefinition) {
  m_reader.load(L"const x : int, const x : int");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::FNPARAM_REDEFINITION, _)).Times(1);
  auto result = parseFnParams();
  ASSERT_TRUE(result == std::nullopt);
}

/* -------------------------------------------------------------------------- */
/*                                    FnDef                                   */
/* -------------------------------------------------------------------------- */

TEST_F(ParserTest, ParserHandlesEmptyFnDef) {
  m_reader.load(L"");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  auto result = parseFnDef();
  EXPECT_EQ(result, nullptr);
}

TEST_F(ParserTest, ParserHandlesEmptyParamsFnDef) {
  m_reader.load(L"fn foo() -> int { }");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  auto result = parseFnDef();
  ASSERT_TRUE(result != nullptr);
  FnDef* fnDef = dynamic_cast<FnDef*>(result.get());
  ASSERT_TRUE(fnDef != nullptr);
  EXPECT_EQ(fnDef->name, Identifier(L"foo"));
  EXPECT_EQ(fnDef->parameters.size(), 0);
  EXPECT_EQ(fnDef->returnType, TypeIdentifier(L"int"));
}

TEST_F(ParserTest, ParserHandlesFnDef) {
  m_reader.load(L"fn foo(const boo: string) -> int { }");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  auto result = parseFnDef();
  ASSERT_TRUE(result != nullptr);
  FnDef* fnDef = dynamic_cast<FnDef*>(result.get());
  ASSERT_TRUE(fnDef != nullptr);
  EXPECT_EQ(fnDef->name, Identifier(L"foo"));
  ASSERT_TRUE(fnDef->parameters.size() == 1);
  EXPECT_EQ(fnDef->returnType, TypeIdentifier(L"int"));
}

TEST_F(ParserTest, ParserHandlesFnDefMissingIdentifier) {
  m_reader.load(L"fn () -> int { return 42; }");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::FNDEF_EXPECTED_IDENTIFIER, _)).Times(1);
  auto result = parseFnDef();
  EXPECT_EQ(result, nullptr);
}

TEST_F(ParserTest, ParserHandlesFnDefMissingLPar) {
  m_reader.load(L"fn foo ) -> int { return 42; }");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::FNDEF_EXPECTED_LPAREN, _)).Times(1);
  auto result = parseFnDef();
  EXPECT_EQ(result, nullptr);
}

TEST_F(ParserTest, ParserHandlesFnDefMissingRPar) {
  m_reader.load(L"fn foo ( -> int { return 42; }");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::FNDEF_EXPECTED_RPAREN, _)).Times(1);
  auto result = parseFnDef();
  EXPECT_EQ(result, nullptr);
}

TEST_F(ParserTest, ParserHandlesFnDefMissingArrow) {
  m_reader.load(L"fn foo () int { return 42; }");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::FNDEF_EXPECTED_ARROW, _)).Times(1);
  auto result = parseFnDef();
  ASSERT_TRUE(result == nullptr);
}

TEST_F(ParserTest, ParserHandlesFnDefMissingReturnType) {
  m_reader.load(L"fn foo () -> { return 42; }");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::FNDEF_EXPECTED_RETURN_TYPE, _)).Times(1);
  auto result = parseFnDef();
  ASSERT_TRUE(result == nullptr);
}

TEST_F(ParserTest, ParserHandlesFnDefMissingBlock) {
  m_reader.load(L"fn foo () -> int");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::FNDEF_EXPECTED_BLOCK, _)).Times(1);
  auto result = parseFnDef();
  EXPECT_EQ(result, nullptr);
}
