#include <iostream>

#include "interpreter_utils.h"

namespace Interpreter {

bool operator==(const TypeRef& lhs, const TypeRef& rhs) { return &lhs.get() == &rhs.get(); }

bool valueVariantTypeMatch(const Value& value, const Variant& variantType) {
  // 1) Value must match at least one of the variant types
  for (const auto& type : variantType.types) {
    if (valueTypeMatch(value, type.get())) return true;
  }
  return false;
}

bool valueStructTypeMatch(const Value& value, const Struct& structType) {
  // 1) Value must be an object to match a struct type
  if (!std::holds_alternative<ObjectValue>(value.value)) return false;
  const auto& object = std::get<ObjectValue>(value.value);

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

bool valueFunctionTypeMatch(const Value& value, const FnSignature& signature) {
  // 1) Value must be a function to match a function type
  if (!std::holds_alternative<Function>(value.value)) return false;
  const auto& function = std::get<Function>(value.value);

  // 2) Function must have the same number of parameters as the function type
  if (function.params.size() != signature.args.size()) return false;

  // 3) Function parameter types must match the corresponding function type parameter types
  for (uint i = 0; i < function.params.size(); ++i) {
    if (function.params[i].modifiers != signature.args[i].modifiers) return false;
    if (&function.params[i].type.get() != &signature.args[i].type.get()) return false;
  }

  // 4) Function return type must match the function type return type
  if (&function.returnType.get() != &signature.returnType.get()) return false;

  return true;
}

/* -------------------------------------------------------------------------- */

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
          [&value](const FnSignature& type) { return valueFunctionTypeMatch(value, type); },
          [](const auto&) { return false; },
      },
      type.type);
}

/* -------------------------------------------------------------------------- */

std::wostream& operator<<(std::wostream& os, const VariantValue& value) {
  return os << L"Variant(" << *value.value << L")";
}

std::wostream& operator<<(std::wostream& os, const ObjectValue& value) {
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

std::wostream& operator<<(std::wostream& os, const Function& value) {
  os << L"Function((";
  if (!value.params.empty()) {
    auto it = value.params.begin();
    os << it->name << L": " << it->type.get();
    for (++it; it != value.params.end(); ++it) {
      os << L", " << it->name << L": " << it->type.get();
    }
  }
  os << L") " << L"-> " << value.returnType.get() << L" { ... })";
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
                 [&os](const ObjectValue& value) { os << value; },
                 [&os](const Function& value) { os << value; },
                 [&os](const auto&) { os << "Unknown value!"; },
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

std::wostream& operator<<(std::wostream& os, const FnSignature& type) {
  os << L"Function{(";
  if (!type.args.empty()) {
    auto it = type.args.begin();
    os << it->type.get();
    for (++it; it != type.args.end(); ++it) {
      os << L", " << it->type.get();
    }
  }
  os << L") " << L"-> " << type.returnType.get() << L"}";
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
                 [&os](const FnSignature& type) { os << type; },
                 [&os](const auto&) { os << L"Unknown type!"; },
             },
             type.type);
  return os;
}

/* -------------------------------------------------------------------------- */

FnSignature fnToFnSignature(const Function& fn) {
  FnSignature signature{fn.returnType, {}};
  for (const auto& param : fn.params) {
    signature.args.push_back({param.type, param.modifiers});
  }
  return signature;
}

}  // namespace Interpreter
