#pragma once

#include <map>
#include <memory>
#include <set>
#include <string>
#include <variant>
#include <vector>

struct BlockStmt;

namespace Interpreter {

struct Type;

using Identifier = std::wstring;
using TypeIdentifier = std::wstring;

const TypeIdentifier VOID = std::wstring(L"void");
const TypeIdentifier INT = std::wstring(L"int");
const TypeIdentifier FLOAT = std::wstring(L"float");
const TypeIdentifier BOOL = std::wstring(L"bool");
const TypeIdentifier CHAR = std::wstring(L"char");
const TypeIdentifier STRING = std::wstring(L"string");

const TypeIdentifier VARIANT = std::wstring(L"variant");
const TypeIdentifier STRUCT = std::wstring(L"struct");
const TypeIdentifier FUNCTION = std::wstring(L"function");

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
struct Void {};

/**
 * @brief Data structure representing builtin int type.
 */
struct Int {};

/**
 * @brief Data structure representing builtin float type.
 */
struct Float {};

/**
 * @brief Data structure representing builtin bool type.
 */
struct Bool {};

/**
 * @brief Data structure representing builtin char type.
 */
struct Char {};

/**
 * @brief Data structure representing builtin string type.
 */
struct String {};

/* ------------------------------ User types -------------------------------- */

/**
 * @brief Data structure representing variant types.
 */
struct Variant {
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
  std::map<Identifier, TypeRef> members;
};

/**
 * @brief Data structure representing function types.
 */
struct Function {
  using Body = std::reference_wrapper<::BlockStmt>;
  struct Param {
    Identifier name;
    TypeRef type;
    std::set<Modifier> modifiers;
  };

  std::vector<Param> params;
  TypeRef returnType;
  std::optional<Body> body;
};

/* --------------------------- Type representation -------------------------- */

/**
 * @brief Data structure representing proton language type.
 */
struct Type {
  template <typename T>
  Type(T&& type, std::set<Modifier>&& modifiers = {})
      : type{std::move(type)}, modifiers{std::move(modifiers)} {
    static_assert(
        std::disjunction_v<std::is_same<T, Void>, std::is_same<T, Int>, std::is_same<T, Float>,
                           std::is_same<T, Bool>, std::is_same<T, Char>, std::is_same<T, String>,
                           std::is_same<T, Variant>, std::is_same<T, Struct>,
                           std::is_same<T, Function>>);
  }

  const std::variant<Void, Int, Float, Bool, Char, String, Variant, Struct, Function> type;
  const std::set<Modifier> modifiers;
};

}  // namespace Interpreter
