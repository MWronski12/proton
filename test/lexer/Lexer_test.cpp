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
}

TEST_F(LexerTest, LexerHandlesEscapeSequencesInStringLiterals) {
  m_reader.load(L"\" \\\" \\\' \"");
  auto token = m_lexer.getNextToken();
  EXPECT_EQ(token.type, TokenType::STRING);
  EXPECT_EQ(token.value, L" \" \' ");
}

TEST_F(LexerTest, LexerHandlesCharLiterals) {
  m_reader.load(L"\'X\'");
  auto token = m_lexer.getNextToken();
  EXPECT_EQ(token.type, TokenType::CHAR);
  EXPECT_EQ(token.value, L"X");
}

TEST_F(LexerTest, LexerHandlesEscapeSequencesInCharLiterals) {
  m_reader.load(L"\'\\\"\'");
  auto token = m_lexer.getNextToken();
  EXPECT_EQ(token.type, TokenType::CHAR);
  EXPECT_EQ(token.value, L"\"");
}

TEST_F(LexerTest, LexerHandlesIntLiterals) {
  m_reader.load(L"1234567890");
  auto token = m_lexer.getNextToken();
  EXPECT_EQ(token.type, TokenType::INTEGER);
  EXPECT_EQ(token.value, L"1234567890");
}

TEST_F(LexerTest, LexerHandlesFloatLiterals) {
  m_reader.load(L"123.456. next");
  auto token = m_lexer.getNextToken();
  EXPECT_EQ(token.type, TokenType::FLOAT);
  EXPECT_EQ(token.value, L"123.456");

  m_reader.load(L"3.2.3x");
  token = m_lexer.getNextToken();
  EXPECT_EQ(token.type, TokenType::FLOAT);
  EXPECT_EQ(token.value, L"3.2");
}

TEST_F(LexerTest, LexerHandlesUnexpectedCharacters) {
  m_reader.load(L"\\");
  auto token = m_lexer.getNextToken();
  EXPECT_EQ(token.type, TokenType::UNEXPECTED);
  EXPECT_EQ(token.value, L"\\");
}
