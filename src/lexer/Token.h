#pragma once

#include <optional>
#include <string>
#include <variant>

#include "Position.h"
#include "TokenType.h"

struct Token {
  TokenType type = TokenType::NO_TOKEN_YET;
  Position position;
  std::wstring representation;
  std::variant<std::monostate, int, float, wchar_t, bool, std::wstring> value;
};
