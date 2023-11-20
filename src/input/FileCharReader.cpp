#include "FileCharReader.h"

FileCharReader::FileCharReader(const std::string &filename)
    : m_stream{std::wifstream(filename)}, CharReaderBase() {}

std::optional<wchar_t> FileCharReader::next() {
  auto character = m_stream.get();

  if (m_stream.eof()) {
    return std::nullopt;
  }

  return character;
}
