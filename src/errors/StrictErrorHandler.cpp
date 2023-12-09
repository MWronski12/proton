#include <sstream>

#include "StrictErrorHandler.h"

void StrictErrorHandler::operator()(const ErrorType type,
                                    const Position& position,
                                    const std::string& sourceFile) {
  ErrorHandlerBase::append(type, position, sourceFile);
  ErrorHandlerBase::exitIfErrors();
}
