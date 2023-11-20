#include <gtest/gtest.h>

#include "StringCharReader.h"

TEST(StringCharReader, HandlesEmptyString) {
  StringCharReader reader{L""};
  EXPECT_EQ(reader.getPosition().line, 0);
  EXPECT_EQ(reader.getPosition().column, 0);
  EXPECT_EQ(reader.getCharacter(), std::nullopt);
  EXPECT_EQ(reader.getNextCharacter(), std::nullopt);
};

TEST(StringCharReader, HandlesSimpleString) {
  std::wstring str = L"Hello World";
  StringCharReader reader{str};

  for (size_t i = 0; i < str.size(); i++) {
    auto c = str[i];
    auto pos = reader.getPosition();

    EXPECT_EQ(reader.getNextCharacter(), c);
    EXPECT_EQ(reader.getCharacter(), c);
    EXPECT_EQ(pos.column, i);
    EXPECT_EQ(pos.line, 0);
  }
};

TEST(StringCharReade, HandlesNewLines) {
  std::wstring str = L"\n\n\n";
  StringCharReader reader{str};
  for (int i = 0; i <= str.length(); i++) {
    EXPECT_EQ(reader.getPosition().line, i);
    reader.getNextCharacter();
  }
};

TEST(StringCharReader, HandlesNonASCIICharacters) {
  std::wstring str = L"こんにちは";
  StringCharReader reader{str};
  while (reader.getNextCharacter())
    ;
  EXPECT_EQ(reader.getPosition().column, str.length() + 1);
};
