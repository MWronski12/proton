#include "utils.h"

#include <iostream>

void printTokenInfo(const Token& token) {
  std::wcout << '\"' << token.value << '\"' << L" -> " << token.type << L" at "
             << token.position.line << ':' << token.position.column << ' '
             << L"\n";
}

std::wostream& operator<<(std::wostream& os, const TokenType& tokenType) {
  return std::wcout << TOKEN_TYPE_NAMES[int(tokenType)];
}
