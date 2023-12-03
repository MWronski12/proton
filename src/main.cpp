#include "Lexer.h"
#include "StrictErrorHandler.h"
#include "StringCharReader.h"
#include "utils.h"

int main() {
  std::wstring src = L"fn main() -> int { return 1; }";
  StringCharReader reader(src);
  StrictErrorHandler errorHandler;

  Lexer lexer(reader, errorHandler);

  auto token = lexer.getNextToken();
  while (token.type != TokenType::ETX) {
    printTokenInfo(token);
    token = lexer.getNextToken();
  }
  printTokenInfo(token);

  return 0;
}
