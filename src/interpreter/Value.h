#pragma once

#include <map>
#include <memory>
#include <variant>

namespace Interpreter {

struct Value;

/**
 * @brief Data structure representing Variant instance value.
 *
 * @note Member 'value' is a unique_ptr but it's copyable like a normal object. That is for allowing
 * default construction of a forward declared struct 'Value'.
 */
struct VariantValue {
  VariantValue(const VariantValue& other) : value{std::make_unique<Value>(*other.value)} {}
  VariantValue& operator=(const VariantValue& other) {
    if (this != &other) value = std::make_unique<Value>(*other.value);
    return *this;
  }
  ~VariantValue() = default;

  VariantValue(const TypeIdentifier& type, std::unique_ptr<Value> value)
      : currentType{type}, value{std::move(value)} {}

  TypeIdentifier currentType;
  std::unique_ptr<Value> value;
};

/**
 * @brief Data structure representing Struct instance value.
 */
struct Object {
  std::map<Identifier, Value> members;
};

/**
 * @brief Data structure representing function value, which is a pointer to a function type.
 */
struct FuncPtr {
  TypeIdentifier name;
};

/* -------------------------- Value representation -------------------------- */

/**
 * @brief Data structure representing proton language Value.
 */
struct Value {
  template <typename T>
  Value(const T& value) : value{value} {
    static_assert(
        std::disjunction_v<std::is_same<T, std::monostate>, std::is_same<T, int>,
                           std::is_same<T, float>, std::is_same<T, bool>, std::is_same<T, wchar_t>,
                           std::is_same<T, std::wstring>, std::is_same<T, VariantValue>,
                           std::is_same<T, Object>, std::is_same<T, FuncPtr>>);
  }

  std::variant<std::monostate, int, float, bool, wchar_t, std::wstring, VariantValue, Object,
               FuncPtr>
      value;
};

}  // namespace Interpreter
