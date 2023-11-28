#include <gtest/gtest.h>

#include <iostream>

#include "Lexer.h"
#include "StringCharReader.h"

class LexerTest : public ::testing::Test {
 public:
  LexerTest() : m_lexer{m_reader} {}

 protected:
  StringCharReader m_reader;
  Lexer m_lexer;
};

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

TEST_F(LexerTest, LexerHandlesStringLiterals) {
  m_reader.load(L"\"Hello World!\"");
  auto token = m_lexer.getNextToken();
  EXPECT_EQ(token.type, TokenType::STRING);
  EXPECT_EQ(token.value, L"Hello World!");

  m_reader.load(L"\" hello ");  // WEOF before closing quote
  token = m_lexer.getNextToken();
  EXPECT_EQ(token.type, TokenType::UNEXPECTED);
  EXPECT_EQ(token.value, L" hello ");

  m_reader.load(
      L"\" first line \n second line \"");  // Newline before closing quote
  token = m_lexer.getNextToken();
  EXPECT_EQ(token.type, TokenType::UNEXPECTED);
  EXPECT_EQ(token.value, L" first line ");
}

TEST_F(LexerTest, LexerHandlesEscapeSequencesInStringLiterals) {
  m_reader.load(L"\" \\\" \\\' \\n \\t \"");
  auto token = m_lexer.getNextToken();
  EXPECT_EQ(token.type, TokenType::STRING);
  EXPECT_EQ(token.value, L" \" \' \n \t ");
}

TEST_F(LexerTest, LexerHandlesCharLiterals) {
  m_reader.load(L"\'X\'");
  auto token = m_lexer.getNextToken();
  EXPECT_EQ(token.type, TokenType::CHAR);
  EXPECT_EQ(token.value, L"X");

  m_reader.load(L"\'XXX\'");  // Multiple character char literal
  token = m_lexer.getNextToken();
  EXPECT_EQ(token.type, TokenType::UNEXPECTED);
  EXPECT_EQ(token.value, L"XXX");

  m_reader.load(L"\'c");  // WEOF before closing quote
  token = m_lexer.getNextToken();
  EXPECT_EQ(token.type, TokenType::UNEXPECTED);
  EXPECT_EQ(token.value, L"c");

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
}

TEST_F(LexerTest, LexerHandlesValidIntLiterals) {
  m_reader.load(L"1234567890 0 666");

  auto token = m_lexer.getNextToken();
  EXPECT_EQ(token.type, TokenType::INTEGER);
  EXPECT_EQ(token.value, L"1234567890");

  token = m_lexer.getNextToken();
  EXPECT_EQ(token.type, TokenType::INTEGER);
  EXPECT_EQ(token.value, L"0");

  token = m_lexer.getNextToken();
  EXPECT_EQ(token.type, TokenType::INTEGER);
  EXPECT_EQ(token.value, L"666");
}

TEST_F(LexerTest, LexerHandlesInvalidIntLiterals) {
  m_reader.load(L"3x 5( 01");

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

TEST_F(LexerTest, LexerHandlesValidFloatLiterals) {
  m_reader.load(L"123.456 0.0 0. 1.290");

  auto token = m_lexer.getNextToken();
  EXPECT_EQ(token.type, TokenType::FLOAT);
  EXPECT_EQ(token.value, L"123.456");

  token = m_lexer.getNextToken();
  EXPECT_EQ(token.type, TokenType::FLOAT);
  EXPECT_EQ(token.value, L"0.0");

  token = m_lexer.getNextToken();
  EXPECT_EQ(token.type, TokenType::FLOAT);
  EXPECT_EQ(token.value, L"0.");

  token = m_lexer.getNextToken();
  EXPECT_EQ(token.type, TokenType::FLOAT);
  EXPECT_EQ(token.value, L"1.290");
}

TEST_F(LexerTest, LexerHandlesInvalidFloatLiterals) {
  m_reader.load(L"00.0 00. 0.2x 3.14# 0.0.0.0");

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

TEST_F(LexerTest, LexerHandlesUnexpectedCharacters) {
  m_reader.load(L"\\ # ? `");

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
