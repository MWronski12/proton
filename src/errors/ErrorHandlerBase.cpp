#include "ErrorHandlerBase.h"

#include <iostream>
#include <sstream>

/*
 * @brief Throws runtime error to stop compilation process and provide user with
 * error info. This method should be called by derrived class.
 */
void ErrorHandlerBase::throwError() {
  auto message = m_errorMessage.str();
  std::cerr << message << std::endl;
  std::exit(1);
}

/*
 * @brief Formats and stores signaled error message.
 */
void ErrorHandlerBase::appendError(ErrorType type, Position position, std::string filename) {
  m_errorMessage << "\n"
                 << "ERROR in line " << position.line << " col " << position.column << " of "
                 << filename << "\n"
                 << "WHAT? " << ERROR_MESSAGES[int(type)].first << ": "
                 << ERROR_MESSAGES[int(type)].second << "\n";
}
