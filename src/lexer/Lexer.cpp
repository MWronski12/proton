#include <algorithm>
#include <cwctype>
#include <functional>
#include <map>
#include <stdexcept>
#include <string>

#include "Lexer.h"
#include "Token.h"
#include "utils.h"

Lexer::Lexer(CharReaderBase& reader, ErrorHandlerBase& errorHandler)
    : m_reader{reader}, m_errorHandler{errorHandler} {}

Token Lexer::getNextToken() {
  skipWhiteSpaces();
  buildToken();
  return m_token;
}

std::string Lexer::getInputFilename() const { return m_reader.getInputFilename(); }

void Lexer::skipWhiteSpaces() {
  while (iswspace(m_reader.peek())) m_reader.get();
}

void Lexer::buildToken() {
  m_token = Token{};
  m_token.position = m_reader.pos();

  auto first = m_reader.peek();
  if (isIdentifierStart(first)) {
    buildIdentifier();
  } else if (isNumberStart(first)) {
    buildNumber();
  } else if (isStringStart(first)) {
    buildString();
  } else if (isCharStart(first)) {
    buildChar();
  } else if (isCommentStart(first)) {
    buildComment();
  } else {
    buildOther();
  }
}

/* -------------------------------------------------------------------------- */
/*                            IDENTIFIER OR KEYWORD                           */
/* -------------------------------------------------------------------------- */

bool Lexer::isIdentifierStart(wchar_t first) { return first == L'_' || iswalpha(first); }

void Lexer::buildIdentifier() {
  if (!isIdentifierStart(m_reader.peek())) {
    throw std::logic_error("Identifier must start with a '_' or alpabet char!");
  }

  while (isIdentifierChar(m_reader.peek())) {
    auto next = m_reader.get();
    m_token.value.push_back(next);
  }

  matchIdentifier();
}

bool Lexer::isIdentifierChar(const wchar_t c) const { return iswalnum(c); }

void Lexer::matchIdentifier() {
  // Keyword
  for (uint i = 0; i < KEYWORDS.size(); i++) {
    if (m_token.value == KEYWORDS[i]) {
      m_token.type = TokenType(i + KEYWORDS_OFFSET);
      return;
    }
  }

  // User defined identifier
  m_token.type = TokenType::IDENTIFIER;
}

/* -------------------------------------------------------------------------- */
/*                              FLOAT OR INTEGER                              */
/* -------------------------------------------------------------------------- */

bool Lexer::isNumberStart(wchar_t first) { return iswdigit(first); }

void Lexer::buildNumber() {
  if (!isNumberStart(m_reader.peek())) {
    throw std::logic_error("Number literal must start with a digit character!");
  }

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
    m_errorHandler(ErrorType::INVALID_NUMBER_LITERAL, m_token.position,
                   m_reader.getInputFilename());
  }

  validateBuiltNumber();
}

void Lexer::matchNumber() {
  if (m_token.value.find('.') != std::wstring::npos) {
    m_token.type = TokenType::FLOAT;
    m_token.floatValue = std::stof(m_token.value);
  } else {
    m_token.type = TokenType::INTEGER;
    m_token.intValue = std::stoi(m_token.value);
  }
}

void Lexer::validateBuiltNumber() {
  if (m_token.type == TokenType::UNEXPECTED) return;
  if (m_token.value.length() == 0) throw std::logic_error("Built token is empty!");

  // Number starting with 0 must be either int 0 or float 0.xxx
  if (m_token.value.front() == L'0' && m_token.value.length() > 1 && m_token.value[1] != L'.') {
    m_token.type = TokenType::UNEXPECTED;
    m_errorHandler(ErrorType::INVALID_NUMBER_LITERAL, m_token.position,
                   m_reader.getInputFilename());
  }

  // Number literal can have 0 or 1 '.'
  else if (std::count(m_token.value.cbegin(), m_token.value.cend(), L'.') > 1) {
    m_token.type = TokenType::UNEXPECTED;
    m_errorHandler(ErrorType::INVALID_NUMBER_LITERAL, m_token.position,
                   m_reader.getInputFilename());
  }
}

bool Lexer::isNumberChar(const wchar_t c) const { return iswdigit(c) || c == L'.'; }

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

bool Lexer::isStringStart(wchar_t first) { return first == L'"'; }

void Lexer::buildString() {
  if (!isStringStart(m_reader.peek())) {
    throw std::logic_error("String literal must start with L'\"' character!");
  }

  m_reader.get();  // Consume opening quote

  while (true) {
    auto next = m_reader.peek();

    // Closing quote
    if (next == L'"') {
      m_reader.get();  // Consume closing quote
      m_token.type = TokenType::STRING;
      m_token.strValue = m_token.value;
      return;
    }
    // Newline or WEOF
    else if (next == L'\n' || next == wchar_t(WEOF)) {
      m_token.type = TokenType::UNEXPECTED;
      m_errorHandler(ErrorType::MISSING_CLOSING_QUOTE, m_token.position,
                     m_reader.getInputFilename());
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

bool Lexer::isCharStart(wchar_t first) { return first == L'\''; }

void Lexer::buildChar() {
  if (!isCharStart(m_reader.peek())) {
    throw std::logic_error("Char literal must start with L'\'' character!");
  }

  m_reader.get();  // Consume opening quote

  while (true) {
    auto next = m_reader.peek();

    // Closing quote
    if (next == L'\'') {
      m_reader.get();
      if (m_token.value.length() == 1) {
        m_token.type = TokenType::CHAR;
        m_token.charValue = m_token.value.front();
      } else {
        m_token.type = TokenType::UNEXPECTED;
        m_errorHandler(ErrorType::INVALID_CHAR_LITERAL, m_token.position,
                       m_reader.getInputFilename());
      }
      return;
    }
    // Newline or WEOF
    else if (next == L'\n' || next == wchar_t(WEOF)) {
      m_token.type = TokenType::UNEXPECTED;
      m_errorHandler(ErrorType::MISSING_CLOSING_QUOTE, m_token.position,
                     m_reader.getInputFilename());
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

bool Lexer::isCommentStart(wchar_t first) { return first == L'$'; }

void Lexer::buildComment() {
  if (!isCommentStart(m_reader.peek())) {
    throw std::logic_error("Comment must start with L'$' character!");
  }

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
      m_token.type = TokenType::UNEXPECTED;
      m_errorHandler(ErrorType::UNEXPECTED_END_OF_FILE, m_token.position,
                     m_reader.getInputFilename());
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

  switch (next) {  // Cases where first char doesnt uniquely identify the token
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

    // Cases where first char uniquely identifies the token
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
      m_errorHandler(ErrorType::UNEXPECTED_CHARACTER, m_token.position,
                     m_reader.getInputFilename());
      return;
  }
}
