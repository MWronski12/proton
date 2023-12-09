#pragma once

#include "CharReaderBase.h"
#include "ErrorHandlerBase.h"
#include "Token.h"

class Lexer {
 public:
  explicit Lexer(CharReaderBase& reader, ErrorHandlerBase& errorHandler);
  Token getNextToken();

 private:
  void buildToken();

  void skipWhiteSpaces();

  // Identifiers
  bool isIdentifierStart(wchar_t first) const;
  void buildIdentifier();
  void matchIdentifier();
  bool isIdentifierChar(const wchar_t c) const;

  // Numbers
  bool isNumberStart(wchar_t first) const;
  void buildNumber();
  void matchNumber();
  void validateBuiltNumber();
  bool isNumberChar(const wchar_t c) const;
  bool isAllowedAfterNumber(const wchar_t c) const;

  // Strings
  bool isStringStart(wchar_t first) const;
  void buildString();
  void addEscapedChar(const wchar_t c);

  // Chars
  bool isCharStart(wchar_t first) const;
  void buildChar();

  // Comments
  bool isCommentStart(wchar_t first) const;
  void buildComment();
  void matchMultiLineComment();
  void matchSingleLineComment();

  // Operators, punctuation and special tokens
  void buildOther();

  CharReaderBase& m_reader;
  ErrorHandlerBase& m_errorHandler;

  Token m_token;
};
