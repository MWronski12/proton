#pragma once

#include <map>
#include <memory>
#include <set>
#include <string>
#include <variant>
#include <vector>

namespace {
struct BlockStmt;  // For function body reference
}

namespace Interpreter {

using Identifier = std::wstring;

struct Type;

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
  std::set<Identifier> types;
};

/**
 * @brief Data structure representing struct types.
 */
struct Struct {
  std::map<Identifier, Identifier> members;
};

/**
 * @brief Data structure representing function types.
 */
struct Function {
  struct Param {
    Identifier name;
    Identifier type;
    std::set<Modifier> modifiers;
  };

  std::vector<Param> params;
  Identifier returnType;
  std::optional<std::reference_wrapper<::BlockStmt>> body;
};

/* --------------------------- Type representation -------------------------- */

/**
 * @brief Data structure representing proton language type.
 */
struct Type {
  std::set<Modifier> modifiers;
  std::variant<Void, Int, Float, Bool, Char, String, Variant, Struct, Function> type;
};

}  // namespace Interpreter
