#include <gmock/gmock.h>

#include "ErrorHandlerBase.h"

class ErrorHandlerMock : public ErrorHandlerBase {
 public:
  MOCK_METHOD(void, handleError, (const ErrorType type, const Position& position), (override));
  MOCK_METHOD(void, exitIfErrors, (), (override));
};
