#pragma once

#include <sstream>
#include <string>

#include "ErrorType.h"
#include "Position.h"

class ErrorHandlerBase {
 public:
  ErrorHandlerBase(const ErrorHandlerBase&) = delete;
  ErrorHandlerBase(ErrorHandlerBase&&) = delete;
  ErrorHandlerBase& operator=(const ErrorHandlerBase&) = delete;
  ErrorHandlerBase& operator=(ErrorHandlerBase&&) = delete;

  ErrorHandlerBase() = default;
  virtual ~ErrorHandlerBase() = default;

  void operator()(const ErrorType type, const Position& position);

  /*
   * @brief This method is used for signaling encountered errors during
   * compilation and execution. Error handlers can add some logic here. The
   * easiest ones are StrictErrorHandler calling assert on first signaled error
   * and SilentErrorHandler which calls assert only after N errors are
   * encountered.
   */
  virtual void handleError(const ErrorType type, const Position& position) = 0;

  virtual void exitIfErrors();

 protected:
  void append(const ErrorType type, const Position& position);

 private:
  std::string format(const ErrorType type, const Position& position) const;

  std::stringstream m_errors;
};
