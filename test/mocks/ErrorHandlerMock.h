#include <gmock/gmock.h>

#include "ErrorHandler.h"

class ErrorHandlerMock : public ErrorHandler {
 public:
  MOCK_METHOD(void, handleError, (const ErrorType type, const Position& position), (override));
};
