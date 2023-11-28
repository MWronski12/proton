#include <algorithm>
#include <cwctype>
#include <stdexcept>
#include <string>

#include "Lexer.h"
#include "Token.h"
#include "utils.h"

Lexer::Lexer(CharReaderBase& reader) : m_reader{reader}, m_token{} {}

Token Lexer::getNextToken() {
  skipWhiteSpaces();
  return buildToken();
}

Token Lexer::getToken() const { return m_token; }

void Lexer::skipWhiteSpaces() {
  while (iswspace(m_reader.peek())) m_reader.get();
}

Token Lexer::buildToken() {
  m_token.value.clear();
  m_token.position = m_reader.pos();

  auto first = m_reader.peek();
  if (first == L'_' || iswalpha(first)) {
    buildIdentifier();
  } else if (iswdigit(first)) {
    buildNumber();
  } else if (first == L'"') {
    buildString();
  } else if (first == L'\'') {
    buildChar();
  } else if (first == L'$') {
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

bool Lexer::isIdentifierChar(const wchar_t c) const { return iswalnum(c); }

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
  else if (m_token.value == L"true")
    m_token.type = TokenType::BOOL;
  else if (m_token.value == L"false")
    m_token.type = TokenType::BOOL;
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

  // Tokens like: "3x", "3(" etc. are not allowed
  if (isAllowedAfterNumber(m_reader.peek())) {
    matchNumber();
  }
  // We wanted to match number, but it was invalidated (ex "3x"). Lets skip to a
  // valid evident of number literal end (space, colon, operator etc.) and
  // consider this as an unexpected token.
  else {
    while (!isAllowedAfterNumber(m_reader.peek())) {
      auto next = m_reader.get();
      m_token.value.push_back(next);
    }
    m_token.type = TokenType::UNEXPECTED;
  }

  validateBuiltNumber();
}

void Lexer::matchNumber() {
  if (m_token.value.find('.') != std::wstring::npos) {
    m_token.type = TokenType::FLOAT;
  } else {
    m_token.type = TokenType::INTEGER;
  }
}

void Lexer::validateBuiltNumber() {
  if (m_token.type == TokenType::UNEXPECTED) return;
  if (m_token.value.length() == 0)
    throw std::logic_error("Built token is empty!");

  // Number starting with 0 must be either int 0 or float 0.xxx
  if (m_token.value.front() == L'0' && m_token.value.length() > 1 &&
      m_token.value[1] != L'.') {
    m_token.type = TokenType::UNEXPECTED;
  }

  // Number literal can have 0 or 1 '.'
  else if (std::count(m_token.value.cbegin(), m_token.value.cend(), L'.') > 1) {
    m_token.type = TokenType::UNEXPECTED;
  }
}

bool Lexer::isNumberChar(const wchar_t c) const {
  return iswdigit(c) || c == L'.';
}

bool Lexer::isAllowedAfterNumber(const wchar_t c) const {
  bool isAllowed = false;
  switch (c) {
    // whitespace, right parenthese, semicolon, comma, comment or EOF
    case L' ':
    case L'\n':
    case L'\t':
    case L')':
    case L']':
    case L'}':
    case L';':
    case L',':
    case L'$':
    case WEOF:
      isAllowed = true;
      break;

    default:
      // Any operator
      for (const auto& el : OPERATORS) {
        if (c == el.front()) {
          isAllowed = true;
          break;
        }
      }
  }

  return isAllowed;
}

/* -------------------------------------------------------------------------- */
/*                               STRING LITERAL                               */
/* -------------------------------------------------------------------------- */

void Lexer::buildString() {
  // Consume opening quote
  if (m_reader.get() != L'\"') {
    throw std::logic_error("String literal must begin with L'\"' character!");
  }

  while (true) {
    auto next = m_reader.peek();

    // Closing quote
    if (next == L'"') {
      m_reader.get();  // Consume closing quote
      m_token.type = TokenType::STRING;
      return;
    }
    // Newline or WEOF
    else if (next == L'\n' || next == wchar_t(WEOF)) {
      m_token.type = TokenType::UNEXPECTED;
      return;
    }
    // Escape sequence
    else if (next == L'\\') {
      m_reader.get();  // skip backslash
      auto escapedChar = m_reader.get();
      addEscapedChar(escapedChar);
    }
    // Valid string literal character
    else {
      m_token.value.push_back(m_reader.get());
    }
  }
}

void Lexer::addEscapedChar(const wchar_t c) {
  switch (c) {
    case L'n':
      m_token.value.push_back(L'\n');
      break;
    case L't':
      m_token.value.push_back(L'\t');
      break;
    default:
      m_token.value.push_back(c);
  }
}

/* -------------------------------------------------------------------------- */
/*                                CHAR LITERAL                                */
/* -------------------------------------------------------------------------- */

void Lexer::buildChar() {
  if (m_reader.get() != L'\'') {  // Consume opening quote
    throw std::logic_error("Char literal must start with L'\'' character!");
  }

  while (true) {
    auto next = m_reader.peek();

    // Closing quote
    if (next == L'\'') {
      m_reader.get();  // Consume closing quote
      m_token.type =
          m_token.value.length() == 1 ? TokenType::CHAR : TokenType::UNEXPECTED;
      return;
    }
    // Newline or WEOF
    else if (next == L'\n' || next == wchar_t(WEOF)) {
      m_token.type = TokenType::UNEXPECTED;
      return;
    }
    // Escape sequence
    else if (next == L'\\') {
      m_reader.get();  // skip backslash
      auto escapedChar = m_reader.get();
      addEscapedChar(escapedChar);
    }
    // Valid char literal character
    else {
      m_token.value.push_back(m_reader.get());
    }
  }
}

/* -------------------------------------------------------------------------- */
/*                                   COMMENT                                  */
/* -------------------------------------------------------------------------- */

void Lexer::buildComment() {
  if (m_reader.peek() != L'$')
    throw std::logic_error("Comment must start with L'$' character!");

  m_token.value.push_back(m_reader.get());

  if (m_reader.peek() == L'$') {
    m_token.value.push_back(m_reader.get());
    matchMultiLineComment();
  } else {
    matchSingleLineComment();
  }
}

void Lexer::matchMultiLineComment() {
  while (true) {
    auto next = m_reader.get();

    if (next == wchar_t(WEOF)) {
      m_token.type = TokenType::UNEXPECTED;  // missing closing comment
      return;
    }

    if (next == L'$' && m_reader.get() == L'$') {
      m_token.value.push_back('$');
      m_token.value.push_back('$');
      break;
    }

    m_token.value.push_back(next);
  }

  m_token.type = TokenType::MULTI_LINE_COMMENT;
}

void Lexer::matchSingleLineComment() {
  while (m_reader.peek() != L'\n' && m_reader.peek() != wchar_t(WEOF)) {
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
    case L'-':
      if (m_reader.peek() == L'>') {
        m_token.value.push_back(m_reader.get());
        m_token.type = TokenType::ARROW;
      } else {
        m_token.type = TokenType::MINUS;
      }
      return;

    case L'=':
      if (m_reader.peek() == L'=') {
        m_token.value.push_back(m_reader.get());
        m_token.type = TokenType::EQUALITY;
      } else {
        m_token.type = TokenType::ASSIGNMENT;
      }
      return;

    case L'<':
      if (m_reader.peek() == L'=') {
        m_token.value.push_back(m_reader.get());
        m_token.type = TokenType::LESS_THAN_EQUAL;
      } else if (m_reader.peek() == L'<') {
        m_token.value.push_back(m_reader.get());
        m_token.type = TokenType::INSERTION_OP;
      } else {
        m_token.type = TokenType::LESS_THAN;
      }
      return;

    case L'>':
      if (m_reader.peek() == L'=') {
        m_token.value.push_back(m_reader.get());
        m_token.type = TokenType::GREATER_THAN_EQUAL;
      } else if (m_reader.peek() == L'>') {
        m_token.value.push_back(m_reader.get());
        m_token.type = TokenType::EXTRACTION_OP;
      } else {
        m_token.type = TokenType::GREATER_THAN;
      }
      return;

    case L'!':
      if (m_reader.peek() == L'=') {
        m_token.value.push_back(m_reader.get());
        m_token.type = TokenType::INEQUALITY;
      } else {
        m_token.type = TokenType::LOGIC_NOT;
      }
      return;

    case L'+':
      m_token.type = TokenType::PLUS;
      return;
    case L'*':
      m_token.type = TokenType::ASTERISK;
      return;
    case L'/':
      m_token.type = TokenType::SLASH;
      return;
    case L'%':
      m_token.type = TokenType::PERCENT;
      return;
    case L'&':
      m_token.value.push_back(m_reader.get());
      m_token.type = TokenType::LOGIC_AND;
      return;
    case L'|':
      m_token.value.push_back(m_reader.get());
      m_token.type = TokenType::LOGIC_OR;
      return;
    case L'.':
      m_token.type = TokenType::DOT;
      return;
    case L',':
      m_token.type = TokenType::COMMA;
      return;
    case L':':
      m_token.type = TokenType::COLON;
      return;
    case L';':
      m_token.type = TokenType::SEMICOLON;
      return;
    case L'(':
      m_token.type = TokenType::LPAREN;
      return;
    case L')':
      m_token.type = TokenType::RPAREN;
      return;
    case L'{':
      m_token.type = TokenType::LBRACE;
      return;
    case L'}':
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
