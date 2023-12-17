#include "ParserTest.h"

TEST_F(ParserTest, ParserHandlesProgram) {
  m_reader.load(L"fn main() -> int { return 42; }");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  auto program = parseProgram();
  ASSERT_TRUE(program != std::nullopt);
}

TEST_F(ParserTest, ParserHandlesProgramMissingMain) {
  m_reader.load(L"");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::EXPECTED_MAIN_FUNCTION_DEF, _)).Times(1);
  auto program = parseProgram();
  ASSERT_TRUE(program == std::nullopt);
}

TEST_F(ParserTest, ParserHandlesProgramWithRedefinition) {
  m_reader.load(L"const main: string = \"Entry point\"; fn main() -> int { return 42; }");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::REDEFINITION, _)).Times(1);
  auto program = parseProgram();
  ASSERT_TRUE(program == std::nullopt);
}
