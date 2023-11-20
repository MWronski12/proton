#include "StringCharReader.h"

StringCharReader::StringCharReader(const std::wstring &str)
    : m_stream{str}, CharReaderBase() {}

std::optional<wchar_t> StringCharReader::next() {
  auto character = m_stream.get();

  if (m_stream.eof()) {
    return std::nullopt;
  }

  return character;
}
