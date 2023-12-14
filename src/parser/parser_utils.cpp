#include <algorithm>

#include "parser_utils.h"

bool isPrimitiveType(TokenType tokenType) {
  static const std::vector<TokenType> primitiveTypes = {
      TokenType::INT_KWRD,  TokenType::FLOAT_KWRD,  TokenType::CHAR_KWRD,
      TokenType::BOOL_KWRD, TokenType::STRING_KWRD,
  };

  return std::find(primitiveTypes.cbegin(), primitiveTypes.cend(), tokenType) !=
         primitiveTypes.cend();
}

bool isTypeIdentifier(TokenType tokenType) {
  return isPrimitiveType(tokenType) || tokenType == TokenType::IDENTIFIER;
}
