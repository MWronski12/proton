#pragma once

#include <string>
#include <vector>

enum class TokenType {
  // Keywords
  CONST_KWRD,
  VOID_KWRD,
  INT_KWRD,
  FLOAT_KWRD,
  CHAR_KWRD,
  BOOL_KWRD,
  STRING_KWRD,
  VARIANT_KWRD,
  STRUCT_KWRD,
  TRUE_KWRD,
  FALSE_KWRD,
  FN_KWRD,
  RETURN_KWRD,

  IF_KWRD,
  ELIF_KWRD,
  ELSE_KWRD,
  FOR_KWRD,
  IN_KWRD,
  UNTIL_KWRD,
  WHILE_KWRD,
  CONTINUE_KWRD,
  BREAK_KWRD,

  MATCH_KWRD,
  CASE_KWRD,
  AS_KWRD,

  // IDENTIFIER
  IDENTIFIER,

  // LITERALS
  INTEGER,
  FLOAT,
  CHAR,
  BOOL,
  STRING,

  // OPERATORS
  ASSIGNMENT,
  PLUS,
  MINUS,
  ASTERISK,
  SLASH,
  PERCENT,
  EQUALITY,
  INEQUALITY,
  LESS_THAN,
  GREATER_THAN,
  LESS_THAN_EQUAL,
  GREATER_THAN_EQUAL,
  LOGIC_OR,
  LOGIC_AND,
  LOGIC_NOT,
  INSERTION_OP,
  EXTRACTION_OP,

  // PUNCTUATION
  DOT,
  COMMA,
  COLON,
  SEMICOLON,
  LPAREN,
  RPAREN,
  LBRACE,
  RBRACE,
  ARROW,

  // COMMENTS
  SINGLE_LINE_COMMENT,
  MULTI_LINE_COMMENT,

  // SPECIAL TOKENS
  ETX,
  UNEXPECTED,
};

static const std::vector<std::wstring> TOKEN_TYPE_NAMES = {
    L"const",
    L"void",
    L"int",
    L"float",
    L"char",
    L"bool",
    L"string",
    L"variant",
    L"struct",
    L"true",
    L"false",
    L"fn",
    L"return",
    L"if",
    L"elif",
    L"else",
    L"for",
    L"in",
    L"until",
    L"while",
    L"continue",
    L"break",
    L"match",
    L"case",
    L"as",
    L"identifier",
    L"integer",
    L"float",
    L"char",
    L"bool",
    L"string",
    L"assignment",
    L"plus",
    L"minus",
    L"asterisk",
    L"slash",
    L"percent",
    L"equality",
    L"inequality",
    L"less_than",
    L"greater_than",
    L"less_than_equal",
    L"greater_than_equal",
    L"logic_or",
    L"logic_and",
    L"logic_not",
    L"insertion_op",
    L"extraction_op",
    L"dot",
    L"comma",
    L"colon",
    L"semicolon",
    L"lparen",
    L"rparen",
    L"lbrace",
    L"rbrace",
    L"arrow",
    L"single_line_comment",
    L"multi_line_comment",
    L"etx",
    L"unexpected",
};

static const int KEYWORDS_OFFSET = (int)TokenType::CONST_KWRD;
static const std::vector<std::wstring> KEYWORDS = {
    L"const",   L"void",   L"int",  L"float", L"char",  L"bool",   L"string",
    L"variant", L"struct", L"true", L"false", L"fn",    L"return", L"if",
    L"elif",    L"else",   L"for",  L"in",    L"until", L"while",  L"continue",
    L"break",   L"match",  L"case", L"as",
};

static const int OPERATORS_OFFSET = (int)TokenType::ASSIGNMENT;
static const std::vector<std::wstring> OPERATORS = {
    L"=", L"+",  L"-",  L"*",  L"/",  L"%", L"==", L"!=", L"<",
    L">", L"<=", L">=", L"||", L"&&", L"!", L"<<", L">>",
};

static const int PUNCTUATION_OFFSET = (int)TokenType::DOT;
static const std::vector<std::wstring> PUNCTUATION = {
    L".", L",", L":", L";", L"(", L")", L"{", L"}", L"->",
};
