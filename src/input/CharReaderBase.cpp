#include "CharReaderBase.h"

CharReaderBase::CharReaderBase() : m_position{0, 0} {}

/*
 * @brief Gets next character from the input stream and advances
 * internal position.
 *
 * @return wchar_t - next character from the input stream
 */
wchar_t CharReaderBase::get() {
  m_char = next();

  if (m_char == L'\n') {
    m_position.line++;
    m_position.column = LINE_START;
  } else {
    m_position.column++;
  }

  return m_char;
}

/*
 * @brief Gets last consumed character.
 * Before consuming first character, returns NO_CHAR_YET.
 * After the end of file was reached returns WEOF.
 *
 * @return wchar_t - last consumed character in the input stream
 */
wchar_t CharReaderBase::last() const { return m_char; }

/*
 * @return Position - current position in the input stream
 */
Position CharReaderBase::pos() const { return m_position; }
