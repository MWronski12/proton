#include <cassert>
#include <unordered_map>
#include <unordered_set>

#include "TypeCastHandler.h"
#include "interpreter_utils.h"

namespace Interpreter {

bool TypeCastHandler::isPrimitiveType(const TypeIdentifier& type) const noexcept {
  static const std::vector<TypeIdentifier> primitiveTypes = {
      Int::typeId, Float::typeId, Bool::typeId, Char::typeId, String::typeId,
  };
  return std::find(primitiveTypes.begin(), primitiveTypes.end(), type) != primitiveTypes.end();
}

TypeIdentifier TypeCastHandler::primitiveTypeToTypeId(const ::PrimitiveType type) const noexcept {
  const static std::unordered_map<::PrimitiveType, TypeIdentifier> primitiveTypeToTypeId = {
      {::PrimitiveType::Int, Int::typeId},       {::PrimitiveType::Float, Float::typeId},
      {::PrimitiveType::Bool, Bool::typeId},     {::PrimitiveType::Char, Char::typeId},
      {::PrimitiveType::String, String::typeId},
  };

  return primitiveTypeToTypeId.at(type);
}

bool TypeCastHandler::isCastable(const TypePtr& from, const TypeIdentifier& to) const noexcept {
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

}  // namespace Interpreter