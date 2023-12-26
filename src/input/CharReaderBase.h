#pragma once

#include <limits>
#include <string>

#include "Position.h"

const wchar_t NO_CHAR_YET = std::numeric_limits<wchar_t>::max();

class CharReaderBase {
 public:
  CharReaderBase(const CharReaderBase&) = delete;
  CharReaderBase(CharReaderBase&&) = delete;
  CharReaderBase& operator=(const CharReaderBase&) = delete;
  CharReaderBase& operator=(CharReaderBase&&) = delete;

  CharReaderBase() = default;
  virtual ~CharReaderBase() = default;

  wchar_t get();
  wchar_t last() const;
  Position pos() const;

  /**
   * @brief Implementors must provide function to get current input file descriptor for tracking
   * position.
   */
  virtual std::string getInputFilename() const = 0;

  /**
   * @brief Implementors must provide a way to Look up next character in the input stream. This
   * method has to be overriden in derived classes Must return WEOF when end of stream is reached.
   *
   * @return wchar_t - next character from the input stream
   */
  virtual wchar_t peek() const = 0;

 protected:
  void setCurrentFilename(const std::string& filename);
  std::string getCurrentFilename() const;

 private:
  /**
   * @brief Implementors must provide a way to consume next character from the input stream. This
   * method has to be overriden in derived classes. Must return WEOF when end of stream is reached.
   *
   * @return wchar_t - next character from the input stream
   */
  virtual wchar_t next() = 0;

  wchar_t m_char = NO_CHAR_YET;
  Position m_position;
};
