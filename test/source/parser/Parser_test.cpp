#include <optional>

#include "ParserFixture.h"

using namespace std;

TEST_F(ParserTest, ParserHandlesEmptyInput) {
  m_reader.load(L"");
  m_parser.consumeToken();

  auto program = m_parser.parseProgram();
  EXPECT_TRUE((int)program->definitions.size() == 0);
}

TEST_F(ParserTest, ParserHandlesVarDef) {
  m_reader.load(L"var foo: string = \"bar\";");
  m_parser.consumeToken();

  optional<Program> program = m_parser.parseProgram();
  ASSERT_TRUE(program != nullopt);

  Program::IdentifierToDefinitionPtrMap::iterator it = program->definitions.find(L"foo");
  ASSERT_TRUE(it != program->definitions.end());

  VarDef* varDef = dynamic_cast<VarDef*>(it->second.get());
  ASSERT_TRUE(varDef != nullptr);
  EXPECT_TRUE(varDef->name == L"foo");
  EXPECT_TRUE(varDef->type == L"string");
  EXPECT_TRUE(varDef->value != nullptr);

  EXPECT_TRUE(m_lexer.getNextToken().type == TokenType::ETX);
}
