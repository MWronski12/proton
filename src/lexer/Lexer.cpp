#include "Lexer.h"

#include <cwctype>
#include <stdexcept>

#include "Token.h"

Lexer::Lexer(CharReaderBase& reader) : m_reader{reader} {}

Token Lexer::getNextToken() {
  skipWhiteSpaces();
  return buildToken();
}

void Lexer::skipWhiteSpaces() {
  while (iswspace(m_reader.peek())) m_reader.get();
}

Token Lexer::buildToken() {
  m_token.value.clear();
  m_token.position = m_reader.pos();

  auto first = m_reader.peek();
  if (first == '_' || iswalpha(first)) {
    buildIdentifier();
  } else if (iswdigit(first)) {
    buildNumber();
  } else if (first == '"') {
    buildString();
  } else if (first == '\'') {
    buildChar();
  } else if (first == '$') {
    buildComment();
  } else {
    buildOther();
  }

  return m_token;
}

/* -------------------------------------------------------------------------- */
/*                            IDENTIFIER OR KEYWORD                           */
/* -------------------------------------------------------------------------- */

void Lexer::buildIdentifier() {
  while (isIdentifierChar(m_reader.peek())) {
    auto next = m_reader.get();
    m_token.value.push_back(next);
  }

  matchIdentifier();
}

bool Lexer::isIdentifierChar(wchar_t c) { return iswalnum(c); }

void Lexer::matchIdentifier() {
  if (m_token.value == L"const")
    m_token.type = TokenType::CONST_KWRD;
  else if (m_token.value == L"void")
    m_token.type = TokenType::VOID_KWRD;
  else if (m_token.value == L"int")
    m_token.type = TokenType::INT_KWRD;
  else if (m_token.value == L"float")
    m_token.type = TokenType::FLOAT_KWRD;
  else if (m_token.value == L"char")
    m_token.type = TokenType::CHAR_KWRD;
  else if (m_token.value == L"bool")
    m_token.type = TokenType::BOOL_KWRD;
  else if (m_token.value == L"string")
    m_token.type = TokenType::STRING_KWRD;
  else if (m_token.value == L"variant")
    m_token.type = TokenType::VARIANT_KWRD;
  else if (m_token.value == L"struct")
    m_token.type = TokenType::STRUCT_KWRD;
  else if (m_token.value == L"true")
    m_token.type = TokenType::TRUE_KWRD;
  else if (m_token.value == L"false")
    m_token.type = TokenType::FALSE_KWRD;
  else if (m_token.value == L"fn")
    m_token.type = TokenType::FN_KWRD;
  else if (m_token.value == L"return")
    m_token.type = TokenType::RETURN_KWRD;
  else if (m_token.value == L"if")
    m_token.type = TokenType::IF_KWRD;
  else if (m_token.value == L"elif")
    m_token.type = TokenType::ELIF_KWRD;
  else if (m_token.value == L"else")
    m_token.type = TokenType::ELSE_KWRD;
  else if (m_token.value == L"for")
    m_token.type = TokenType::FOR_KWRD;
  else if (m_token.value == L"in")
    m_token.type = TokenType::IN_KWRD;
  else if (m_token.value == L"until")
    m_token.type = TokenType::UNTIL_KWRD;
  else if (m_token.value == L"while")
    m_token.type = TokenType::WHILE_KWRD;
  else if (m_token.value == L"continue")
    m_token.type = TokenType::CONTINUE_KWRD;
  else if (m_token.value == L"break")
    m_token.type = TokenType::BREAK_KWRD;
  else if (m_token.value == L"match")
    m_token.type = TokenType::MATCH_KWRD;
  else if (m_token.value == L"case")
    m_token.type = TokenType::CASE_KWRD;
  else if (m_token.value == L"as")
    m_token.type = TokenType::AS_KWRD;
  else
    m_token.type = TokenType::IDENTIFIER;
}

/* -------------------------------------------------------------------------- */
/*                              FLOAT OR INTEGER                              */
/* -------------------------------------------------------------------------- */

void Lexer::buildNumber() {
  while (isNumberChar(m_reader.peek())) {
    auto next = m_reader.get();
    m_token.value.push_back(next);
  }

  matchNumber();
}

bool Lexer::isNumberChar(wchar_t c) {
  return iswdigit(c) ||
         (c == '.' && m_token.value.find('.') == std::wstring::npos);
}

void Lexer::matchNumber() {
  // Trailing dot - invalid
  if (m_token.value.back() == '.') {
    m_token.type = TokenType::UNEXPECTED;
  }
  // More than one dot - invalid
  else if (m_token.value.find_first_of('.') !=
           m_token.value.find_last_of('.')) {
    m_token.type = TokenType::UNEXPECTED;
  }
  // Exactly one dot - float
  else if (m_token.value.find('.') != std::wstring::npos) {
    m_token.type = TokenType::FLOAT;
  }
  // No dot - integer
  else {
    m_token.type = TokenType::INTEGER;
  }
}

/* -------------------------------------------------------------------------- */
/*                               STRING LITERAL                               */
/* -------------------------------------------------------------------------- */

//

void Lexer::buildString() {
  if (m_reader.get() != '\"')
    throw std::logic_error("String literal must begin with '\"' character!");

  while (true) {
    auto next = m_reader.peek();

    // Escape character
    if (next == '\\') {
      m_reader.get();                           // skip backslash
      m_token.value.push_back(m_reader.get());  // add escaped character
      continue;
    }
    // Newline and EOF not allowed
    else if (next == '\n' || next == WEOF) {
      m_token.type = TokenType::UNEXPECTED;  // missing closing quote
      return;
    }
    // End of string literal
    else if (next == '\"') {
      break;
    }

    // Add next character to the string
    m_token.value.push_back(m_reader.get());
  }

  if (m_reader.get() != '\"')
    throw std::logic_error("String literal must end with '\"' character!");

  m_token.type = TokenType::STRING;
}

/* -------------------------------------------------------------------------- */
/*                                CHAR LITERAL                                */
/* -------------------------------------------------------------------------- */

void Lexer::buildChar() {
  if (m_reader.get() != '\'')
    throw std::logic_error("Char literal must start with '\'' character!");

  auto next = m_reader.peek();

  // Escape character
  if (next == '\\') {
    m_reader.get();                           // skip backslash
    m_token.value.push_back(m_reader.get());  // add escaped character
  }
  // Newline and EOF not allowed
  else if (next == '\n' || next == WEOF) {
    m_token.type = TokenType::UNEXPECTED;  // missing closing quote
    return;
  }
  // Empty char literal
  else if (next == '\'') {
    m_token.type = TokenType::UNEXPECTED;  // empty char literal
    return;
  }
  // insert char literal value
  else {
    m_token.value.push_back(m_reader.get());
  }

  if (m_reader.get() != '\'')
    throw std::logic_error("Char literal must end with '\'' character!");

  m_token.type = TokenType::CHAR;
}

/* -------------------------------------------------------------------------- */
/*                                   COMMENT                                  */
/* -------------------------------------------------------------------------- */

void Lexer::buildComment() {
  if (m_reader.peek() != '$')
    throw std::logic_error("Comment must start with '$' character!");

  m_token.value.push_back(m_reader.get());

  if (m_reader.peek() == '$') {
    m_token.value.push_back(m_reader.get());
    matchMultiLineComment();
  } else {
    matchSingleLineComment();
  }
}

void Lexer::matchMultiLineComment() {
  while (true) {
    auto next = m_reader.get();

    if (next == WEOF) {
      m_token.type = TokenType::UNEXPECTED;  // missing closing comment
      return;
    }

    if (next == '$' && m_reader.get() == '$') {
      m_token.value.push_back('$');
      m_token.value.push_back('$');
      break;
    }

    m_token.value.push_back(next);
  }

  m_token.type = TokenType::MULTI_LINE_COMMENT;
}

void Lexer::matchSingleLineComment() {
  while (m_reader.peek() != '\n' && m_reader.peek() != WEOF) {
    m_token.value.push_back(m_reader.get());
  }

  m_token.type = TokenType::SINGLE_LINE_COMMENT;
}

/* -------------------------------------------------------------------------- */
/*                     OTHER - OPERATORS, PUNCTUATION, ETX                    */
/* -------------------------------------------------------------------------- */

void Lexer::buildOther() {
  auto next = m_reader.get();
  m_token.value.push_back(next);

  switch (next) {
    case '-':
      if (m_reader.peek() == '>') {
        m_token.value.push_back(m_reader.get());
        m_token.type = TokenType::ARROW;
      } else {
        m_token.type = TokenType::MINUS;
      }
      return;

    case '=':
      if (m_reader.peek() == '=') {
        m_token.value.push_back(m_reader.get());
        m_token.type = TokenType::EQUALITY;
      } else {
        m_token.type = TokenType::ASSIGNMENT;
      }
      return;

    case '<':
      if (m_reader.peek() == '=') {
        m_token.value.push_back(m_reader.get());
        m_token.type = TokenType::LESS_THAN_EQUAL;
      } else if (m_reader.peek() == '<') {
        m_token.value.push_back(m_reader.get());
        m_token.type = TokenType::INSERTION_OP;
      } else {
        m_token.type = TokenType::LESS_THAN;
      }
      return;

    case '>':
      if (m_reader.peek() == '=') {
        m_token.value.push_back(m_reader.get());
        m_token.type = TokenType::GREATER_THAN_EQUAL;
      } else if (m_reader.peek() == '>') {
        m_token.value.push_back(m_reader.get());
        m_token.type = TokenType::EXTRACTION_OP;
      } else {
        m_token.type = TokenType::GREATER_THAN;
      }
      return;

    case '!':
      if (m_reader.peek() == '=') {
        m_token.value.push_back(m_reader.get());
        m_token.type = TokenType::INEQUALITY;
      } else {
        m_token.type = TokenType::LOGIC_NOT;
      }
      return;

    case '+':
      m_token.type = TokenType::PLUS;
      return;
    case '*':
      m_token.type = TokenType::ASTERISK;
      return;
    case '/':
      m_token.type = TokenType::SLASH;
      return;
    case '%':
      m_token.type = TokenType::PERCENT;
      return;
    case '&':
      m_token.value.push_back(m_reader.get());
      m_token.type = TokenType::LOGIC_AND;
      return;
    case '|':
      m_token.value.push_back(m_reader.get());
      m_token.type = TokenType::LOGIC_OR;
      return;
    case '.':
      m_token.type = TokenType::DOT;
      return;
    case ',':
      m_token.type = TokenType::COMMA;
      return;
    case ':':
      m_token.type = TokenType::COLON;
      return;
    case ';':
      m_token.type = TokenType::SEMICOLON;
      return;
    case '(':
      m_token.type = TokenType::LPAREN;
      return;
    case ')':
      m_token.type = TokenType::RPAREN;
      return;
    case '{':
      m_token.type = TokenType::LBRACE;
      return;
    case '}':
      m_token.type = TokenType::RBRACE;
      return;
    case WEOF:
      m_token.type = TokenType::ETX;
      return;
    default:
      m_token.type = TokenType::UNEXPECTED;  // stray
      return;
  }
}
