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
   * compilation and execution. Error handlers can add some logic here. The
   * easiest ones are StrictErrorHandler calling assert on first signaled error
   * and SilentErrorHandler which calls assert only after N errors are
   * encountered.
   */
  virtual void operator()(const ErrorType type, const Position& position,
                          const std::string& sourceFile) = 0;

  void exitIfErrors();

 protected:
  void append(const ErrorType type, const Position& position,
              const std::string& sourceFile);

 private:
  std::string format(const ErrorType type, const Position& position,
                     const std::string& sourceFile) const;

  std::stringstream m_errors;
};
