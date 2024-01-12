#include <optional>

#include "Expression.h"
#include "Type.h"

namespace Interpreter {

class TypeCastHandler {
 public:
  bool isPrimitiveType(const TypeIdentifier& type) const noexcept;
  bool isCastable(const TypePtr& from, const TypeIdentifier& to) const noexcept;
  TypeIdentifier primitiveTypeToTypeId(const ::PrimitiveType type) const noexcept;
};

}  // namespace Interpreter
