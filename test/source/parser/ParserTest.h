#include <gtest/gtest.h>

#include "Lexer.h"
#include "Parser.h"
#include "StringCharReader.h"
#include "mocks/ErrorHandlerMock.h"

using namespace std;
using namespace ::testing;

class ParserTest : public ::Test {
 public:
  ParserTest()
      : m_reader{L""},
        m_lexer{m_reader, m_errorHandler},
        m_parser{
            m_lexer,
            m_errorHandler,
        } {}

 protected:
  auto consumeToken() { return m_parser.consumeToken(); }

  auto parseVarDef() { return m_parser.parseVarDef(); }
  auto parseConstDef() { return m_parser.parseConstDef(); }

  auto parseExpression() { return m_parser.parseExpression(); }

  auto parseStructMember() { return m_parser.parseStructMember(); }
  auto parseStructMembers() { return m_parser.parseStructMembers(); }
  auto parseStructDef() { return m_parser.parseStructDef(); }

  auto parseVariantType() { return m_parser.parseVariantType(); }
  auto parseVariantTypes() { return m_parser.parseVariantTypes(); }
  auto parseVariantDef() { return m_parser.parseVariantDef(); }

  auto parseProgram() { return m_parser.parseProgram(); }

  ::testing::StrictMock<ErrorHandlerMock> m_errorHandler;
  StringCharReader m_reader;
  Lexer m_lexer;
  Parser m_parser;
};
