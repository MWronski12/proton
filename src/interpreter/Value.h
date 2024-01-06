#pragma once

#include <functional>
#include <map>
#include <memory>
#include <variant>

#include "Type.h"

struct Value;

namespace Interpreter {

struct Value;
struct FunctionBody;

/**
 * @brief Data structure representing Variant instance value.
 *
 * @note Member 'value' is a unique_ptr but it's copyable like a normal object. That is for
 * allowing default construction of a forward declared struct 'Value'.
 */
struct VariantValue {
  VariantValue(const VariantValue& other) : value{std::make_unique<Value>(*other.value)} {}
  VariantValue& operator=(const VariantValue& other) {
    if (this != &other) value = std::make_unique<Value>(*other.value);
    return *this;
  }
  ~VariantValue() = default;

  std::unique_ptr<Value> value;
};

/**
 * @brief Data structure representing Struct instance value.
 */
struct ObjectValue {
  std::map<Identifier, Value> members;
};

/**
 * @brief Data structure representing function value.
 */
struct Function {
  struct Param {
    Param(const Identifier& name, const TypeIdentifier& type, bool isConst)
        : name{name}, type{type} {
      if (isConst) modifiers.emplace(Modifier::CONST);
    }
    Identifier name;
    TypeIdentifier type;  // for prettyprinting only
    std::set<Modifier> modifiers;
  };

  std::vector<Param> params;
  TypeIdentifier returnType;           // for prettyprinting only
  std::shared_ptr<FunctionBody> body;  // visitable
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
                           std::is_same<T, ObjectValue>, std::is_same<T, Function>>);
  }

  std::variant<std::monostate, int, float, bool, wchar_t, std::wstring, VariantValue, ObjectValue,
               Function>
      value;
};

}  // namespace Interpreter
