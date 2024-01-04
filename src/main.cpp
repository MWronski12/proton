// #include "Interpreter.h"
// #include "StringCharReader.h"

#include <iostream>

#include "Environment.h"
#include "Statement.h"
#include "interpreter_utils.h"

using namespace Interpreter;

int main() {
  // Has builtin types Void, Int, Float, Bool, Char, String
  Environment env;

  // Define a 'Number' variant type that can hold either an Int or a Float
  auto variantTypes = {*env.getType(INT), *env.getType(FLOAT)};
  Type Number{Variant{std::move(variantTypes)}};
  env.defineType(std::wstring(L"Number"), std::move(Number));

  // Define a struct type 'Person' with members String 'foo' and Int 'bar'
  std::map<Identifier, TypeRef> structMembers{
      {std::wstring(L"foo"), *env.getType(STRING)},
      {std::wstring(L"bar"), *env.getType(INT)},
  };
  Type Person{Struct{std::move(structMembers)}};
  env.defineType(std::wstring(L"Person"), std::move(Person));

  // Define a function foo(x: int, y: float) -> string {  }
  std::vector<Function::Param> fnParams;
  fnParams.push_back({std::wstring(L"x"), *env.getType(INT), {}});
  fnParams.push_back({std::wstring(L"y"), *env.getType(FLOAT), {}});
  env.defineFn(L"foo", *env.getType(STRING), *(new ::BlockStmt(Position(), {})),
               std::move(fnParams));

  // Using valueTypeMatch
  std::wcout << "------------------------ valueTypeMatch -------------------------\n";
  std::wcout << "valueTypeMatch(Void(null), Void): "
             << valueTypeMatch(Value(std::monostate{}), env.getType(VOID).value().get())
             << std::endl;
  std::wcout << "valueTypeMatch(Int(1), Int): "
             << valueTypeMatch(Value(1), env.getType(INT).value().get()) << std::endl;
  std::wcout << "valueTypeMatch(Float(1.329), Float): "
             << valueTypeMatch(Value(1.329f), env.getType(FLOAT).value().get()) << std::endl;
  std::wcout << "valueTypeMatch(Bool(true), Bool): "
             << valueTypeMatch(Value(true), env.getType(BOOL).value().get()) << std::endl;
  std::wcout << "valueTypeMatch(Char('a'), Char): "
             << valueTypeMatch(Value(L'a'), env.getType(CHAR).value().get()) << std::endl;
  std::wcout << "valueTypeMatch(String(\"Hello, World!\"), String): "
             << valueTypeMatch(Value(std::wstring(L"Hello, World!")),
                               env.getType(STRING).value().get())
             << std::endl;
  std::wcout << "valueTypeMatch(Number(1), " << env.getType(std::wstring(L"Number")).value().get()
             << "): "
             << valueTypeMatch(Value(1), env.getType(std::wstring(L"Number")).value().get())
             << std::endl;
  std::wcout << "valueTypeMatch(Number(1.329), "
             << env.getType(std::wstring(L"Number")).value().get() << "): "
             << valueTypeMatch(Value(1.329f), env.getType(std::wstring(L"Number")).value().get())
             << std::endl;
  std::wcout << "valueTypeMatch(Person{foo: \"Hello, World!\", bar: 1}, "
             << env.getType(std::wstring(L"Person")).value().get() << "): "
             << valueTypeMatch(Value(ObjectValue{{
                                   {std::wstring(L"foo"), Value(std::wstring(L"Hello, World!"))},
                                   {std::wstring(L"bar"), Value(1)},
                               }}),
                               env.getType(std::wstring(L"Person")).value().get())
             << std::endl;
  std::wcout << "valueTypeMatch(" << env.getFnSignature(L"foo").value() << ", "
             << env.getFunction(L"foo").value() << "): "
             << valueTypeMatch(env.getFunction(L"foo").value(), env.getFnSignature(L"foo").value())
             << std::endl;
  std::wcout << "valueTypeMatch(" << FnSignature{*env.getType(STRING), {}} << ", "
             << env.getFunction(L"foo").value() << "): "
             << valueTypeMatch(env.getFunction(L"foo").value(),
                               FnSignature{*env.getType(STRING), {}})
             << std::endl;

  // Builtin example values
  Value none(std::monostate{});
  Value integer(1);
  Value floating(1.329f);
  Value boolean(true);
  Value character(L'a');
  Value string(std::wstring(L"Hello, World!"));

  // Value compatibile with Number variant type
  Value number(VariantValue{std::make_unique<Value>(1)});

  // Value compatibile with Person struct type
  Value person{ObjectValue{{
      {std::wstring(L"foo"), integer},
      {std::wstring(L"bar"), floating},
  }}};

  // Print example values and types
  std::wcout << "------------------------ Example values ------------------------\n";
  for (const Value& value : {none, integer, floating, boolean, character, string, number, person}) {
    std::wcout << value << std::endl;
  }

  std::wcout << "------------------------ Example types -------------------------\n";
  for (const Type& type : {
           env.getType(VOID).value().get(),
           env.getType(INT).value().get(),
           env.getType(FLOAT).value().get(),
           env.getType(BOOL).value().get(),
           env.getType(CHAR).value().get(),
           env.getType(STRING).value().get(),
           env.getType(L"Number").value().get(),
           env.getType(L"Person").value().get(),
       }) {
    std::wcout << type << std::endl;
  }
  std::wcout << "----------------------------------------------------------------\n";

  return 0;
}
