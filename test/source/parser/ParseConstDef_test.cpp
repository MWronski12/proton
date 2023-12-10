#include "ParserTest.h"

TEST_F(ParserTest, ParserHandlesConstDef) {
  m_reader.load(L"const foo: string = \"bar\";");
  consumeToken();

  auto def = parseConstDef();
  ASSERT_TRUE(def != nullptr);

  ConstDef* varDef = dynamic_cast<ConstDef*>(def.get());
  ASSERT_TRUE(varDef != nullptr);
  EXPECT_TRUE(varDef->name == L"foo");
  EXPECT_TRUE(varDef->type == L"string");
  EXPECT_TRUE(varDef->value != nullptr);
  EXPECT_TRUE(varDef->position.line == FIRST_LINE);
  EXPECT_TRUE(varDef->position.column == 1);
  EXPECT_TRUE(varDef->position.sourceFile == "<custom string>");
}

TEST_F(ParserTest, ParserHandlesConstDefMissingName) {
  m_reader.load(L"const : string = \"bar\";");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::CONSTDEF_EXPECTED_IDENTIFIER, _)).Times(1);
  auto def = parseConstDef();
  ASSERT_EQ(def, nullptr);
}

TEST_F(ParserTest, ParserHandlesConstDefMissingColon) {
  m_reader.load(L"const foo string = \"bar\";");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::CONSTDEF_EXPECTED_COLON, _)).Times(1);
  auto def = parseConstDef();
  ASSERT_EQ(def, nullptr);
}

TEST_F(ParserTest, ParserHandlesConstDefMissingType) {
  m_reader.load(L"const foo : = \"bar\";");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::CONSTDEF_EXPECTED_TYPE_IDENTIFIER, _))
      .Times(1);
  auto def = parseConstDef();
  ASSERT_EQ(def, nullptr);
}

TEST_F(ParserTest, ParserHandlesConstDefMissingAssignment) {
  m_reader.load(L"const foo : string \"bar\";");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::CONSTDEF_EXPECTED_ASSIGNMENT, _)).Times(1);
  auto def = parseConstDef();
  ASSERT_EQ(def, nullptr);
}

TEST_F(ParserTest, ParserHandlesConstDefMissingExpr) {
  m_reader.load(L"const foo : string = ;");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::CONSTDEF_EXPECTED_EXPRESSION, _)).Times(1);
  auto def = parseConstDef();
  ASSERT_EQ(def, nullptr);
}

TEST_F(ParserTest, ParserHandlesConstDefMissingSemicolon) {
  m_reader.load(L"const foo : string = \"bar\"");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::CONSTDEF_EXPECTED_SEMICOLON, _)).Times(1);
  auto def = parseConstDef();
  ASSERT_EQ(def, nullptr);
}
