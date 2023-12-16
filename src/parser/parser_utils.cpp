#include <algorithm>

#include "parser_utils.h"

bool isPrimitiveType(TokenType tokenType) {
  return std::find(primitiveTypes.cbegin(), primitiveTypes.cend(), tokenType) !=
         primitiveTypes.cend();
}

bool isTypeIdentifier(TokenType tokenType) {
  return isPrimitiveType(tokenType) || tokenType == TokenType::IDENTIFIER;
}
