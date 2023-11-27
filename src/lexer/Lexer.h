#pragma once

#include "CharReaderBase.h"
#include "Token.h"

class Lexer {
 public:
  Lexer(CharReaderBase& reader);
  Token getNextToken();

 private:
  Token buildToken();

  void skipWhiteSpaces();
  void buildIdentifier();
  bool isIdentifierChar(wchar_t c);
  void matchIdentifier();
  void buildNumber();
  bool isNumberChar(wchar_t c);
  void matchNumber();
  void buildString();
  void buildChar();
  void buildComment();
  void matchMultiLineComment();
  void matchSingleLineComment();
  void buildOther();

  CharReaderBase& m_reader;
  Token m_token;
};
