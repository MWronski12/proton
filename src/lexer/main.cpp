#include <iostream>
#include <sstream>
#include <string>

#include "ErrorHandler.h"
#include "FileCharReader.h"
#include "Lexer.h"
#include "lexer_utils.h"

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cerr << "Usage: lexer <file>" << std::endl;
    return 1;
  }
  std::string file = argv[1];

  ErrorHandler errorHandler;
  FileCharReader reader{file};
  Lexer lexer{reader, errorHandler};

  auto token = lexer.getNextToken();
  while (token.type != TokenType::ETX) {
    printTokenInfo(token);
    token = lexer.getNextToken();
  }
  printTokenInfo(token);

  return 0;
}
