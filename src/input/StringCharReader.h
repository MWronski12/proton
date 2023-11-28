#pragma once

#include <sstream>

#include "CharReaderBase.h"

class StringCharReader : public CharReaderBase {
 public:
  StringCharReader() = default;
  StringCharReader(const std::wstring& str);

  wchar_t peek() const override;
  void load(const std::wstring& str);

 private:
  wchar_t next() override;

  mutable std::wistringstream m_stream;
};