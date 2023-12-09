#include "StringCharReader.h"

StringCharReader::StringCharReader(const std::wstring& str) : m_stream{str} {}

std::string StringCharReader::getInputFilename() const { return std::string("<Custom string>"); }

void StringCharReader::load(const std::wstring& str) { m_stream = std::wistringstream(str); }

wchar_t StringCharReader::peek() const { return m_stream.peek(); }

wchar_t StringCharReader::next() { return m_stream.get(); }
