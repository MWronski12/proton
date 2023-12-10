#include "ASTNode.h"
#include "Token.h"

Identifier getIdentifier(const Token& token);
TypeIdentifier getTypeIdentifier(const Token& token);
bool isTypeIdentifier(const Token& token);
