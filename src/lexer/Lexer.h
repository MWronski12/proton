#pragma once

#include "CharReaderBase.h"
#include "Token.h"

class Lexer {
 public:
  Lexer(CharReaderBase& reader);
  Token getNextToken();
  Token getToken() const;

 private:
  Token buildToken();

  void skipWhiteSpaces();
  void buildIdentifier();
  void matchIdentifier();
  bool isIdentifierChar(const wchar_t c) const;

  void buildNumber();
  void matchNumber();
  void validateBuiltNumber();
  bool isNumberChar(const wchar_t c) const;
  bool isAllowedAfterNumber(const wchar_t c) const;

  void buildChar();
  void buildString();
  void addEscapedChar(const wchar_t c);

  void buildComment();
  void matchMultiLineComment();
  void matchSingleLineComment();
  void buildOther();

  CharReaderBase& m_reader;
  Token m_token;
};
