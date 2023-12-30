#include <iostream>
#include <sstream>

#include "ErrorHandler.h"

ErrorHandler::ErrorHandler(const int numToleratedErrors)
    : m_numToleratedErrors(numToleratedErrors) {}

/**
 * @brief Signals error or warning.
 *
 * @note ErrorLevel::Error is default and makes the program exit immedietely.
 * @note ErrorLevel::Warning makes the program exit if the number of tolerated warnings is
 * exceeded.
 */
void ErrorHandler::operator()(const ErrorType type, const Position& position, ErrorLevel level) {
  m_numErrors++;
  level == ErrorLevel::Error ? handleError(type, position) : handleWarning(type, position);
}

/**
 * @brief Prints formatted error message and exits.
 */
void ErrorHandler::handleError(const ErrorType type, const Position& position) {
  appendErrorMessage(type, position);
  dumpErrors();
  exit(1);
}

/*
 * @brief Stores warning information. If number of tolerated warnings is exceeded,
 * prints formatted error message and exits.
 */
void ErrorHandler::handleWarning(const ErrorType type, const Position& position) {
  appendErrorMessage(type, position);
  if (m_numErrors >= m_numToleratedErrors) {
    dumpErrors();
    exit(1);
  }
}

bool ErrorHandler::hasErrors() const { return m_numErrors > 0; }

void ErrorHandler::dumpErrors() const { std::cerr << m_errorMessages.str(); }

void ErrorHandler::appendErrorMessage(const ErrorType type, const Position& position) {
  m_errorMessages << formatErrorMessage(type, position);
}

std::string ErrorHandler::formatErrorMessage(const ErrorType type, const Position& position) const {
  std::stringstream message;

  message << "\n"
          << "ERROR in line " << position.line << " col " << position.column << " of "
          << position.sourceFile << "\n"
          << "WHAT? " << Errors.at(type).first << ": " << Errors.at(type).second << "\n";

  return message.str();
}
