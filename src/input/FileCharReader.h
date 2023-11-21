#pragma once

#include <fstream>

#include "CharReaderBase.h"

class FileCharReader : public CharReaderBase {
 public:
  FileCharReader(const std::string& filename);
  wchar_t peek() override;

 private:
  wchar_t next() override;

  std::wifstream m_stream;
};
