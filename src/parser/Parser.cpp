#include "Parser.h"

Parser::Parser(Lexer& lexer, ErrorHandlerBase& errorHandler)
    : m_lexer{lexer}, m_errorHandler{errorHandler} {
  consumeToken();
}

void Parser::consumeToken() { m_token = m_lexer.getNextToken(); }

/*
 * @brief Recover from syntax error by skipping tokens until semicolon or end of text. After calling
 * skipSyntaxError m_token is set to first token of the next statement or end of text.
 */
void Parser::skipSyntaxError() {
  do {
    consumeToken();
  } while (m_token.type != TokenType::SEMICOLON && m_token.type != TokenType::ETX);

  if (m_token.type == TokenType::SEMICOLON) {
    consumeToken();
  }
}
