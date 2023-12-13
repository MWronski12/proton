#pragma once

#include <sstream>
#include <string>

#include "ErrorType.h"
#include "Position.h"

class ErrorHandler {
 public:
  ErrorHandler(const ErrorHandler&) = delete;
  ErrorHandler(ErrorHandler&&) = delete;
  ErrorHandler& operator=(const ErrorHandler&) = delete;
  ErrorHandler& operator=(ErrorHandler&&) = delete;

  ErrorHandler();
  ErrorHandler(int numToleratedErrors);
  ~ErrorHandler();

  void operator()(const ErrorType type, const Position& position);
  virtual void handleError(const ErrorType type, const Position& position);

 private:
  void append(const ErrorType type, const Position& position);
  std::string format(const ErrorType type, const Position& position) const;
  void exitIfErrors();

  int m_numToleratedErrors;
  std::stringstream m_errors;
};
