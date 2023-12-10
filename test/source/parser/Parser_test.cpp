#include "ParserTest.h"
#include "parser_utils.h"

TEST_F(ParserTest, ParserHandlesEmptyInput) {
  m_reader.load(L"");
  consumeToken();

  EXPECT_CALL(m_errorHandler, exitIfErrors()).Times(1);
  auto program = parseProgram();
  EXPECT_TRUE((int)program->definitions.size() == 0);
}
