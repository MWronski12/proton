#pragma once

#include "CharReaderBase.h"

#include <sstream>

class StringCharReader : public CharReaderBase {
public:
  StringCharReader(const std::wstring &str);
  wchar_t peek();

private:
  wchar_t next() override;

  std::wstringstream m_stream;
  int m_index = 0;
};
