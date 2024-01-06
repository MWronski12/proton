#pragma once

#include "Type.h"
#include "Value.h"

namespace Interpreter {

template <class... Ts>
struct overloaded : Ts... {
  using Ts::operator()...;
};

template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

/**
 * @brief Checks if both TypeRef refer to the same Type object.
 */
bool operator==(const TypeRef& lhs, const TypeRef& rhs);

/**
 * @brief Checks if underlying type is the same.
 */
bool operator==(const Type& lhs, const Type& rhs);

/*
 * @brief Checks if both Structs are exactly compatibile.
 */
bool operator==(const Struct& lhs, const Struct& rhs);

/**
 * @brief Checks if trying to assign value of Type 'from' to value of Type 'to' semantically makes
 * sense.
 *
 * Useful for semantic analysis before any evaluations happen and working with variant types, where
 * assigning an int value to a variant Number { int, float } is valid.
 */
bool isAssignable(const Type& from, const Type& to);

std::wostream& operator<<(std::wostream& os, const VariantValue& value);
std::wostream& operator<<(std::wostream& os, const ObjectValue& value);
std::wostream& operator<<(std::wostream& os, const Function& value);
std::wostream& operator<<(std::wostream& os, const Value& value);

std::wostream& operator<<(std::wostream& os, const Variant& type);
std::wostream& operator<<(std::wostream& os, const Struct& type);
std::wostream& operator<<(std::wostream& os, const FnSignature& type);
std::wostream& operator<<(std::wostream& os, const Type& type);

bool valueTypeMatch(const Value& value, const Type& type);

bool valueVariantTypeMatch(const Value& value, const Variant& variantType);
bool valueStructTypeMatch(const Value& value, const Struct& structType);
bool valueFunctionTypeMatch(const Value& value, const FnSignature& signature);

}  // namespace Interpreter
