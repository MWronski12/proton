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

std::wostream& operator<<(std::wostream& os, const VariantValue& value);
std::wostream& operator<<(std::wostream& os, const ObjectValue& value);
std::wostream& operator<<(std::wostream& os, const Function& value);
std::wostream& operator<<(std::wostream& os, const Value& value);

std::wostream& operator<<(std::wostream& os, const Variant& type);
std::wostream& operator<<(std::wostream& os, const Struct& type);
std::wostream& operator<<(std::wostream& os, const FnSignature& type);
std::wostream& operator<<(std::wostream& os, const Type& type);

bool valueTypeMatch(const Value& value, const Type& type);

}  // namespace Interpreter
