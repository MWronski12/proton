#pragma once

const int FIRST_COL = 1;
const int FIRST_LINE = 1;

struct Position {
  int line = 1;
  int column = 0;  // Before consuming first character, column is 0

  std::string sourceFile;
};
