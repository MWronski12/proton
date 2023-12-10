#include <algorithm>

#include "parser_utils.h"

static const std::vector<std::wstring> primitiveTypes = {
    L"int", L"float", L"bool", L"char", L"string",
};

Identifier getIdentifier(const Token& token) { return token.value; }
TypeIdentifier getTypeIdentifier(const Token& token) { return token.value; }

bool isPrimitiveType(const Token& token) {
  return std::find(primitiveTypes.cbegin(), primitiveTypes.cend(), token.value) !=
         primitiveTypes.cend();
}
bool isTypeIdentifier(const Token& token) {
  return isPrimitiveType(token) || token.type == TokenType::IDENTIFIER;
}