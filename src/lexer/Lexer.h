#pragma once

#include "CharReaderBase.h"
#include "Token.h"

class Lexer {
 public:
  explicit Lexer(CharReaderBase& reader);
  Token getNextToken();

 private:
  void buildToken();

  void skipWhiteSpaces();

  bool isIdentifierStart(wchar_t first);
  void buildIdentifier();
  void matchIdentifier();
  bool isIdentifierChar(const wchar_t c) const;

  bool isNumberStart(wchar_t first);
  void buildNumber();
  void matchNumber();
  void validateBuiltNumber();
  bool isNumberChar(const wchar_t c) const;
  bool isAllowedAfterNumber(const wchar_t c) const;

  bool isStringStart(wchar_t first);
  void buildString();
  void addEscapedChar(const wchar_t c);

  bool isCharStart(wchar_t first);
  void buildChar();

  bool isCommentStart(wchar_t first);
  void buildComment();
  void matchMultiLineComment();
  void matchSingleLineComment();

  void buildOther();

  CharReaderBase& m_reader;
  Token m_token;
};
