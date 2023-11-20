#include "Lexer.h"
#include "Token.h"

#include "cwctype"

Lexer::Lexer(CharReaderBase &reader) : m_reader{reader} {}

Token Lexer::getNextToken() {
  skipWhiteSpaces();

  return Token{};
}

void Lexer::skipWhiteSpaces() {
  while (iswspace(m_reader.peek()))
    m_reader.get();
}
