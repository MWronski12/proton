#include <iostream>

#include "ParserTest.h"
TEST_F(ParserTest, ParserHandlesVarDef) {
  std::cout << m_reader.pos().column << std::endl;
  m_reader.load(L"var foo: string = \"bar\";");
  std::cout << m_reader.pos().column << std::endl;
  consumeToken();
  std::cout << m_reader.pos().column << std::endl;

  auto def = parseVarDef();
  ASSERT_TRUE(def != nullptr);

  VarDef* varDef = dynamic_cast<VarDef*>(def.get());
  ASSERT_TRUE(varDef != nullptr);
  EXPECT_TRUE(varDef->name == L"foo");
  EXPECT_TRUE(varDef->type == L"string");
  EXPECT_TRUE(varDef->value != nullptr);
  EXPECT_EQ(varDef->position.line, FIRST_LINE);
  EXPECT_EQ(varDef->position.column, FIRST_COL);
  EXPECT_TRUE(varDef->position.sourceFile == "<custom string>");
}

TEST_F(ParserTest, ParserHandlesVarDefMissingName) {
  m_reader.load(L"var : string = \"bar\";");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::VARDEF_EXPECTED_IDENTIFIER, _)).Times(1);
  auto def = parseVarDef();
  ASSERT_EQ(def, nullptr);
}

TEST_F(ParserTest, ParserHandlesVarDefMissingColon) {
  m_reader.load(L"var foo string = \"bar\";");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::VARDEF_EXPECTED_COLON, _)).Times(1);
  auto def = parseVarDef();
  ASSERT_EQ(def, nullptr);
}

TEST_F(ParserTest, ParserHandlesVarDefMissingType) {
  m_reader.load(L"var foo : = \"bar\";");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::VARDEF_EXPECTED_TYPE_IDENTIFIER, _)).Times(1);
  auto def = parseVarDef();
  ASSERT_EQ(def, nullptr);
}

TEST_F(ParserTest, ParserHandlesVarDefMissingAssignment) {
  m_reader.load(L"var foo : string \"bar\";");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::VARDEF_EXPECTED_ASSIGNMENT, _)).Times(1);
  auto def = parseVarDef();
  ASSERT_EQ(def, nullptr);
}

TEST_F(ParserTest, ParserHandlesVarDefMissingExpr) {
  m_reader.load(L"var foo : string = ;");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::VARDEF_EXPECTED_EXPRESSION, _)).Times(1);
  auto def = parseVarDef();
  ASSERT_EQ(def, nullptr);
}

TEST_F(ParserTest, ParserHandlesVarDefMissingSemicolon) {
  m_reader.load(L"var foo : string = \"bar\"");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::VARDEF_EXPECTED_SEMICOLON, _)).Times(1);
  auto def = parseVarDef();
  ASSERT_EQ(def, nullptr);
}
