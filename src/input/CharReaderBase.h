#pragma once

#include <limits>

#include "Position.h"

const wchar_t NO_CHAR_YET = std::numeric_limits<wchar_t>::max();

class CharReaderBase {
 public:
  CharReaderBase();

  wchar_t get();
  wchar_t last() const;
  Position pos() const;

  /*
   * @brief Consumes next character from the input stream. This method has to be
   * overriden in derived classes. Must return WEOF when end of stream is
   * reached.
   *
   * @return wchar_t - next character from the input stream
   */
  virtual wchar_t next() = 0;

  /*
   * @brief Looks up next character in the input stream. This method has to be
   * overriden in derived classes Must return WEOF when end of stream is
   * reached.
   *
   * @return wchar_t - next character from the input stream
   */
  virtual wchar_t peek() const = 0;

 private:
  wchar_t m_char = NO_CHAR_YET;
  Position m_position;
};
