#include "FileCharReader.h"

FileCharReader::FileCharReader(const std::string &filename)
    : m_stream{std::wifstream(filename)}, CharReaderBase() {}

wchar_t FileCharReader::peek() { return m_stream.peek(); }

wchar_t FileCharReader::next() { return m_stream.get(); }
