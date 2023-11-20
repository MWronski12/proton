#pragma once

struct Token {
  enum class TokenType {

    // Keywords
    CONST_KWRD,
    VOID_KWRD,
    INT_KWRD,
    FLOAT_KWRD,
    BOOL_KWRD,
    CHAR_KWRD,
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
    RANGE_SPECIFIER,
    EXTRACTION_OP,
    INSERTION_OP,

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
    SINGLE_QUOTE,
    DOUBLE_QUOTE,
    BACKSLASH,

    // COMMENTS
    COMMENT,

    // WHITESPACES
    SPACE,
    TAB,
    NEWLINE,

    // SPECIAL TOKENS
    ETX,
    UNDEFINED,
  };
};
