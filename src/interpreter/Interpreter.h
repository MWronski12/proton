// #pragma once

// #include "CharReaderBase.h"
// #include "Lexer.h"
// #include "Parser.h"
// #include "ScopeAnalyzer.h"

// namespace Interpreter {

// class Interpreter {
//  public:
//   Interpreter(CharReaderBase& charReader) : m_charReader{charReader} {}

//   void run() {
//     auto program = m_parser.parseProgram();
//     if (program.has_value()) {
//       m_scopeAnalyzer.visit(*program);
//     }
//   }

//  private:
//   CharReaderBase& m_charReader;
//   ErrorHandler m_errorHandler{};
//   Lexer m_lexer{m_charReader, m_errorHandler};
//   Parser m_parser{m_lexer, m_errorHandler};
//   Environment m_env{};
//   ScopeAnalyzer m_scopeAnalyzer{m_env, m_errorHandler};
// };

// }  // namespace Interpreter
