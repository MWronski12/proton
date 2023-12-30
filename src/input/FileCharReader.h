#pragma once

#include <fstream>

#include "CharReaderBase.h"

class FileCharReader : public CharReaderBase {
 public:
  FileCharReader() = delete;  // Requires filename
  FileCharReader(const std::string& filename);

  std::string getInputFilename() const override;
  void load(const std::string& filename);
  wchar_t peek() const override;

 private:
  wchar_t next() override;

  mutable std::wifstream m_stream;
  std::string m_currentFilename;
};
