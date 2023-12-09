#include "Definition.h"
#include "Expression.h"
#include "Lexer.h"
#include "Parser.h"
#include "StrictErrorHandler.h"
#include "StringCharReader.h"

int main() {
  StrictErrorHandler errorHandler;
  StringCharReader reader{L"2 + 2 * 3;"};
  Lexer lexer{reader, errorHandler};
  Parser parser{lexer, errorHandler};

  // std::optional<Program> program;

  // ProgramDeclarations declarations;
  // auto decl = std::make_unique<VarDecl>(L"foo", L"bar");
  // declarations[decl->name] = std::move(decl);

  // ProgramDefinitions definitions;
  // StructMembers members;
  // auto baz = std::make_unique<VarDecl>(L"baz", L"string");
  // members.push_back(std::move(baz));
  // auto Baz = std::make_unique<StructDef>(L"Baz", std::move(members));
  // definitions[Baz->name] = std::move(Baz);

  // program = Program{std::move(declarations), std::move(definitions)};

  return 0;
}
