#include "StringCharReader.h"

StringCharReader::StringCharReader(const std::wstring& str)
    : CharReaderBase(), m_stream{str} {}

wchar_t StringCharReader::peek() const { return m_stream.peek(); }

wchar_t StringCharReader::next() { return m_stream.get(); }

void StringCharReader::load(const std::wstring& str) {
  m_stream = std::wistringstream(str);
}
