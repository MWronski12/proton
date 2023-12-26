#pragma once

#include <sstream>
#include <string>

#include "ErrorType.h"
#include "Position.h"

enum class ErrorLevel { Error, Warning };

class ErrorHandler {
 public:
  ErrorHandler(const ErrorHandler&) = delete;
  ErrorHandler(ErrorHandler&&) = delete;
  ErrorHandler& operator=(const ErrorHandler&) = delete;
  ErrorHandler& operator=(ErrorHandler&&) = delete;

  ErrorHandler(int numToleratedErrors);
  ErrorHandler() = default;
  ~ErrorHandler() = default;

  void operator()(const ErrorType type, const Position& position,
                  ErrorLevel level = ErrorLevel::Error);

 protected:
  virtual void handleError(const ErrorType type, const Position& position);
  virtual void handleWarning(const ErrorType type, const Position& position);

 private:
  bool hasErrors() const;
  void dumpErrors() const;
  void appendErrorMessage(const ErrorType type, const Position& position);
  std::string formatErrorMessage(const ErrorType type, const Position& position) const;

  int m_numErrors = 0;
  const int m_numToleratedErrors = 10;
  std::stringstream m_errorMessages;
};
