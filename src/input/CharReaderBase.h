#pragma once

#include <optional>

#include "Position.h"

class CharReaderBase {
public:
  CharReaderBase();

  std::optional<wchar_t> getNextCharacter();

  std::optional<wchar_t> getCharacter();
  Position getPosition();

  /*
   * @brief Consumes next character from the input stream. This method has to be
   * overriden in derived classes. It operates on std::optional<wchar_t> for
   * unicode support. Returns nullopt if end of file is reached.
   *
   * @return std::optional<wchar_t> - next character from the input stream or
   * nullopt if end of file is reached
   */
  virtual std::optional<wchar_t> next() = 0;

private:
  std::optional<wchar_t> m_char;
  Position m_position;
};
