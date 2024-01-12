#include "Expression.h"
#include "Type.h"

namespace Interpreter {

class TypeOperatorsHandler {
 public:
  bool operatorIsSupported(const TypePtr& type, const ::Operator op) const noexcept;
  bool operandsMatch(const TypePtr& left, const TypePtr& right) const noexcept;
  TypeIdentifier resultTypeId(const TypePtr& operandsType, const ::Operator op) const noexcept;
};

}  // namespace Interpreter
