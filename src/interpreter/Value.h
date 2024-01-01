#pragma once

#include <map>
#include <memory>
#include <variant>

namespace {
struct BlockStmt;
}

namespace Interpreter {

struct Value;

/**
 * @brief Data structure representing Struct instance value.
 */
struct Object {
  std::map<Identifier, Value> members;
};

/**
 * @brief Data structure representing proton language Value.
 */
struct Value {
  std::variant<int, float, bool, wchar_t, std::wstring, Object> value;
};

}  // namespace Interpreter
