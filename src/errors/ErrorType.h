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

  ERROR_PARSING_VARDEF,
  VARDEF_EXPECTED_IDENTIFIER,
  VARDEF_EXPECTED_COLON,
  VARDEF_EXPECTED_TYPE_IDENTIFIER,
  VARDEF_EXPECTED_ASSIGNMENT,
  VARDEF_EXPECTED_EXPRESSION,
  VARDEF_EXPECTED_SEMICOLON,

  ERROR_PARSING_CONSTDEF,
  CONSTDEF_EXPECTED_IDENTIFIER,
  CONSTDEF_EXPECTED_COLON,
  CONSTDEF_EXPECTED_TYPE_IDENTIFIER,
  CONSTDEF_EXPECTED_ASSIGNMENT,
  CONSTDEF_EXPECTED_EXPRESSION,
  CONSTDEF_EXPECTED_SEMICOLON,

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
    {SYNTAX_ERROR, "Definition expected!"},  // Definition

    {SYNTAX_ERROR, "Error parsing variable definition!"},
    {SYNTAX_ERROR, "Expected identifier in variable definition!"},
    {SYNTAX_ERROR, "Expected colon in variable definition!"},
    {SYNTAX_ERROR, "Expected type identifier in variable definition!"},
    {SYNTAX_ERROR, "Expected assignment in variable definition!"},
    {SYNTAX_ERROR, "Expected expression in variable definition!"},
    {SYNTAX_ERROR, "Missing semicolon at the end of variable definition!"},

    {SYNTAX_ERROR, "Error parsing const definition!"},
    {SYNTAX_ERROR, "Expected identifier in const definition!"},
    {SYNTAX_ERROR, "Expected colon in const definition!"},
    {SYNTAX_ERROR, "Expected type identifier in const definition!"},
    {SYNTAX_ERROR, "Expected assignment in const definition!"},
    {SYNTAX_ERROR, "Expected expression in const definition!"},
    {SYNTAX_ERROR, "Missing semicolon at the end of const definition!"},

    // Semantic Errors
    {SEMANTIC_ERROR, "Expected main function definition (fn main() -> int { return 0; }) !"},
    {SEMANTIC_ERROR, "Function redefinition!"},
};
