#pragma once

#include <optional>
#include <string>

#include "Position.h"
#include "TokenType.h"

struct Token {
  TokenType type = TokenType::NO_TOKEN_YET;
  Position position;
  std::wstring value;

  std::optional<std::wstring> strValue;
  std::optional<wchar_t> charValue;
  std::optional<int> intValue;
  std::optional<float> floatValue;
};
