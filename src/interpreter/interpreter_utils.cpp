#include <iostream>

#include "interpreter_utils.h"

namespace {

using namespace Interpreter;

bool valueVariantTypeMatch(const Value& value, const Variant& variantType) {
  // 1) Value must match at least one of the variant types
  for (const auto& type : variantType.types) {
    if (valueTypeMatch(value, type.get())) return true;
  }
  return false;
}

bool valueStructTypeMatch(const Value& value, const Struct& structType) {
  // 1) Value must be an object to match a struct type
  if (!std::holds_alternative<Object>(value.value)) return false;
  const auto& object = std::get<Object>(value.value);

  // 2) Object must have the same number of members as the struct type
  if (object.members.size() != structType.members.size()) return false;

  // 3) Each object member must have a corresponding struct member
  for (const auto& [name, value] : object.members) {
    if (!structType.members.contains(name)) return false;
  }

  // 4) Each object member value must match the corresponding struct member type
  for (const auto& [name, type] : structType.members) {
    if (!valueTypeMatch(object.members.at(name), type.get())) return false;
  }

  return true;
}

// bool valueFunctionTypeMatch(const Value& value, const Function& functionType) { return false; }

}  // namespace

namespace Interpreter {

bool valueTypeMatch(const Value& value, const Type& type) {
  return std::visit(
      overloaded{
          [&value](const Void&) { return std::holds_alternative<std::monostate>(value.value); },
          [&value](const Int&) { return std::holds_alternative<int>(value.value); },
          [&value](const Float&) { return std::holds_alternative<float>(value.value); },
          [&value](const Bool&) { return std::holds_alternative<bool>(value.value); },
          [&value](const Char&) { return std::holds_alternative<wchar_t>(value.value); },
          [&value](const String&) { return std::holds_alternative<std::wstring>(value.value); },
          [&value](const Variant& type) { return valueVariantTypeMatch(value, type); },
          [&value](const Struct& type) { return valueStructTypeMatch(value, type); },
          // [&value](const Function& type) { return valueFunctionTypeMatch(value, type); },
          [](const auto&) { return false; },
      },
      type.type);
}

std::wostream& operator<<(std::wostream& os, const VariantValue& value) {
  return os << L"Variant(" << *value.value << L")";
}

std::wostream& operator<<(std::wostream& os, const Object& value) {
  os << L"Object({ ";
  if (!value.members.empty()) {
    auto it = value.members.begin();
    os << it->first << L": " << it->second;
    for (++it; it != value.members.end(); ++it) {
      os << L", " << it->first << L": " << it->second;
    }
  }
  os << L" })";
  return os;
}

std::wostream& operator<<(std::wostream& os, const Value& value) {
  std::visit(overloaded{
                 [&os](const std::monostate&) { os << L"Void(null)"; },
                 [&os](const int& value) { os << L"Int(" << value << ")"; },
                 [&os](const float& value) { os << L"Float(" << value << ")"; },
                 [&os](const bool& value) { os << L"Bool(" << std::boolalpha << value << ")"; },
                 [&os](const wchar_t& value) { os << L"Char('" << value << "')"; },
                 [&os](const std::wstring& value) { os << L"String(\"" << value << "\")"; },
                 [&os](const VariantValue& value) { os << value; },
                 [&os](const Object& value) { os << value; },
                 [&os](const FuncPtr&) { os << L"FuncPtr()"; },
                 [](const auto&) {},
             },
             value.value);
  return os;
}

/* -------------------------------------------------------------------------- */

std::wostream& operator<<(std::wostream& os, const Variant& type) {
  os << L"Variant{ ";
  if (!type.types.empty()) {
    auto it = type.types.begin();
    os << it->get();
    for (++it; it != type.types.end(); ++it) {
      os << L", " << it->get();
    }
  }
  os << L" }";
  return os;
}

std::wostream& operator<<(std::wostream& os, const Struct& type) {
  os << L"Struct{ ";
  if (!type.members.empty()) {
    auto it = type.members.begin();
    os << it->first << L": " << it->second.get();
    for (++it; it != type.members.end(); ++it) {
      os << L", " << it->first << L": " << it->second.get();
    }
  }
  os << L" }";
  return os;
}

std::wostream& operator<<(std::wostream& os, const Type& type) {
  std::visit(overloaded{
                 [&os](const Void&) { os << L"Void"; },
                 [&os](const Int&) { os << L"Int"; },
                 [&os](const Float&) { os << L"Float"; },
                 [&os](const Bool&) { os << L"Bool"; },
                 [&os](const Char&) { os << L"Char"; },
                 [&os](const String&) { os << L"String"; },
                 [&os](const Variant& type) { os << type; },
                 [&os](const Struct& type) { os << type; },
                 [](const auto&) {},
             },
             type.type);
  return os;
}

}  // namespace Interpreter
