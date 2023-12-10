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
  EXPECTED_DEFINITION,
  VARDEF_MISSING_SEMICOLON,

  // Semantic Errors
  MISSING_MAIN_FUNCTION,
  FUNCTION_REDEFINITION,
};

const auto LEXICAL_ERROR = std::string("Lexical Error");
const auto SYNTAX_ERROR = std::string("Syntax Error");
const auto SEMANTIC_ERROR = std::string("Semantic Error");

using ErrorInfo = std::pair<std::string, std::string>;  // pairs of <ErrorTypeStr, ErrorMsg>

static const std::vector<ErrorInfo> ERROR_MESSAGES = {
    // Lexical Errors
    {LEXICAL_ERROR, "Invalid number literal!"},
    {LEXICAL_ERROR,
     "Invalid char literal! Char literals must contain exactly one character "
     "enclosed in single quotes!"},
    {LEXICAL_ERROR, "Missing closing quote!"},
    {LEXICAL_ERROR, "Unexpected character encountered!"},
    {LEXICAL_ERROR, "Unexpected end of file reached!"},

    // Syntax Errors
    {SYNTAX_ERROR, "Expected definition!"},
    {SYNTAX_ERROR, "Missing semicolon in variable definition!"},

    // Semantic Errors
    {SEMANTIC_ERROR, "Expected main function definition (fn main() -> int { return 0; }) !"},
    {SEMANTIC_ERROR, "Function redefinition!"},
};
