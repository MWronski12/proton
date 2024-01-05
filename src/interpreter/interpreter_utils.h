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

bool operator==(const TypeRef& lhs, const TypeRef& rhs);

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

FnSignature fnToFnSignature(const Function& fn);

}  // namespace Interpreter
