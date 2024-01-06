#pragma once

#include <map>
#include <memory>
#include <set>
#include <string>
#include <variant>
#include <vector>

namespace Interpreter {

struct Type;

using Identifier = std::wstring;
using TypeIdentifier = std::wstring;

const TypeIdentifier LEN_FN = L"len";

// Composite types hold references to other types for recursive type checking.
// All referenced subtypes must outlive the composite type.
using TypeRef = std::reference_wrapper<const Type>;

struct TypeRefComparator {
  bool operator()(const TypeRef& lhs, const TypeRef& rhs) const { return &lhs.get() < &rhs.get(); }
};

enum class Modifier { CONST };

/* ------------------------------ Builtin types ----------------------------- */

/**
 * @brief Data structure representing builtin void type.
 */
struct Void {
  static const inline TypeIdentifier typeId = L"void";
};

/**
 * @brief Data structure representing builtin int type.
 */
struct Int {
  static const inline TypeIdentifier typeId = L"int";
};

/**
 * @brief Data structure representing builtin float type.
 */
struct Float {
  static const inline TypeIdentifier typeId = L"float";
};

/**
 * @brief Data structure representing builtin bool type.
 */
struct Bool {
  static const inline TypeIdentifier typeId = L"bool";
};

/**
 * @brief Data structure representing builtin char type.
 */
struct Char {
  static const inline TypeIdentifier typeId = L"char";
};

/**
 * @brief Data structure representing builtin string type.
 */
struct String {
  static const inline TypeIdentifier typeId = L"string";
};

/* ------------------------------ User types -------------------------------- */

/**
 * @brief Data structure representing variant types.
 */
struct Variant {
  static const inline TypeIdentifier typeId = L"variant";

  template <typename... Ts>
  Variant(Ts&&... variantTypes) {
    std::set<TypeRef, TypeRefComparator> typesSet{std::forward<Ts>(variantTypes)...};
    types = std::move(typesSet);
  }

  std::set<TypeRef, TypeRefComparator> types;
};

/**
 * @brief Data structure representing struct types.
 */
struct Struct {
  static const inline TypeIdentifier typeId = L"struct";

  std::map<Identifier, TypeRef> members;
};

/**
 * @brief Data structure representing function types.
 */
struct FnSignature {
  static const inline TypeIdentifier typeId = L"function";

  struct Param {
    Param(const TypeRef& type, bool isConst) : type{type} {
      if (isConst) modifiers.emplace(Modifier::CONST);
    }
    TypeRef type;
    std::set<Modifier> modifiers;
  };

  std::vector<Param> params;
  TypeRef returnType;
};

/* --------------------------- Type representation -------------------------- */

/**
 * @brief Data structure representing proton language type.
 */
struct Type {
  template <typename T>
  Type(T&& type) : type{std::move(type)} {
    static_assert(
        std::disjunction_v<std::is_same<T, Void>, std::is_same<T, Int>, std::is_same<T, Float>,
                           std::is_same<T, Bool>, std::is_same<T, Char>, std::is_same<T, String>,
                           std::is_same<T, Variant>, std::is_same<T, Struct>,
                           std::is_same<T, FnSignature>>);
  }

  TypeIdentifier typeId() const {
    return std::visit([](const auto& type) { return type.typeId; }, type);
  }

  const std::variant<Void, Int, Float, Bool, Char, String, Variant, Struct, FnSignature> type;
};

}  // namespace Interpreter
