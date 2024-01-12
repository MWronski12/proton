#include <cassert>
#include <unordered_map>
#include <vector>

#include "TypeOperatorsHandler.h"

namespace Interpreter {

static const std::vector<::Operator> ALL_OPERATORS = {
    ::Operator::Add, ::Operator::Sub, ::Operator::Mul, ::Operator::Div, ::Operator::Mod,
    ::Operator::And, ::Operator::Or,  ::Operator::Not, ::Operator::Eq,  ::Operator::Neq,
    ::Operator::Lt,  ::Operator::Gt,  ::Operator::Leq, ::Operator::Geq,
};

// Operators that return the type of their operands
static const std::vector<::Operator> ARITHMETIC_OPERATORS = {
    ::Operator::Add, ::Operator::Sub, ::Operator::Mul, ::Operator::Div, ::Operator::Mod,
};

// Operators that return bool
static const std::vector<::Operator> LOGIC_OPERATORS = {
    ::Operator::And, ::Operator::Or, ::Operator::Not, ::Operator::Eq,  ::Operator::Neq,
    ::Operator::Lt,  ::Operator::Gt, ::Operator::Leq, ::Operator::Geq,
};

bool TypeOperatorsHandler::operandsMatch(const TypePtr& left, const TypePtr& right) const noexcept {
  return left->typeId() == right->typeId();
}

bool TypeOperatorsHandler::operatorIsSupported(const TypePtr& type,
                                               const ::Operator op) const noexcept {
  static const auto typeToValidOpsMap = std::unordered_map<TypeIdentifier, std::vector<::Operator>>{
      {Int::typeId, ALL_OPERATORS},
      {Float::typeId, ALL_OPERATORS},
      {Bool::typeId, LOGIC_OPERATORS},
      {String::typeId,
       {
           ::Operator::Add,
           ::Operator::Eq,
           ::Operator::Neq,
           ::Operator::Lt,
           ::Operator::Gt,
           ::Operator::Leq,
           ::Operator::Geq,
       }},
      {Char::typeId,
       {
           ::Operator::Add,
           ::Operator::Sub,
           ::Operator::Eq,
           ::Operator::Neq,
           ::Operator::Lt,
           ::Operator::Gt,
           ::Operator::Leq,
           ::Operator::Geq,
       }},
      {Struct::typeId, {}},
      {Variant::typeId, {}},
      {FnSignature::typeId, {}},
      {Void::typeId, {}},
  };

  auto typeId = type->typeId();
  const auto typeOps = typeToValidOpsMap.find(typeId);
  assert(typeOps != typeToValidOpsMap.end() && "Unknown type!");

  return std::find(typeOps->second.begin(), typeOps->second.end(), op) != typeOps->second.end();
}

TypeIdentifier TypeOperatorsHandler::resultTypeId(const TypePtr& operandsType,
                                                  const ::Operator op) const noexcept {
  if (std::find(LOGIC_OPERATORS.begin(), LOGIC_OPERATORS.end(), op) != LOGIC_OPERATORS.end()) {
    return Bool::typeId;
  }

  return operandsType->typeId();
}

}  // namespace Interpreter
