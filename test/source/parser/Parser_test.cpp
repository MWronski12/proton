#include <optional>

#include "ParserFixture.h"

using namespace std;

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
  EXPECT_TRUE(varDef->type == TypeIdentifier::String);
  EXPECT_TRUE(varDef->value != nullptr);
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
  EXPECT_TRUE(constDef->name == L"bar");
  EXPECT_TRUE(constDef->type == TypeIdentifier::Int);
  EXPECT_TRUE(constDef->value != nullptr);
}
