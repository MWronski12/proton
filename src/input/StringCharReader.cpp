#include "StringCharReader.h"

StringCharReader::StringCharReader(const std::wstring &str)
    : m_stream{str}, CharReaderBase() {}

wchar_t StringCharReader::peek() { return m_stream.peek(); }

wchar_t StringCharReader::next() { return m_stream.get(); }
