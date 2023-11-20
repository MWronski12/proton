#include "CharReaderBase.h"

CharReaderBase::CharReaderBase() : m_char{std::nullopt}, m_position{0, 0} {};

/*
 * @brief Consumes next character from the input stream and advances
 * internal position. Returns nullopt if end of file is reached.
 *
 * @return std::optional<wchar_t> - next character from the input stream or
 * nullopt if end of file is reached
 */
std::optional<wchar_t> CharReaderBase::getNextCharacter() {
  m_char = next();

  if (m_char == L'\n') {
    m_position.line++;
    m_position.column = 0;
  } else {
    m_position.column++;
  }

  return m_char;
};

/*
 * @brief Returns current character in the input stream. Returns nullopt if
 * getNextCharacter() was not called before or end of file is reached.
 *
 * @return std::optional<wchar_t> - current character in the input stream
 */
std::optional<wchar_t> CharReaderBase::getCharacter() { return m_char; };

/*
 * @return Position - current position in the input stream
 */
Position CharReaderBase::getPosition() { return m_position; };
