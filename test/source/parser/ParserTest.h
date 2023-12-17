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
  auto parseProgram() { return m_parser.parseProgram(); }

  /* ----------------------------- Utility Methods ---------------------------- */

  auto consumeToken() { return m_parser.consumeToken(); }

  /* ------------------------------- Definitions ------------------------------ */

  auto parseVarDef() { return m_parser.parseVarDef(); }
  auto parseConstDef() { return m_parser.parseConstDef(); }

  auto parseStructDef() { return m_parser.parseStructDef(); }
  auto parseStructMembers() { return m_parser.parseStructMembers(); }
  auto parseStructMember() { return m_parser.parseStructMember(); }

  auto parseVariantDef() { return m_parser.parseVariantDef(); }
  auto parseVariantTypes() { return m_parser.parseVariantTypes(); }
  auto parseVariantType() { return m_parser.parseVariantType(); }

  auto parseFnDef() { return m_parser.parseFnDef(); }
  auto parseFnParams() { return m_parser.parseFnParams(); }
  auto parseFnParam() { return m_parser.parseFnParam(); }
  auto parseFnReturnType() { return m_parser.parseFnReturnType(); }

  /* ------------------------------- Expressions ------------------------------ */
  auto parseExpression() { return m_parser.parseExpression(); }

  auto parsePrimaryExpression() { return m_parser.parsePrimaryExpression(); }
  auto parseIdentifierExpr() { return m_parser.parseIdentifierExpr(); }
  template <typename T>
  auto parseLiteral() {
    return m_parser.parseLiteral<T>();
  }
  auto parseObject() { return m_parser.parseObject(); }
  auto parseObjectMember() { return m_parser.parseObjectMember(); }
  auto parseObjectMembers() { return m_parser.parseObjectMembers(); }
  auto parseParenExpr() { return m_parser.parseParenExpr(); }
  auto parseCastExpr() { return m_parser.parseCastExpr(); }

  auto parseFunctionalExpression() { return m_parser.parseFunctionalExpression(); }
  auto parseFunctionalExpressionPostfix() { return m_parser.parseFunctionalExpressionPostfix(); }
  auto parseFnCallPostfix() { return m_parser.parseFnCallPostfix(); }
  auto parseFnCallArgs() { return m_parser.parseFnCallArgs(); }
  auto parseMemberAccessPostfix() { return m_parser.parseMemberAccessPostfix(); }
  auto parseVariantAccessPostfix() { return m_parser.parseVariantAccessPostfix(); }

  /* ------------------------------- Statements ------------------------------- */

  ::testing::StrictMock<ErrorHandlerMock> m_errorHandler;
  StringCharReader m_reader;
  Lexer m_lexer;
  Parser m_parser;
};
