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

  /* ------------------------------- Expressions ------------------------------ */
  auto parseExpression() { return m_parser.parseExpression(); }

  auto parseIdentifierExpr() { return m_parser.parseIdentifierExpr(); }
  auto parsePrimaryExpr() { return m_parser.parsePrimaryExpr(); }
  template <typename T>
  auto parseLiteral() {
    return m_parser.parseLiteral<T>();
  }
  auto parseObject() { return m_parser.parseObject(); }
  auto parseObjectMember() { return m_parser.parseObjectMember(); }
  auto parseObjectMembers() { return m_parser.parseObjectMembers(); }
  auto parseParenExpr() { return m_parser.parseParenExpr(); }
  auto parseCastExpr() { return m_parser.parseCastExpr(); }

  /* ------------------------------- Definitions ------------------------------ */
  auto parseVarDef() { return m_parser.parseVarDef(); }
  auto parseConstDef() { return m_parser.parseConstDef(); }

  auto parseStructMember() { return m_parser.parseStructMember(); }
  auto parseStructMembers() { return m_parser.parseStructMembers(); }
  auto parseStructDef() { return m_parser.parseStructDef(); }

  auto parseVariantType() { return m_parser.parseVariantType(); }
  auto parseVariantTypes() { return m_parser.parseVariantTypes(); }
  auto parseVariantDef() { return m_parser.parseVariantDef(); }

  auto parseFnReturnType() { return m_parser.parseFnReturnType(); }
  auto parseFnParam() { return m_parser.parseFnParam(); }
  auto parseFnParams() { return m_parser.parseFnParams(); }
  auto parseFnDef() { return m_parser.parseFnDef(); }

  /* ------------------------------- Statements ------------------------------- */

  auto parseProgram() { return m_parser.parseProgram(); }

  ::testing::StrictMock<ErrorHandlerMock> m_errorHandler;
  StringCharReader m_reader;
  Lexer m_lexer;
  Parser m_parser;
};
