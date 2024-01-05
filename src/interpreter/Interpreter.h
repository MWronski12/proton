#pragma once

#include "CharReaderBase.h"
#include "Environment.h"
#include "Lexer.h"
#include "Parser.h"
#include "SemanticAnalyzer.h"

namespace Interpreter {

class Interpreter : ASTVisitor {
 public:
  Interpreter(CharReaderBase& charReader) : m_charReader{charReader} {}

  void run() {
    auto program = m_parser.parseProgram();
    if (program.has_value()) {
      program->accept(m_semanticAnalyzer);
      //   program->accept(*this);
    }
  }

 private:
  CharReaderBase& m_charReader;

  ErrorHandler m_errorHandler;
  Lexer m_lexer{m_charReader, m_errorHandler};
  Parser m_parser{m_lexer, m_errorHandler};
  Environment m_env{};
  SemanticAnalyzer m_semanticAnalyzer{m_env, m_errorHandler};
};

}  // namespace Interpreter
