#pragma once

#include "Position.h"
#include "TokenType.h"

#include <string>

struct Token {
  TokenType type;
  Position position;
  std::wstring value;
};
