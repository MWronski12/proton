#include "FileCharReader.h"

FileCharReader::FileCharReader(const std::string& filename)
    : m_stream{std::wifstream(filename)}, m_currentFilename{filename} {}

std::string FileCharReader::getInputFilename() const {
  return m_currentFilename;
}

void FileCharReader::load(const std::string& filename) {
  m_stream = std::wifstream(filename);
  m_currentFilename = filename;
}

wchar_t FileCharReader::peek() const { return m_stream.peek(); }

wchar_t FileCharReader::next() { return m_stream.get(); }
