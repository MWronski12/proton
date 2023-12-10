#include <sstream>

#include "StrictErrorHandler.h"

void StrictErrorHandler::handleError(const ErrorType type, const Position& position) {
  ErrorHandlerBase::append(type, position);
  ErrorHandlerBase::exitIfErrors();
}
