#pragma once

#include <sstream>
#include <string>

#include "ErrorType.h"
#include "Position.h"

class ErrorHandlerBase {
 public:
  ErrorHandlerBase() = default;
  virtual ~ErrorHandlerBase() = default;

  /*
   * @brief This method is used for signaling encountered errors during
   * compilation and execution.
   */
  virtual void error(ErrorType type, Position position,
                     std::string filename) = 0;

 protected:
  void appendError(ErrorType type, Position position, std::string filename);
  void throwError();

 private:
  std::stringstream m_errorMessage;
};
