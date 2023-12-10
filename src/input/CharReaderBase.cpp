#include "CharReaderBase.h"

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
    m_position.column = FIRST_COL;
  } else {
    m_position.column++;
  }

  return m_char;
}

/*
 * @brief Should be called by implementors in constructor and on each
 * load operation in order to update sourceFile of tracked.
 *
 * @param filename - new filename
 */
void CharReaderBase::setCurrentFilename(const std::string& filename) {
  m_position.sourceFile = filename;
}
std::string CharReaderBase::getCurrentFilename() const { return m_position.sourceFile; }

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
