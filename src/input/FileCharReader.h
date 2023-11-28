#pragma once

#include <fstream>

#include "CharReaderBase.h"

class FileCharReader : public CharReaderBase {
 public:
  FileCharReader(const std::string& filename);
  wchar_t peek() const override;

 private:
  wchar_t next() override;

  mutable std::wifstream m_stream;
};