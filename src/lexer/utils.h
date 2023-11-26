#include <ostream>

#include "Token.h"
#include "TokenType.h"

void printTokenInfo(const Token& token);
std::wostream& operator<<(std::wostream& os, const TokenType& tokenType);
