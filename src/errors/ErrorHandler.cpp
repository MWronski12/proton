#include <iostream>
#include <sstream>

#include "ErrorHandler.h"

ErrorHandler::ErrorHandler() : m_numToleratedErrors(1) {}

ErrorHandler::ErrorHandler(const int numToleratedErrors)
    : m_numToleratedErrors(numToleratedErrors) {}

ErrorHandler::~ErrorHandler() { exitIfErrors(); }

/*
 * @brief Calls handleError method of derived class. Just a convenience wrapper, which also allows
 * for testing with mocks.
 */
void ErrorHandler::operator()(const ErrorType type, const Position& position) {
  handleError(type, position);
}

/*
 * @brief method for signaling errors. If error is not tolerated, exits the program.
 */
void ErrorHandler::handleError(const ErrorType type, const Position& position) {
  append(type, position);
  if (--m_numToleratedErrors <= 0) {
    exitIfErrors();
  }
}

/*
 * @brief Prints error message and exits if there are any errors.
 */
void ErrorHandler::exitIfErrors() {
  auto message = m_errors.str();
  if (!message.empty()) {
    std::cerr << message << std::endl;
    std::exit(1);
  }
}

/*
 * @brief Appends formatted error message. Designed to be called from derived.
 */
void ErrorHandler::append(const ErrorType type, const Position& position) {
  m_errors << format(type, position);
}

/*
 * @brief Formats error message including ErorType, position and sourceFile.
 */
std::string ErrorHandler::format(const ErrorType type, const Position& position) const {
  std::stringstream message;

  message << "\n"
          << "ERROR in line " << position.line << " col " << position.column << " of "
          << position.sourceFile << "\n"
          << "WHAT? " << Errors.at(type).first << ": " << Errors.at(type).second << "\n";

  return message.str();
}
