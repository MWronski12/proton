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

  // Semantic Errors
};

using ErrorInfo =
    std::pair<std::string, std::string>;  // pairs of <ErrorTypeStr, ErrorMsg>

static const std::vector<ErrorInfo> ERROR_MESSAGES = {
    {"Lexical Error", "Invalid number literal!"},
    {"Lexical Error",
     "Invalid char literal! Char literals must contain exactly one character "
     "enclosed in single quotes!"},
    {"Lexical Error", "Missing closing quote!"},
    {"Lexical Error", "Unexpected character encountered!"},
    {"Lexical Error", "Unexpected end of file reached!"},
};
