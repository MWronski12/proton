#include <cassert>
#include <functional>
#include <unordered_map>
#include <variant>
#include <vector>

#include "OperatorsHandler.h"
#include "interpreter_utils.h"

namespace Interpreter {

Value operator+(const Value& left, const Value& right);
Value operator-(const Value& left, const Value& right);
Value operator*(const Value& left, const Value& right);
Value operator/(const Value& left, const Value& right);
Value operator%(const Value& left, const Value& right);
Value operator&&(const Value& left, const Value& right);
Value operator||(const Value& left, const Value& right);
Value operator==(const Value& left, const Value& right);
Value operator!=(const Value& left, const Value& right);
Value operator<(const Value& left, const Value& right);
Value operator>(const Value& left, const Value& right);
Value operator<=(const Value& left, const Value& right);
Value operator>=(const Value& left, const Value& right);
Value operator-(const Value& value);
Value operator!(const Value& value);

/* ------------------------------- Helper maps ------------------------------ */

static const auto opToAllowedTypesMap = std::unordered_map<::Operator, std::vector<TypeIdentifier>>{
    {::Operator::Add, {Int::typeId, Float::typeId, String::typeId}},
    {::Operator::Sub, {Int::typeId, Float::typeId}},
    {::Operator::Mul, {Int::typeId, Float::typeId}},
    {::Operator::Div, {Int::typeId, Float::typeId}},
    {::Operator::Mod, {Int::typeId}},
    {::Operator::And, {Bool::typeId}},
    {::Operator::Or, {Bool::typeId}},
    {::Operator::Not, {Bool::typeId}},
    {::Operator::Eq, {Int::typeId, Float::typeId, Bool::typeId, Char::typeId, String::typeId}},
    {::Operator::Neq, {Int::typeId, Float::typeId, Bool::typeId, Char::typeId, String::typeId}},
    {::Operator::Lt, {Int::typeId, Float::typeId}},
    {::Operator::Gt, {Int::typeId, Float::typeId}},
    {::Operator::Leq, {Int::typeId, Float::typeId}},
    {::Operator::Geq, {Int::typeId, Float::typeId}},
};

using BinaryApplier = std::function<Value(const Value& left, const Value& right)>;

const static std::unordered_map<::Operator, BinaryApplier> binaryOperatorAppliers = {
    {::Operator::Add, [](const Value& left, const Value& right) { return left + right; }},
    {::Operator::Sub, [](const Value& left, const Value& right) { return left - right; }},
    {::Operator::Mul, [](const Value& left, const Value& right) { return left * right; }},
    {::Operator::Div, [](const Value& left, const Value& right) { return left / right; }},
    {::Operator::Mod, [](const Value& left, const Value& right) { return left % right; }},
    {::Operator::And, [](const Value& left, const Value& right) { return left && right; }},
    {::Operator::Or, [](const Value& left, const Value& right) { return left || right; }},
    {::Operator::Eq, [](const Value& left, const Value& right) { return left == right; }},
    {::Operator::Neq, [](const Value& left, const Value& right) { return left != right; }},
    {::Operator::Lt, [](const Value& left, const Value& right) { return left < right; }},
    {::Operator::Gt, [](const Value& left, const Value& right) { return left > right; }},
    {::Operator::Leq, [](const Value& left, const Value& right) { return left <= right; }},
    {::Operator::Geq, [](const Value& left, const Value& right) { return left >= right; }},
};

using UnaryApplier = std::function<Value(const Value& value)>;

const static std::unordered_map<::Operator, UnaryApplier> unaryOperatorAppliers = {
    {::Operator::Sub, [](const Value& value) { return -value; }},
    {::Operator::Not, [](const Value& value) { return !value; }},
};

/* ------------------------ OperatorsHandler methods ------------------------ */

bool OperatorsHandler::operandsMatch(const TypePtr& left, const TypePtr& right) const noexcept {
  return left->typeId() == right->typeId();
}

bool OperatorsHandler::operatorIsSupported(const TypePtr& type,
                                           const ::Operator op) const noexcept {
  auto types = opToAllowedTypesMap.at(op);
  return std::find(types.begin(), types.end(), type->typeId()) != types.end();
}

TypeIdentifier OperatorsHandler::resultTypeId(const TypePtr& operandsType,
                                              const ::Operator op) const noexcept {
  // Operators that return bool
  static const std::vector<::Operator> LOGIC_OPERATORS = {
      ::Operator::And, ::Operator::Or, ::Operator::Not, ::Operator::Eq,  ::Operator::Neq,
      ::Operator::Lt,  ::Operator::Gt, ::Operator::Leq, ::Operator::Geq,
  };

  if (std::find(LOGIC_OPERATORS.begin(), LOGIC_OPERATORS.end(), op) != LOGIC_OPERATORS.end()) {
    return Bool::typeId;
  }

  return operandsType->typeId();
}

Value OperatorsHandler::applyOperator(const Value& left, const Value& right,
                                      const ::Operator op) const noexcept {
  assert(left.value.index() == right.value.index() &&
         "Operands must match, should be done by SemanticAnalyzer");
  assert(binaryOperatorAppliers.find(op) != binaryOperatorAppliers.end() &&
         "Invalid binary operator");

  return binaryOperatorAppliers.at(op)(left, right);
}

Value OperatorsHandler::applyOperator(const Value& value, const ::Operator op) const noexcept {
  assert(unaryOperatorAppliers.find(op) != unaryOperatorAppliers.end() && "Invalid unary operator");

  return unaryOperatorAppliers.at(op)(value);
}

/* ---------------------------- Binary operators ---------------------------- */

Value operator+(const Value& left, const Value& right) {
  return std::visit(
      overloaded{
          [](int left, int right) -> Value { return Value{left + right}; },
          [](float left, float right) -> Value { return Value{left + right}; },
          [](std::wstring& left, std::wstring& right) -> Value { return Value{left + right}; },
          [](auto&&, auto &&) -> Value { throw std::logic_error("Bad maps config"); },
      },
      left.value, right.value);
}

Value operator-(const Value& left, const Value& right) {
  return std::visit(overloaded{
                        [](int left, int right) -> Value { return Value{left - right}; },
                        [](float left, float right) -> Value { return Value{left - right}; },
                        [](auto&&, auto &&) -> Value { throw std::logic_error("Bad maps config"); },
                    },
                    left.value, right.value);
}

Value operator*(const Value& left, const Value& right) {
  return std::visit(overloaded{
                        [](int left, int right) -> Value { return Value{left * right}; },
                        [](float left, float right) -> Value { return Value{left * right}; },
                        [](auto&&, auto &&) -> Value { throw std::logic_error("Bad maps config"); },
                    },
                    left.value, right.value);
}

Value operator/(const Value& left, const Value& right) {
  return std::visit(overloaded{
                        [](int left, int right) -> Value { return Value{left / right}; },
                        [](float left, float right) -> Value { return Value{left / right}; },
                        [](auto&&, auto &&) -> Value { throw std::logic_error("Bad maps config"); },
                    },
                    left.value, right.value);
}

Value operator%(const Value& left, const Value& right) {
  return std::visit(overloaded{
                        [](int left, int right) -> Value { return Value{left % right}; },
                        [](auto&&, auto &&) -> Value { throw std::logic_error("Bad maps config"); },
                    },
                    left.value, right.value);
}

Value operator&&(const Value& left, const Value& right) {
  return std::visit(overloaded{
                        [](bool left, bool right) -> Value { return Value{left && right}; },
                        [](auto&&, auto &&) -> Value { throw std::logic_error("Bad maps config"); },
                    },
                    left.value, right.value);
}

Value operator||(const Value& left, const Value& right) {
  return std::visit(overloaded{
                        [](bool left, bool right) -> Value { return Value{left || right}; },
                        [](auto&&, auto &&) -> Value { throw std::logic_error("Bad maps config"); },
                    },
                    left.value, right.value);
}

Value operator==(const Value& left, const Value& right) {
  return std::visit(
      overloaded{
          [](int left, int right) -> Value { return Value{left == right}; },
          [](float left, float right) -> Value { return Value{left == right}; },
          [](bool left, bool right) -> Value { return Value{left == right}; },
          [](wchar_t left, wchar_t right) -> Value { return Value{left == right}; },
          [](std::wstring& left, std::wstring& right) { return Value{left == right}; },
          [](auto&&, auto &&) -> Value { throw std::logic_error("Bad maps config"); },
      },
      left.value, right.value);
}

Value operator!=(const Value& left, const Value& right) {
  return std::visit(
      overloaded{
          [](int left, int right) -> Value { return Value{left != right}; },
          [](float left, float right) -> Value { return Value{left != right}; },
          [](bool left, bool right) -> Value { return Value{left != right}; },
          [](wchar_t left, wchar_t right) -> Value { return Value{left != right}; },
          [](std::wstring& left, std::wstring& right) { return Value{left != right}; },
          [](auto&&, auto &&) -> Value { throw std::logic_error("Bad maps config"); },
      },
      left.value, right.value);
}

Value operator<(const Value& left, const Value& right) {
  return std::visit(overloaded{
                        [](int left, int right) -> Value { return Value{left < right}; },
                        [](float left, float right) -> Value { return Value{left < right}; },
                        [](auto&&, auto &&) -> Value { throw std::logic_error("Bad maps config"); },
                    },
                    left.value, right.value);
}

Value operator>(const Value& left, const Value& right) {
  return std::visit(overloaded{
                        [](int left, int right) -> Value { return Value{left > right}; },
                        [](float left, float right) -> Value { return Value{left > right}; },
                        [](auto&&, auto &&) -> Value { throw std::logic_error("Bad maps config"); },
                    },
                    left.value, right.value);
}

Value operator<=(const Value& left, const Value& right) {
  return std::visit(overloaded{
                        [](int left, int right) -> Value { return Value{left <= right}; },
                        [](float left, float right) -> Value { return Value{left <= right}; },
                        [](auto&&, auto &&) -> Value { throw std::logic_error("Bad maps config"); },
                    },
                    left.value, right.value);
}

Value operator>=(const Value& left, const Value& right) {
  return std::visit(overloaded{
                        [](int left, int right) -> Value { return Value{left >= right}; },
                        [](float left, float right) -> Value { return Value{left >= right}; },
                        [](auto&&, auto &&) -> Value { throw std::logic_error("Bad maps config"); },
                    },
                    left.value, right.value);
}

/* ----------------------------- Unary Operators ---------------------------- */

Value operator-(const Value& value) {
  return std::visit(overloaded{
                        [](int value) -> Value { return Value{-value}; },
                        [](float value) -> Value { return Value{-value}; },
                        [](auto &&) -> Value { throw std::logic_error("Bad maps config"); },
                    },
                    value.value);
}

Value operator!(const Value& value) {
  return std::visit(overloaded{
                        [](bool value) -> Value { return Value{!value}; },
                        [](auto &&) -> Value { throw std::logic_error("Bad maps config"); },
                    },
                    value.value);
}

}  // namespace Interpreter
