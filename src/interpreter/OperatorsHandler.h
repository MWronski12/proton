#pragma once

#include "Expression.h"
#include "Type.h"
#include "Value.h"

namespace Interpreter {

class OperatorsHandler {
 public:
  bool operatorIsSupported(const TypePtr& type, const ::Operator op) const noexcept;
  bool operandsMatch(const TypePtr& left, const TypePtr& right) const noexcept;
  TypeIdentifier resultTypeId(const TypePtr& operandsType, const ::Operator op) const noexcept;

  Value applyOperator(const Value& left, const Value& right, const ::Operator op) const noexcept;
  Value applyOperator(const Value& value, const ::Operator op) const noexcept;
};

}  // namespace Interpreter
