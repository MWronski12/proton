#include <cassert>
#include <functional>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>

#include "CastHandler.h"
#include "interpreter_utils.h"

namespace Interpreter {

bool CastHandler::isPrimitiveType(const TypeIdentifier& type) const {
  static const std::vector<TypeIdentifier> primitiveTypes = {
      Int::typeId, Float::typeId, Bool::typeId, Char::typeId, String::typeId,
  };
  return std::find(primitiveTypes.begin(), primitiveTypes.end(), type) != primitiveTypes.end();
}

TypeIdentifier CastHandler::primitiveTypeToTypeId(const ::PrimitiveType type) const {
  const static std::unordered_map<::PrimitiveType, TypeIdentifier> primitiveTypeToTypeId = {
      {::PrimitiveType::Int, Int::typeId},       {::PrimitiveType::Float, Float::typeId},
      {::PrimitiveType::Bool, Bool::typeId},     {::PrimitiveType::Char, Char::typeId},
      {::PrimitiveType::String, String::typeId},
  };

  return primitiveTypeToTypeId.at(type);
}

bool CastHandler::isCastable(const TypePtr& from, const TypeIdentifier& to) const {
  assert(from != nullptr && "TypePtr should not be nullptr");

  if (!isPrimitiveType(to)) return false;

  static const std::unordered_map<TypeIdentifier, std::unordered_set<TypeIdentifier>>
      typeCastTable = {
          {Int::typeId, {Int::typeId, Float::typeId, Bool::typeId, Char::typeId, String::typeId}},
          {Float::typeId, {Int::typeId, Float::typeId, Bool::typeId, String::typeId}},
          {Char::typeId, {Int::typeId, Float::typeId, Bool::typeId, Char::typeId, String::typeId}},
          {Bool::typeId, {}},
          {String::typeId, {}},
          {Struct::typeId, {}},
          {Variant::typeId, {}},
          {FnSignature::typeId, {}},
          {Void::typeId, {}},
      };

  return typeCastTable.at(from->typeId()).find(to) != typeCastTable.at(from->typeId()).end();
}

const static std::unordered_map<TypeIdentifier, std::function<Value(int)>> intCasters = {
    {Int::typeId, [](int integer) { return Value(int(integer)); }},
    {Float::typeId, [](int integer) { return Value(float(integer)); }},
    {Bool::typeId, [](int integer) { return Value(bool(integer)); }},
    {Char::typeId, [](int integer) { return Value(wchar_t(integer)); }},
    {String::typeId, [](int integer) { return std::to_wstring(integer); }},
};

const static std::unordered_map<TypeIdentifier, std::function<Value(float)>> floatCasters = {
    {Int::typeId, [](float floating) { return Value(int(floating)); }},
    {Float::typeId, [](float floating) { return Value(float(floating)); }},
    {Bool::typeId, [](float floating) { return Value(bool(floating)); }},
    {Char::typeId, [](float floating) { return Value(wchar_t(floating)); }},
    {String::typeId, [](float floating) { return std::to_wstring(floating); }},
};

const static std::unordered_map<TypeIdentifier, std::function<Value(wchar_t)>> charCasters = {
    {Int::typeId, [](wchar_t character) { return Value(int(character)); }},
    {Float::typeId, [](wchar_t character) { return Value(float(character)); }},
    {Bool::typeId, [](wchar_t character) { return Value(bool(character)); }},
    {Char::typeId, [](wchar_t character) { return Value(wchar_t(character)); }},
    {String::typeId, [](wchar_t character) { return std::to_wstring(character); }},
};

Value CastHandler::applyCast(const Value& value, const ::PrimitiveType to) const {
  auto typeId = primitiveTypeToTypeId(to);
  return std::visit(
      overloaded{
          [](int integer) -> Value { return intCasters.at(Int::typeId)(integer); },
          [](float floating) -> Value { return floatCasters.at(Float::typeId)(floating); },
          [](wchar_t character) -> Value { return charCasters.at(Char::typeId)(character); },
          [](auto &&) -> Value { throw std::logic_error("Invalid cast operation"); },
      },
      value.value);
}

}  // namespace Interpreter
