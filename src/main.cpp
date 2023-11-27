#include "Lexer.h"
#include "StringCharReader.h"

int main() {
  std::wstring src = L"fn main() -> int { return 1; }";
  StringCharReader reader(src);
  Lexer lexer(reader);
  auto firstToken = lexer.getNextToken();
  return 0;
}
