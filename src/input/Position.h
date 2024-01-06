#pragma once

const int FIRST_COL = 1;
const int FIRST_LINE = 1;

struct Position {
  int line = FIRST_LINE;
  int column = FIRST_COL;

  std::string sourceFile;
};
