#include <gtest/gtest.h>

#include "Lexer.h"
#include "Parser.h"
#include "StringCharReader.h"
#include "mocks/ErrorHandlerMock.h"

class ParserTest : public ::testing::Test {
 public:
  ParserTest() : m_lexer{m_reader, m_errorHandler}, m_parser{m_lexer, m_errorHandler} {}

 protected:
  ErrorHandlerMock m_errorHandler;
  StringCharReader m_reader;
  Lexer m_lexer;
  Parser m_parser;
};
