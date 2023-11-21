#pragma once

#include <sstream>

#include "CharReaderBase.h"

class StringCharReader : public CharReaderBase {
 public:
  StringCharReader() = default;
  StringCharReader(const std::wstring& str);

  wchar_t peek();
  void load(const std::wstring& str);

 private:
  wchar_t next() override;

  std::wistringstream m_stream;
  int m_index = 0;
};
