#include <gtest/gtest.h>

#include "Lexer.h"
#include "Parser.h"
#include "StringCharReader.h"
#include "mocks/ErrorHandlerMock.h"

class ParserTest : public ::testing::Test {
 public:
  ParserTest()
      : m_reader{L""},
        m_lexer{m_reader, m_errorHandler},
        m_parser{
            m_lexer,
            m_errorHandler,
        } {}

  void SetUp() override {
    // Should be called each time we finish parsing program
    EXPECT_CALL(m_errorHandler, exitIfErrors()).Times(1);
  }

 protected:
  ::testing::StrictMock<ErrorHandlerMock> m_errorHandler;
  StringCharReader m_reader;
  Lexer m_lexer;
  Parser m_parser;
};
