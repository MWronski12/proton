#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <iostream>

#include "Lexer.h"
#include "StringCharReader.h"
#include "mocks/ErrorHandlerMock.h"

using namespace ::testing;

class LexerTest : public ::Test {
 public:
  LexerTest() : m_lexer{m_reader, m_errorHandler} {}

 protected:
  NiceMock<ErrorHandlerMock> m_errorHandler;
  StringCharReader m_reader;
  Lexer m_lexer;
};

TEST(TokenType, TokenTypeConfigurationsAreCorrect) {
  ASSERT_EQ((int)TokenType::UNEXPECTED, 0);
  ASSERT_EQ((int)TokenType::ETX, TOKEN_TYPE_NAMES.size() - 1);
}

/* -------------------------------------------------------------------------- */
/*                                SPECIAL CASES                               */
/* -------------------------------------------------------------------------- */

TEST_F(LexerTest, LexerHandlesEmptyInput) {
  m_reader.load(L"");
  auto token = m_lexer.getNextToken();
  EXPECT_EQ(token.type, TokenType::ETX);
  EXPECT_EQ(token.value.front(), WEOF);

  token = m_lexer.getNextToken();
  EXPECT_EQ(token.type, TokenType::ETX);
  EXPECT_EQ(token.value.front(), WEOF);
}

TEST_F(LexerTest, LexerHandlesWhitespaces) {
  m_reader.load(L"  \t  \n  void  \n  \t  ");
  auto token = m_lexer.getNextToken();
  EXPECT_EQ(token.type, TokenType::VOID_KWRD);
  EXPECT_EQ(token.value, L"void");

  token = m_lexer.getNextToken();
  EXPECT_EQ(token.type, TokenType::ETX);
  EXPECT_EQ(token.value.front(), WEOF);
}

TEST_F(LexerTest, LexerHandlesUnexpectedCharacters) {
  m_reader.load(L"\\ # ? `");

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::UNEXPECTED_CHARACTER, _, _)).Times(4);

  auto token = m_lexer.getNextToken();
  EXPECT_EQ(token.type, TokenType::UNEXPECTED);
  EXPECT_EQ(token.value, L"\\");

  token = m_lexer.getNextToken();
  EXPECT_EQ(token.type, TokenType::UNEXPECTED);
  EXPECT_EQ(token.value, L"#");

  token = m_lexer.getNextToken();
  EXPECT_EQ(token.type, TokenType::UNEXPECTED);
  EXPECT_EQ(token.value, L"?");

  token = m_lexer.getNextToken();
  EXPECT_EQ(token.type, TokenType::UNEXPECTED);
  EXPECT_EQ(token.value, L"`");
}

/* -------------------------------------------------------------------------- */
/*                                  KEYWORDS                                  */
/* -------------------------------------------------------------------------- */

TEST_F(LexerTest, LexerHandlesAllKeywords) {
  std::wstringstream wss;

  for (auto keyword : KEYWORDS) {
    wss << keyword << L" ";
  }

  m_reader.load(wss.str());
  std::wcout << wss.str() << std::endl;

  for (size_t i = 0; i < KEYWORDS.size(); i++) {
    auto token = m_lexer.getNextToken();
    EXPECT_EQ((int)token.type, i + KEYWORDS_OFFSET);
    EXPECT_EQ(token.value, KEYWORDS[i]);
  }
}

/* -------------------------------------------------------------------------- */
/*                                  OPERATORS                                 */
/* -------------------------------------------------------------------------- */

TEST_F(LexerTest, LexerHandlesAllOperators) {
  std::wstringstream wss;

  for (auto op : OPERATORS) {
    wss << op << L" ";
  }

  m_reader.load(wss.str());
  std::wcout << wss.str() << std::endl;

  for (size_t i = 0; i < OPERATORS.size(); i++) {
    auto token = m_lexer.getNextToken();
    EXPECT_EQ(token.type, static_cast<TokenType>(i + OPERATORS_OFFSET));
    EXPECT_EQ(token.value, OPERATORS[i]);
  }
}

/* -------------------------------------------------------------------------- */
/*                                 PUNCTUATION                                */
/* -------------------------------------------------------------------------- */

TEST_F(LexerTest, LexerHandlesAllPunctuation) {
  std::wstringstream wss;

  for (auto punc : PUNCTUATION) {
    wss << punc << L" ";
  }

  m_reader.load(wss.str());
  std::wcout << wss.str() << std::endl;

  for (size_t i = 0; i < PUNCTUATION.size(); i++) {
    auto token = m_lexer.getNextToken();
    EXPECT_EQ(token.type, static_cast<TokenType>(i + PUNCTUATION_OFFSET));
    EXPECT_EQ(token.value, PUNCTUATION[i]);
  }
}

/* -------------------------------------------------------------------------- */
/*                                  INTEGERS                                  */
/* -------------------------------------------------------------------------- */

TEST_F(LexerTest, LexerHandlesValidIntLiterals) {
  m_reader.load(L"1234567890 0 666");

  auto token = m_lexer.getNextToken();
  EXPECT_EQ(token.type, TokenType::INTEGER);
  EXPECT_EQ(token.value, L"1234567890");
  EXPECT_EQ(token.intValue, 1234567890);

  token = m_lexer.getNextToken();
  EXPECT_EQ(token.type, TokenType::INTEGER);
  EXPECT_EQ(token.value, L"0");
  EXPECT_EQ(token.intValue, 0);

  token = m_lexer.getNextToken();
  EXPECT_EQ(token.type, TokenType::INTEGER);
  EXPECT_EQ(token.value, L"666");
  EXPECT_EQ(token.intValue, 666);
}

TEST_F(LexerTest, LexerHandlesInvalidIntLiterals) {
  m_reader.load(L"3x 5( 01");

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::INVALID_NUMBER_LITERAL, _, _)).Times(3);

  auto token = m_lexer.getNextToken();
  EXPECT_EQ(token.type, TokenType::UNEXPECTED);
  EXPECT_EQ(token.value, L"3x");

  token = m_lexer.getNextToken();
  EXPECT_EQ(token.type, TokenType::UNEXPECTED);
  EXPECT_EQ(token.value, L"5(");

  token = m_lexer.getNextToken();
  EXPECT_EQ(token.type, TokenType::UNEXPECTED);
  EXPECT_EQ(token.value, L"01");
}

/* -------------------------------------------------------------------------- */
/*                                   FLOATS                                   */
/* -------------------------------------------------------------------------- */

TEST_F(LexerTest, LexerHandlesValidFloatLiterals) {
  m_reader.load(L"123.456 0.0 0. 1.290");

  auto token = m_lexer.getNextToken();
  EXPECT_EQ(token.type, TokenType::FLOAT);
  EXPECT_EQ(token.value, L"123.456");
  EXPECT_NEAR(token.floatValue.value(), 123.456, 1e-5);

  token = m_lexer.getNextToken();
  EXPECT_EQ(token.type, TokenType::FLOAT);
  EXPECT_EQ(token.value, L"0.0");
  EXPECT_NEAR(token.floatValue.value(), 0.0, 1e-5);

  token = m_lexer.getNextToken();
  EXPECT_EQ(token.type, TokenType::FLOAT);
  EXPECT_EQ(token.value, L"0.");
  EXPECT_NEAR(token.floatValue.value(), 0.0, 1e-5);

  token = m_lexer.getNextToken();
  EXPECT_EQ(token.type, TokenType::FLOAT);
  EXPECT_EQ(token.value, L"1.290");
  EXPECT_NEAR(token.floatValue.value(), 1.29, 1e-5);
}

TEST_F(LexerTest, LexerHandlesInvalidFloatLiterals) {
  m_reader.load(L"00.0 00. 0.2x 3.14# 0.0.0.0");

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::INVALID_NUMBER_LITERAL, _, _)).Times(5);

  auto token = m_lexer.getNextToken();
  EXPECT_EQ(token.type, TokenType::UNEXPECTED);
  EXPECT_EQ(token.value, L"00.0");

  token = m_lexer.getNextToken();
  EXPECT_EQ(token.type, TokenType::UNEXPECTED);
  EXPECT_EQ(token.value, L"00.");

  token = m_lexer.getNextToken();
  EXPECT_EQ(token.type, TokenType::UNEXPECTED);
  EXPECT_EQ(token.value, L"0.2x");

  token = m_lexer.getNextToken();
  EXPECT_EQ(token.type, TokenType::UNEXPECTED);
  EXPECT_EQ(token.value, L"3.14#");

  token = m_lexer.getNextToken();
  EXPECT_EQ(token.type, TokenType::UNEXPECTED);
  EXPECT_EQ(token.value, L"0.0.0.0");
}

/* -------------------------------------------------------------------------- */
/*                                    CHARS                                   */
/* -------------------------------------------------------------------------- */

TEST_F(LexerTest, LexerHandlesCharLiterals) {
  m_reader.load(L"\'X\'");
  auto token = m_lexer.getNextToken();
  EXPECT_EQ(token.type, TokenType::CHAR);
  EXPECT_EQ(token.value, L"X");
  EXPECT_EQ(token.charValue, L'X');

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::INVALID_CHAR_LITERAL, _, _)).Times(1);
  m_reader.load(L"\'XXX\'");  // Multiple character char literal
  token = m_lexer.getNextToken();
  EXPECT_EQ(token.type, TokenType::UNEXPECTED);
  EXPECT_EQ(token.value, L"XXX");

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::MISSING_CLOSING_QUOTE, _, _)).Times(1);
  m_reader.load(L"\'c");  // WEOF before closing quote
  token = m_lexer.getNextToken();
  EXPECT_EQ(token.type, TokenType::UNEXPECTED);
  EXPECT_EQ(token.value, L"c");

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::MISSING_CLOSING_QUOTE, _, _)).Times(1);
  m_reader.load(L"\'s\n\'");  // Newline before closing quote
  token = m_lexer.getNextToken();
  EXPECT_EQ(token.type, TokenType::UNEXPECTED);
  EXPECT_EQ(token.value, L"s");
}

TEST_F(LexerTest, LexerHandlesEscapeSequencesInCharLiterals) {
  m_reader.load(L"\'\\\"\'");
  auto token = m_lexer.getNextToken();
  EXPECT_EQ(token.type, TokenType::CHAR);
  EXPECT_EQ(token.value, L"\"");
  EXPECT_EQ(token.charValue.value(), L'\"');
}

/* -------------------------------------------------------------------------- */
/*                                  BOOLEANS                                  */
/* -------------------------------------------------------------------------- */

TEST_F(LexerTest, LexerHandlesBooleanLiterals) {
  m_reader.load(L"true false");

  auto token = m_lexer.getNextToken();
  EXPECT_EQ(token.type, TokenType::TRUE_KWRD);
  EXPECT_EQ(token.value, L"true");

  token = m_lexer.getNextToken();
  EXPECT_EQ(token.type, TokenType::FALSE_KWRD);
  EXPECT_EQ(token.value, L"false");
}

/* -------------------------------------------------------------------------- */
/*                                   STRINGS                                  */
/* -------------------------------------------------------------------------- */

TEST_F(LexerTest, LexerHandlesStringLiterals) {
  m_reader.load(L"\"Hello World!\"");
  auto token = m_lexer.getNextToken();
  EXPECT_EQ(token.type, TokenType::STRING);
  EXPECT_EQ(token.value, L"Hello World!");
  EXPECT_EQ(token.strValue.value(), L"Hello World!");

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::MISSING_CLOSING_QUOTE, _, _)).Times(2);

  m_reader.load(L"\" hello ");  // WEOF before closing quote
  token = m_lexer.getNextToken();
  EXPECT_EQ(token.type, TokenType::UNEXPECTED);
  EXPECT_EQ(token.value, L" hello ");

  m_reader.load(L"\" first line \n second line \"");  // Newline before closing quote
  token = m_lexer.getNextToken();
  EXPECT_EQ(token.type, TokenType::UNEXPECTED);
  EXPECT_EQ(token.value, L" first line ");
}

TEST_F(LexerTest, LexerHandlesEscapeSequencesInStringLiterals) {
  m_reader.load(L"\" \\\" \\\' \\n \\t \"");
  auto token = m_lexer.getNextToken();
  EXPECT_EQ(token.type, TokenType::STRING);
  EXPECT_EQ(token.value, L" \" \' \n \t ");
  EXPECT_EQ(token.strValue.value(), L" \" \' \n \t ");
}
