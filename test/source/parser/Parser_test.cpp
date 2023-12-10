#include <optional>

#include "ParserFixture.h"
#include "parser_utils.h"

using namespace std;
using namespace ::testing;

TEST_F(ParserTest, ParserHandlesEmptyInput) {
  m_reader.load(L"");

  auto program = m_parser.parseProgram();
  EXPECT_TRUE((int)program->definitions.size() == 0);
}

/* -------------------------------------------------------------------------- */
/*                                   VarDef                                   */
/* -------------------------------------------------------------------------- */

TEST_F(ParserTest, ParserHandlesVarDef) {
  m_reader.load(L"var foo: string = \"bar\";");

  optional<Program> program = m_parser.parseProgram();
  ASSERT_TRUE(program != nullopt);

  Program::IdentifierToDefinitionPtrMap::iterator it = program->definitions.find(L"foo");
  ASSERT_TRUE(it != program->definitions.end());

  VarDef* varDef = dynamic_cast<VarDef*>(it->second.get());
  ASSERT_TRUE(varDef != nullptr);
  EXPECT_TRUE(varDef->name == L"foo");
  EXPECT_TRUE(varDef->type == L"string");
  EXPECT_TRUE(varDef->value != nullptr);
  EXPECT_TRUE(varDef->position.line == FIRST_LINE);
  EXPECT_TRUE(varDef->position.column == FIRST_COL);
  EXPECT_TRUE(varDef->position.sourceFile == "<custom string>");
}

TEST_F(ParserTest, ParserHandlesVarDefMissingName) {
  m_reader.load(L"var : string = \"bar\";");
  EXPECT_CALL(m_errorHandler, handleError(ErrorType::ERROR_PARSING_VARDEF, _)).Times(1);
  EXPECT_CALL(m_errorHandler, handleError(ErrorType::VARDEF_EXPECTED_IDENTIFIER, _)).Times(1);
  auto program = m_parser.parseProgram();
  ASSERT_EQ(program, nullopt);
}

TEST_F(ParserTest, ParserHandlesVarDefMissingColon) {
  m_reader.load(L"var foo string = \"bar\";");
  EXPECT_CALL(m_errorHandler, handleError(ErrorType::ERROR_PARSING_VARDEF, _)).Times(1);
  EXPECT_CALL(m_errorHandler, handleError(ErrorType::VARDEF_EXPECTED_COLON, _)).Times(1);
  auto program = m_parser.parseProgram();
  ASSERT_EQ(program, nullopt);
}

TEST_F(ParserTest, ParserHandlesVarDefMissingType) {
  m_reader.load(L"var foo : = \"bar\";");
  EXPECT_CALL(m_errorHandler, handleError(ErrorType::ERROR_PARSING_VARDEF, _)).Times(1);
  EXPECT_CALL(m_errorHandler, handleError(ErrorType::VARDEF_EXPECTED_TYPE_IDENTIFIER, _)).Times(1);
  auto program = m_parser.parseProgram();
  ASSERT_EQ(program, nullopt);
}

TEST_F(ParserTest, ParserHandlesVarDefMissingAssignment) {
  m_reader.load(L"var foo : string \"bar\";");
  EXPECT_CALL(m_errorHandler, handleError(ErrorType::ERROR_PARSING_VARDEF, _)).Times(1);
  EXPECT_CALL(m_errorHandler, handleError(ErrorType::VARDEF_EXPECTED_ASSIGNMENT, _)).Times(1);
  auto program = m_parser.parseProgram();
  ASSERT_EQ(program, nullopt);
}

TEST_F(ParserTest, ParserHandlesVarDefMissingExpr) {
  m_reader.load(L"var foo : string = ;");
  EXPECT_CALL(m_errorHandler, handleError(ErrorType::ERROR_PARSING_VARDEF, _)).Times(1);
  EXPECT_CALL(m_errorHandler, handleError(ErrorType::VARDEF_EXPECTED_EXPRESSION, _)).Times(1);
  auto program = m_parser.parseProgram();
  ASSERT_EQ(program, nullopt);
}

TEST_F(ParserTest, ParserHandlesVarDefMissingSemicolon) {
  m_reader.load(L"var foo : string = \"bar\"");
  EXPECT_CALL(m_errorHandler, handleError(ErrorType::ERROR_PARSING_VARDEF, _)).Times(1);
  EXPECT_CALL(m_errorHandler, handleError(ErrorType::VARDEF_EXPECTED_SEMICOLON, _)).Times(1);
  auto program = m_parser.parseProgram();
  ASSERT_EQ(program, nullopt);
}

/* -------------------------------------------------------------------------- */
/*                                  ConstDef                                  */
/* -------------------------------------------------------------------------- */

TEST_F(ParserTest, ParserHandlesConstDef) {
  m_reader.load(L"const timeLeft: int = \"0\";");

  optional<Program> program = m_parser.parseProgram();
  ASSERT_TRUE(program != nullopt);

  Program::IdentifierToDefinitionPtrMap::iterator it = program->definitions.find(L"timeLeft");
  ASSERT_TRUE(it != program->definitions.end());

  ConstDef* constDef = dynamic_cast<ConstDef*>(it->second.get());
  ASSERT_TRUE(constDef != nullptr);
  EXPECT_TRUE(constDef->name == L"timeLeft");
  EXPECT_TRUE(constDef->type == L"int");
  EXPECT_TRUE(constDef->value != nullptr);
  EXPECT_TRUE(constDef->position.line == FIRST_LINE);
  EXPECT_TRUE(constDef->position.column == FIRST_COL);
  EXPECT_TRUE(constDef->position.sourceFile == "<custom string>");
}

TEST_F(ParserTest, ParserHandlesConstDefMissingName) {
  m_reader.load(L"const : string = \"bar\";");
  EXPECT_CALL(m_errorHandler, handleError(ErrorType::ERROR_PARSING_CONSTDEF, _)).Times(1);
  EXPECT_CALL(m_errorHandler, handleError(ErrorType::CONSTDEF_EXPECTED_IDENTIFIER, _)).Times(1);
  auto program = m_parser.parseProgram();
  ASSERT_EQ(program, nullopt);
}

TEST_F(ParserTest, ParserHandlesConstDefMissingColon) {
  m_reader.load(L"const foo string = \"bar\";");
  EXPECT_CALL(m_errorHandler, handleError(ErrorType::ERROR_PARSING_CONSTDEF, _)).Times(1);
  EXPECT_CALL(m_errorHandler, handleError(ErrorType::CONSTDEF_EXPECTED_COLON, _)).Times(1);
  auto program = m_parser.parseProgram();
  ASSERT_EQ(program, nullopt);
}

TEST_F(ParserTest, ParserHandlesConstDefMissingType) {
  m_reader.load(L"const foo : = \"bar\";");
  EXPECT_CALL(m_errorHandler, handleError(ErrorType::ERROR_PARSING_CONSTDEF, _)).Times(1);
  EXPECT_CALL(m_errorHandler, handleError(ErrorType::CONSTDEF_EXPECTED_TYPE_IDENTIFIER, _))
      .Times(1);
  auto program = m_parser.parseProgram();
  ASSERT_EQ(program, nullopt);
}

TEST_F(ParserTest, ParserHandlesConstDefMissingAssignment) {
  m_reader.load(L"const foo : string \"bar\";");
  EXPECT_CALL(m_errorHandler, handleError(ErrorType::ERROR_PARSING_CONSTDEF, _)).Times(1);
  EXPECT_CALL(m_errorHandler, handleError(ErrorType::CONSTDEF_EXPECTED_ASSIGNMENT, _)).Times(1);
  auto program = m_parser.parseProgram();
  ASSERT_EQ(program, nullopt);
}

TEST_F(ParserTest, ParserHandlesConstDefMissingExpr) {
  m_reader.load(L"const foo : string = ;");
  EXPECT_CALL(m_errorHandler, handleError(ErrorType::ERROR_PARSING_CONSTDEF, _)).Times(1);
  EXPECT_CALL(m_errorHandler, handleError(ErrorType::CONSTDEF_EXPECTED_EXPRESSION, _)).Times(1);
  auto program = m_parser.parseProgram();
  ASSERT_EQ(program, nullopt);
}

TEST_F(ParserTest, ParserHandlesConstDefMissingSemicolon) {
  m_reader.load(L"const foo : string = \"bar\"");
  EXPECT_CALL(m_errorHandler, handleError(ErrorType::ERROR_PARSING_CONSTDEF, _)).Times(1);
  EXPECT_CALL(m_errorHandler, handleError(ErrorType::CONSTDEF_EXPECTED_SEMICOLON, _)).Times(1);
  auto program = m_parser.parseProgram();
  ASSERT_EQ(program, nullopt);
}
