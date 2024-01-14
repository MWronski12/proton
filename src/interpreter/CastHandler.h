#pragma once

#include <optional>

#include "Expression.h"
#include "Type.h"
#include "Value.h"

namespace Interpreter {

class CastHandler {
 public:
  bool isPrimitiveType(const TypeIdentifier& type) const;
  bool isCastable(const TypePtr& from, const TypeIdentifier& to) const;
  TypeIdentifier primitiveTypeToTypeId(const ::PrimitiveType type) const;
  Value applyCast(const Value& value, const ::PrimitiveType to) const;
};

}  // namespace Interpreter
