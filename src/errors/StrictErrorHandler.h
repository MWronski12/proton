#pragma once

#include "ErrorHandlerBase.h"

/*
 * @brief StrictErrorHandler finishes compilation process when the first error
 * is signaled.
 */
class StrictErrorHandler : public ErrorHandlerBase {
 public:
  void error(ErrorType type, Position position, std::string filename) override;
};
