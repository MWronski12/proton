#include "Lexer.h"
#include "Parser.h"
#include "StrictErrorHandler.h"
#include "StringCharReader.h"

int main() {
  StrictErrorHandler errorHandler;
  StringCharReader reader{L"2 + 2 * 3;"};
  Lexer lexer{reader, errorHandler};
  Parser parser{lexer, errorHandler};

  auto program = parser.parseProgram();
}