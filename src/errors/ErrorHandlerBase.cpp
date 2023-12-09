#include <iostream>
#include <sstream>

#include "ErrorHandlerBase.h"

/*
 * @brief Throws runtime error to if internal error message is not empty. Must
 * be explicitly called at the end of compilation.
 */
void ErrorHandlerBase::exitIfErrors() {
  auto message = m_errors.str();
  if (!message.empty()) {
    std::cerr << message << std::endl;
    std::exit(1);
  }
}

/*
 * @brief Appends formatted error message. Designed to be called from derived.
 */
void ErrorHandlerBase::append(const ErrorType type, const Position& position,
                              const std::string& sourceFile) {
  m_errors << format(type, position, sourceFile);
}

/*
 * @brief Formats error message including ErorType, position and sourceFile.
 */
std::string ErrorHandlerBase::format(const ErrorType type, const Position& position,
                                     const std::string& sourceFile) const {
  std::stringstream message;

  message << "\n"
          << "ERROR in line " << position.line << " col " << position.column << " of " << sourceFile
          << "\n"
          << "WHAT? " << ERROR_MESSAGES[int(type)].first << ": " << ERROR_MESSAGES[int(type)].second
          << "\n";

  return message.str();
}
