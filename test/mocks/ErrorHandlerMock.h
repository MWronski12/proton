#include "ErrorHandlerBase.h"

class ErrorHandlerMock : public ErrorHandlerBase {
  void operator()(const ErrorType type, const Position& position, const std::string& sourceFile) {
    (void)type;
    (void)position;
    (void)sourceFile;
  }
};
