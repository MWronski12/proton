#include <ostream>

#include "Token.h"
#include "TokenType.h"

std::wostream& operator<<(std::wostream& os, const TokenType& tokenType);
void printTokenInfo(const Token& token);
