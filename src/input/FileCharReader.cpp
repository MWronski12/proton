#include "FileCharReader.h"

FileCharReader::FileCharReader(const std::string& filename) : m_stream{std::wifstream(filename)} {
  setCurrentFilename(filename);
}

std::string FileCharReader::getInputFilename() const { return getCurrentFilename(); }

void FileCharReader::load(const std::string& filename) {
  m_stream = std::wifstream(filename);
  setCurrentFilename(filename);
}

wchar_t FileCharReader::peek() const { return m_stream.peek(); }

wchar_t FileCharReader::next() { return m_stream.get(); }
