#pragma once

#include <unordered_map>

enum class ErrorType {
  // Lexical Errors
  INVALID_NUMBER_LITERAL = 0,
  INVALID_CHAR_LITERAL,
  MISSING_CLOSING_QUOTE,
  UNEXPECTED_CHARACTER,
  UNEXPECTED_END_OF_FILE,

  /* ------------------------------ Syntax Errors ----------------------------- */

  // Variable Definition
  VARDEF_EXPECTED_VAR_KWRD,
  VARDEF_EXPECTED_IDENTIFIER,
  VARDEF_EXPECTED_COLON,
  VARDEF_EXPECTED_TYPE_IDENTIFIER,
  VARDEF_EXPECTED_ASSIGNMENT,
  VARDEF_EXPECTED_EXPRESSION,
  VARDEF_EXPECTED_SEMICOLON,

  // Const Definition
  CONSTDEF_EXPECTED_CONST_KWRD,
  CONSTDEF_EXPECTED_IDENTIFIER,
  CONSTDEF_EXPECTED_COLON,
  CONSTDEF_EXPECTED_TYPE_IDENTIFIER,
  CONSTDEF_EXPECTED_ASSIGNMENT,
  CONSTDEF_EXPECTED_EXPRESSION,
  CONSTDEF_EXPECTED_SEMICOLON,

  // Struct Definition
  STRUCTDEF_EXPECTED_STRUCT_KWRD,
  STRUCTDEF_EXPECTED_IDENTIFIER,
  STRUCTDEF_EXPECTED_LBRACE,
  STRUCTDEF_EXPECTED_RBRACE,
  STRUCTDEF_EXPECTED_SEMICOLON,
  STRUCTMEMBER_EXPECTED_IDENTIFIER,
  STRUCTMEMBER_EXPECTED_COLON,
  STRUCTMEMBER_EXPECTED_TYPE_IDENTIFIER,
  STRUCTMEMBER_EXPECTED_SEMICOLON,
  STRUCTMEMBER_REDEFINITION,

  // Variant Definition
  VARIANTDEF_EXPECTED_VARIANT_KWRD,
  VARIANTDEF_EXPECTED_IDENTIFIER,
  VARIANTDEF_EXPECTED_LBRACE,
  VARIANTDEF_EXPECTED_RBRACE,
  VARIANTDEF_EXPECTED_SEMICOLON,

  // Semantic Errors
  MISSING_MAIN_FUNCTION,
  REDEFINITION,
};

const auto LEXICAL_ERROR = std::string("Lexical Error");
const auto SYNTAX_ERROR = std::string("Syntax Error");
const auto SEMANTIC_ERROR = std::string("Semantic Error");
const auto INTERNAL_ERROR = std::string("Internal Error");

using ErrorInfo = std::pair<std::string, std::string>;  // pairs of <ErrorTypeStr, ErrorMsg>

static const std::unordered_map<ErrorType, ErrorInfo> Errors = {
    /* ----------------------------- Lexical Errors ----------------------------- */

    {ErrorType::INVALID_NUMBER_LITERAL, {LEXICAL_ERROR, "Invalid number literal!"}},
    {ErrorType::INVALID_CHAR_LITERAL,
     {LEXICAL_ERROR,
      "Invalid char literal! Char literals must contain exactly one character "
      "enclosed in single quotes!"}},
    {ErrorType::MISSING_CLOSING_QUOTE, {LEXICAL_ERROR, "Missing closing quote!"}},
    {ErrorType::UNEXPECTED_CHARACTER, {LEXICAL_ERROR, "Unexpected character encountered!"}},
    {ErrorType::UNEXPECTED_END_OF_FILE, {LEXICAL_ERROR, "Unexpected end of file reached!"}},

    /* ------------------------------ Syntax Errors ----------------------------- */

    // Variable Definition
    {ErrorType::VARDEF_EXPECTED_VAR_KWRD,
     {INTERNAL_ERROR, "Tried to parse variable definition, but 'var' keyword was not found!"}},
    {ErrorType::VARDEF_EXPECTED_IDENTIFIER,
     {SYNTAX_ERROR, "Expected identifier in variable definition!"}},
    {ErrorType::VARDEF_EXPECTED_COLON, {SYNTAX_ERROR, "Expected colon in variable definition!"}},
    {ErrorType::VARDEF_EXPECTED_TYPE_IDENTIFIER,
     {SYNTAX_ERROR, "Expected type identifier in variable definition!"}},
    {ErrorType::VARDEF_EXPECTED_ASSIGNMENT,
     {SYNTAX_ERROR, "Expected assignment in variable definition!"}},
    {ErrorType::VARDEF_EXPECTED_EXPRESSION,
     {SYNTAX_ERROR, "Expected expression in variable definition!"}},
    {ErrorType::VARDEF_EXPECTED_SEMICOLON,
     {SYNTAX_ERROR, "Missing semicolon at the end of variable definition!"}},

    // Const Definition
    {ErrorType::CONSTDEF_EXPECTED_CONST_KWRD,
     {INTERNAL_ERROR, "Tried to parse const definition, but 'const' keyword was not found!"}},
    {ErrorType::CONSTDEF_EXPECTED_IDENTIFIER,
     {SYNTAX_ERROR, "Expected identifier in const definition!"}},
    {ErrorType::CONSTDEF_EXPECTED_COLON, {SYNTAX_ERROR, "Expected colon in const definition!"}},
    {ErrorType::CONSTDEF_EXPECTED_TYPE_IDENTIFIER,
     {SYNTAX_ERROR, "Expected type identifier in const definition!"}},
    {ErrorType::CONSTDEF_EXPECTED_ASSIGNMENT,
     {SYNTAX_ERROR, "Expected assignment in const definition!"}},
    {ErrorType::CONSTDEF_EXPECTED_EXPRESSION,
     {SYNTAX_ERROR, "Expected expression in const definition!"}},
    {ErrorType::CONSTDEF_EXPECTED_SEMICOLON,
     {SYNTAX_ERROR, "Missing semicolon at the end of const definition!"}},

    // Struct Definition
    {ErrorType::STRUCTDEF_EXPECTED_STRUCT_KWRD,
     {INTERNAL_ERROR, "Tried to parse struct definition, but 'struct' keyword was not found!"}},
    {ErrorType::STRUCTDEF_EXPECTED_IDENTIFIER,
     {SYNTAX_ERROR, "Expected identifier in struct definition!"}},
    {ErrorType::STRUCTDEF_EXPECTED_LBRACE,
     {SYNTAX_ERROR, "Expected opening brace in struct definition!"}},
    {ErrorType::STRUCTDEF_EXPECTED_RBRACE,
     {SYNTAX_ERROR, "Expected closing brace in struct definition!"}},
    {ErrorType::STRUCTDEF_EXPECTED_SEMICOLON,
     {SYNTAX_ERROR, "Missing semicolon at the end of struct definition!"}},
    {ErrorType::STRUCTMEMBER_EXPECTED_IDENTIFIER,
     {SYNTAX_ERROR, "Expected identifier in struct member definition!"}},
    {ErrorType::STRUCTMEMBER_EXPECTED_COLON,
     {SYNTAX_ERROR, "Expected colon in struct member definition!"}},
    {ErrorType::STRUCTMEMBER_EXPECTED_TYPE_IDENTIFIER,
     {SYNTAX_ERROR, "Expected type identifier in struct member definition!"}},
    {ErrorType::STRUCTMEMBER_EXPECTED_SEMICOLON,
     {SYNTAX_ERROR, "Missing semicolon at the end of struct member definition!"}},
    {ErrorType::STRUCTMEMBER_REDEFINITION, {SEMANTIC_ERROR, "Struct member redefinition!"}},

    // Variant Definition
    {ErrorType::VARIANTDEF_EXPECTED_VARIANT_KWRD,
     {INTERNAL_ERROR, "Tried to parse variant definition, but 'variant' keyword was not found!"}},
    {ErrorType::VARIANTDEF_EXPECTED_IDENTIFIER,
     {SYNTAX_ERROR, "Expected identifier in variant definition!"}},
    {ErrorType::VARIANTDEF_EXPECTED_LBRACE,
     {SYNTAX_ERROR, "Expected opening brace in variant definition!"}},
    {ErrorType::VARIANTDEF_EXPECTED_RBRACE,
     {SYNTAX_ERROR, "Expected closing brace in variant definition!"}},
    {ErrorType::VARIANTDEF_EXPECTED_SEMICOLON,
     {SYNTAX_ERROR, "Missing semicolon at the end of variant definition!"}},

    /* ----------------------------- Semantic Errors ---------------------------- */
    {ErrorType::MISSING_MAIN_FUNCTION,
     {SEMANTIC_ERROR, "Expected main function definition (fn main() -> int { return 0; }) !"}},
    {ErrorType::REDEFINITION, {SEMANTIC_ERROR, "Redefinition!"}},
};
