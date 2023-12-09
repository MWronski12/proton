#pragma once

#include "ErrorHandlerBase.h"

/*
 * @brief StrictErrorHandler finishes compilation process when the first error
 * is signaled.
 */
class StrictErrorHandler : public ErrorHandlerBase {
 public:
  void operator()(const ErrorType type, const Position& position,
                  const std::string& sourceFile) override;
};
