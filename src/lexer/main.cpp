#include <iostream>
#include <sstream>
#include <string>

#include "FileCharReader.h"
#include "Lexer.h"
#include "utils.h"

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <file>" << std::endl;
    return 1;
  }
  std::string file = argv[1];

  FileCharReader reader{file};
  Lexer lexer{reader};

  do {
    auto token = lexer.getNextToken();
    printTokenInfo(token);
  } while (lexer.getToken().type != TokenType::ETX);

  return 0;
}
