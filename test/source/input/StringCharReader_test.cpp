#include "StringCharReader.h"

#include <gtest/gtest.h>

TEST(StringCharReader, HandlesEmptyString) {
  StringCharReader reader{L""};
  EXPECT_EQ(reader.pos().line, 0);
  EXPECT_EQ(reader.pos().column, 0);
  EXPECT_EQ(reader.pos().sourceFile, std::string("<custom string>"));
  EXPECT_EQ(reader.last(), NO_CHAR_YET);
  EXPECT_EQ(reader.peek(), wchar_t(WEOF));
  EXPECT_EQ(reader.get(), wchar_t(WEOF));
}

TEST(StringCharReader, HandlesSimpleString) {
  std::wstring str = L"Hello World";
  StringCharReader reader{str};

  for (size_t i = 0; i < str.size(); i++) {
    auto c = str[i];
    auto pos = reader.pos();

    EXPECT_EQ(reader.get(), c);
    EXPECT_EQ(reader.last(), c);
    EXPECT_EQ(pos.column, i);
    EXPECT_EQ(pos.line, 0);
    EXPECT_EQ(reader.pos().sourceFile, std::string("<custom string>"));
  }
}

TEST(StringCharReader, HandlesNewLines) {
  std::wstring str = L"\n\n\n";
  StringCharReader reader{str};
  for (uint i = 0; i <= str.length(); i++) {
    EXPECT_EQ(reader.pos().line, i);
    EXPECT_EQ(reader.pos().sourceFile, std::string("<custom string>"));
    reader.get();
  }
}

TEST(StringCharReader, HandlesNonASCIICharacters) {
  std::wstring str = L"こんにちは";
  StringCharReader reader{str};

  EXPECT_EQ(reader.peek(), str[0]);
  while (reader.get() != wchar_t(WEOF))
    ;
  EXPECT_EQ(reader.pos().column, str.length() + 1);
  EXPECT_EQ(reader.pos().sourceFile, std::string("<custom string>"));
}

TEST(StringCharReader, HandlesLoadingNewString) {
  std::wstring str = L"Hello";
  StringCharReader reader{str};
  EXPECT_EQ(reader.getInputFilename(), "<custom string>");
  EXPECT_EQ(reader.peek(), L'H');

  reader.load(L"World");
  EXPECT_EQ(reader.getInputFilename(), "<custom string>");
  EXPECT_EQ(reader.peek(), L'W');
}
