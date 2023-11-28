#include "FileCharReader.h"

FileCharReader::FileCharReader(const std::string& filename)
    : CharReaderBase(), m_stream{std::wifstream(filename)} {}

wchar_t FileCharReader::peek() const { return m_stream.peek(); }

wchar_t FileCharReader::next() { return m_stream.get(); }
