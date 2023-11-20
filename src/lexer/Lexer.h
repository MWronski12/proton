#pragma once

#include "CharReaderBase.h"
#include "Token.h"

class Lexer {
public:
  Lexer(CharReaderBase &reader);

  Token getNextToken();

private:
  void skipWhiteSpaces();

  CharReaderBase &m_reader;
};
