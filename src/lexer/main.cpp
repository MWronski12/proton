#include <iostream>
#include <sstream>
#include <string>

#include "FileCharReader.h"
#include "Lexer.h"
#include "utils.h"

const std::string srcString = R"(

$$
This is a sample program.
Proton programming language is cool.
$$

const N: int = 10;

$ entry point of the program
fn main() -> int {
    for i in 0 until N {
        << i << "\n";
    }
}

)";

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <file>" << std::endl;
    return 1;
  }
  std::string file = argv[1];

  FileCharReader reader{file};
  Lexer lexer{reader};

  auto token = lexer.getNextToken();
  do {
    printTokenInfo(token);
    token = lexer.getNextToken();
  } while (token.type != TokenType::ETX);

  printTokenInfo(token);

  return 0;
}
