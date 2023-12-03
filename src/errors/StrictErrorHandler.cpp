#include "StrictErrorHandler.h"

#include <sstream>

void StrictErrorHandler::error(ErrorType type, Position position,
                               std::string filename) {
  appendError(type, position, filename);
  throwError();
}
