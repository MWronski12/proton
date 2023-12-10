#pragma once

#include <optional>
#include <string>
#include <variant>

#include "Position.h"
#include "TokenType.h"

struct Token {
  TokenType type = TokenType::NO_TOKEN_YET;
  Position position;
  std::wstring readValue;
  std::variant<std::wstring, wchar_t, int, float, bool> value;
};
