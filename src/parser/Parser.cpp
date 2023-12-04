#include "Parser.h"

Parser::Parser(Lexer& lexer, ErrorHandlerBase& errorHandler)
    : m_lexer{lexer}, m_errorHandler{errorHandler} {}

std::optional<Program> Parser::parseProgram() { return std::nullopt; }
