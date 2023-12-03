#pragma once

#include <fstream>

#include "CharReaderBase.h"

class FileCharReader : public CharReaderBase {
 public:
  FileCharReader() = default;
  FileCharReader(const std::string& filename);

  std::string getInputFilename() const override;

  wchar_t peek() const override;
  void load(const std::string& str);

 private:
  wchar_t next() override;

  mutable std::wifstream m_stream;
  std::string m_currentFilename;
};
