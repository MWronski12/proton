#include <iostream>

#include "lexer_utils.h"

void printTokenInfo(const Token& token) {
  std::wcout << '\"' << token.value << '\"' << L" -> " << token.type << L" at "
             << token.position.line << ':' << token.position.column << ' ' << L"\n";
}

std::wostream& operator<<(std::wostream& os, const TokenType& tokenType) {
  return os << TOKEN_TYPE_NAMES[int(tokenType)];
}
