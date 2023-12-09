#pragma once

#include <vector>

enum class ErrorType {
  // Lexical Errors
  INVALID_NUMBER_LITERAL = 0,
  INVALID_CHAR_LITERAL,
  MISSING_CLOSING_QUOTE,
  UNEXPECTED_CHARACTER,
  UNEXPECTED_END_OF_FILE,

  // Syntax Errors
  EXPECTED_LBRACE,

  // Semantic Errors
};

const auto LEXICAL_ERROR = std::string("Lexical Error");
const auto SYNTAX_ERROR = std::string("Syntax Error");

using ErrorInfo = std::pair<std::string, std::string>;  // pairs of <ErrorTypeStr, ErrorMsg>

static const std::vector<ErrorInfo> ERROR_MESSAGES = {
    {LEXICAL_ERROR, "Invalid number literal!"},
    {LEXICAL_ERROR,
     "Invalid char literal! Char literals must contain exactly one character "
     "enclosed in single quotes!"},
    {LEXICAL_ERROR, "Missing closing quote!"},
    {LEXICAL_ERROR, "Unexpected character encountered!"},
    {LEXICAL_ERROR, "Unexpected end of file reached!"},

    {SYNTAX_ERROR, "Exppected LBRACE '{' !"},
};
