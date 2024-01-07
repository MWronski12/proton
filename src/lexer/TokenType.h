#pragma once

#include <string>
#include <vector>

enum class TokenType {
  // Default
  UNEXPECTED = 0,

  // Keywords
  VAR_KWRD,
  CONST_KWRD,
  VOID_KWRD,
  INT_KWRD,
  FLOAT_KWRD,
  CHAR_KWRD,
  BOOL_KWRD,
  STRING_KWRD,
  VARIANT_KWRD,
  STRUCT_KWRD,
  FN_KWRD,

  IF_KWRD,
  ELIF_KWRD,
  ELSE_KWRD,
  FOR_KWRD,
  IN_KWRD,
  UNTIL_KWRD,
  WHILE_KWRD,
  CONTINUE_KWRD,
  BREAK_KWRD,
  RETURN_KWRD,

  MATCH_KWRD,
  CASE_KWRD,
  AS_KWRD,

  // IDENTIFIER
  IDENTIFIER,

  // LITERALS
  INTEGER,
  FLOAT,
  CHAR,
  STRING,
  BOOL,

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
  LESS_OR_EQUAL,
  GREATER_OR_EQUAL,
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
  NO_TOKEN_YET,
};

inline static const std::vector<std::wstring> TOKEN_TYPE_NAMES = {
    // DEFAULT
    L"unexpected",

    // KEYWORDS
    L"var",
    L"const",
    L"void",
    L"int",
    L"float",
    L"char",
    L"bool",
    L"string",
    L"variant",
    L"struct",
    L"fn",

    L"if",
    L"elif",
    L"else",
    L"for",
    L"in",
    L"until",
    L"while",
    L"continue",
    L"break",
    L"return",

    L"match",
    L"case",
    L"as",

    // IDENTIFIER
    L"identifier",

    // LITERALS
    L"integer_literal",
    L"float_literal",
    L"char_literal",
    L"string_literal",
    L"bool_literal",

    // OPERATORS
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
    L"less_or_equal",
    L"greater_or_equal",
    L"logic_or",
    L"logic_and",
    L"logic_not",
    L"insertion_op",
    L"extraction_op",

    // PUNCTUATION
    L"dot",
    L"comma",
    L"colon",
    L"semicolon",
    L"lparen",
    L"rparen",
    L"lbrace",
    L"rbrace",
    L"arrow",

    // COMMENTS
    L"single_line_comment",
    L"multi_line_comment",

    // SPECIAL TOKENS
    L"etx",
};

inline static const int KEYWORDS_OFFSET = (int)TokenType::VAR_KWRD;
inline static const std::vector<std::wstring> KEYWORDS = {
    L"var",     L"const",  L"void",     L"int",   L"float",  L"char",  L"bool", L"string",
    L"variant", L"struct", L"fn",       L"if",    L"elif",   L"else",  L"for",  L"in",
    L"until",   L"while",  L"continue", L"break", L"return", L"match", L"case", L"as",
};

inline static const int OPERATORS_OFFSET = (int)TokenType::ASSIGNMENT;
inline static const std::vector<std::wstring> OPERATORS = {
    L"=", L"+",  L"-",  L"*",  L"/",  L"%", L"==", L"!=", L"<",
    L">", L"<=", L">=", L"||", L"&&", L"!", L"<<", L">>",
};

inline static const int PUNCTUATION_OFFSET = (int)TokenType::DOT;
inline static const std::vector<std::wstring> PUNCTUATION = {
    L".", L",", L":", L";", L"(", L")", L"{", L"}", L"->",
};
