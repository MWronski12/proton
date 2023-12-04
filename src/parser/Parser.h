// 1. Every ebnf definition is one parsingFunction
// 2. Need to report appropriate errors
// 3. Make sure objects representing lang constructs can only exist in a valid
// state.
//    Dont create empty objects and assign values to its members.
// 4. Expr bazowy, inne funkcje korzystaja tylko z niego.
//    Pochodne az do Literal sa wykorzystywane tylko wewnetrznie.
// 5. Keep position of the construct

#pragma once

#include <optional>

#include "ErrorHandlerBase.h"
#include "Lexer.h"

class Program {};

class Parser {
 public:
  Parser(Lexer& lexer, ErrorHandlerBase& errorHandler);

  std::optional<Program> parseProgram();

 private:
  Lexer& m_lexer;
  ErrorHandlerBase& m_errorHandler;
};
