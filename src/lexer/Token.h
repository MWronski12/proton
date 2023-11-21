#pragma once

#include <string>

#include "Position.h"
#include "TokenType.h"

struct Token {
  TokenType type;
  Position position;
  std::wstring value;
};
