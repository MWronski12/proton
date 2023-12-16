#include <typeindex>

#include "TokenType.h"

static const std::vector<TokenType> primitiveTypes = {
    TokenType::INT_KWRD,  TokenType::FLOAT_KWRD,  TokenType::CHAR_KWRD,
    TokenType::BOOL_KWRD, TokenType::STRING_KWRD,
};

static const std::vector<TokenType> literals = {
    TokenType::INTEGER, TokenType::FLOAT, TokenType::CHAR, TokenType::BOOL, TokenType::STRING,
};

static const std::unordered_map<std::type_index, TokenType> primitiveTypeToTokenType = {
    {std::type_index(typeid(int)), TokenType::INTEGER},
    {std::type_index(typeid(float)), TokenType::FLOAT},
    {std::type_index(typeid(bool)), TokenType::BOOL},
    {std::type_index(typeid(wchar_t)), TokenType::CHAR},
    {std::type_index(typeid(std::wstring)), TokenType::STRING},
};

template <typename T>
bool isLiteralT(TokenType tokenType) {
  auto expectedTokenType = primitiveTypeToTokenType.at(std::type_index(typeid(T)));
  return tokenType == expectedTokenType;
}

bool isPrimitiveType(TokenType tokenType);
bool isTypeIdentifier(TokenType tokenType);
