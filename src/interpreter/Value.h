#pragma once

#include <functional>
#include <map>
#include <memory>
#include <variant>

#include "Type.h"

namespace Interpreter {

const Identifier MAIN = L"main";

struct Value;
struct FunctionBody;

using ValuePtr = std::shared_ptr<Value>;

/**
 * @brief Data structure representing Variant instance value.
 */
struct VariantValue {
  TypeIdentifier currentType;
  ValuePtr value;
};

/**
 * @brief Data structure representing Struct instance value.
 */
struct ObjectValue {
  std::map<Identifier, ValuePtr> members;
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
    TypeIdentifier type;
    std::set<Modifier> modifiers;
  };

  std::vector<Param> params;
  TypeIdentifier returnType;
  std::shared_ptr<FunctionBody> body;
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

  Value clone() const noexcept {
    return std::visit(
        [](auto&& arg) -> Value {
          using T = std::decay_t<decltype(arg)>;
          // To clone the variant we need to clone its embedded value
          if constexpr (std::is_same_v<T, std::shared_ptr<VariantValue>>) {
            return Value(std::make_shared<VariantValue>(
                VariantValue{arg->currentType, arg->value->clone()}));
          }
          // To clone the object we need to clone its members values
          else if constexpr (std::is_same_v<T, std::shared_ptr<ObjectValue>>) {
            ObjectValue obj;
            for (const auto& [name, value] : arg->members) {
              obj.members.emplace(name, value->clone());
            }
            return Value(std::make_shared<ObjectValue>(std::move(obj)));
          }
          // Rest can be trivially copied, because they are not pointers
          else {
            return Value{arg};
          }
        },
        value);
  }
};

}  // namespace Interpreter
