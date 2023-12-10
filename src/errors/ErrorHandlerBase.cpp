#include <iostream>
#include <sstream>

#include "ErrorHandlerBase.h"

/*
 * @brief Calls handleError method of derived class. Just a convenience wrapper, which also allows
 * for testing with mocks.
 */
void ErrorHandlerBase::operator()(const ErrorType type, const Position& position) {
  handleError(type, position);
}

/*
 * @brief Checks if there are any errors and exits if so.
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
void ErrorHandlerBase::append(const ErrorType type, const Position& position) {
  m_errors << format(type, position);
}

/*
 * @brief Formats error message including ErorType, position and sourceFile.
 */
std::string ErrorHandlerBase::format(const ErrorType type, const Position& position) const {
  std::stringstream message;

  message << "\n"
          << "ERROR in line " << position.line << " col " << position.column << " of "
          << position.sourceFile << "\n"
          << "WHAT? " << ERROR_MESSAGES[int(type)].first << ": " << ERROR_MESSAGES[int(type)].second
          << "\n";

  return message.str();
}
