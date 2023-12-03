#include <gtest/gtest.h>

#include "TokenType.h"

TEST(TokenTypeTest, UnexpectedTokenIsFirst) {
  EXPECT_EQ(int(TokenType::UNEXPECTED), 0);
}

TEST(TokenTypeTest, ETXTokenIsLast) {
  EXPECT_EQ(int(TokenType::ETX), TOKEN_TYPE_NAMES.size() - 1);
}

TEST(TokenTypeTest, KeywordsOffset) {
  EXPECT_EQ(KEYWORDS_OFFSET, static_cast<int>(TokenType::UNEXPECTED) + 1);
}

TEST(TokenTypeTest, OperatorsOffset) {
  EXPECT_EQ(OPERATORS_OFFSET, static_cast<int>(TokenType::STRING) + 1);
}

TEST(TokenTypeTest, PunctuationOffset) {
  EXPECT_EQ(PUNCTUATION_OFFSET, static_cast<int>(TokenType::EXTRACTION_OP) + 1);
}

TEST(TokenTypeTest, KeywordsNamesMatchLiteralRepresentation) {
  for (size_t i = 0; i < KEYWORDS.size(); ++i) {
    EXPECT_EQ(KEYWORDS[i], TOKEN_TYPE_NAMES[i + KEYWORDS_OFFSET]);
  }
}
