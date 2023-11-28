#include "Lexer.h"
#include "StringCharReader.h"
#include "utils.h"

int main() {
  std::wstring src = L"fn main() -> int { return 1; }";
  StringCharReader reader(src);
  Lexer lexer(reader);

  do {
    auto token = lexer.getNextToken();
    printTokenInfo(token);
  } while (lexer.getToken().type != TokenType::ETX);

  return 0;
}
