// 1. Every ebnf definition is one parsing Function
// 2. Need to report appropriate errors
// 3. Make sure objects representing lang constructs can only exist in a valid
// state.
//    Dont create empty objects and assign values to its members.
// 4. Expr bazowy, inne funkcje korzystaja tylko z niego.
//    Pochodne az do Literal sa wykorzystywane tylko wewnetrznie.
// 5. Keep position of the construct

#pragma once

#include <functional>
#include <memory>

#include "Definition.h"
#include "ErrorHandlerBase.h"
#include "Lexer.h"
#include "Program.h"

using Result = std::pair<std::unique_ptr<ASTNode>, std::optional<ErrorType>>;

class Parser {
 public:
  Parser(Lexer& lexer, ErrorHandlerBase& errorHandler);

  void consumeToken();
  void skipSyntaxError();

  template <typename... FactoryMethods>
  Result expectOneOf(FactoryMethods&&... args) {
    std::function<Result(Parser&)> arr[] = {std::forward<FactoryMethods>(args)...};

    for (auto&& creator : arr) {
      auto [obj, error] = creator(*this);
      if (error != std::nullopt) {
        return {std::move(obj), std::nullopt};
      }
    }
    return {nullptr, ErrorType::EXPECTED_LBRACE};
  }

  template <typename... FactoryMethods>
  std::optional<std::vector<Result>> expectAllOf(FactoryMethods&&... args) {
    std::function<Result(Parser&)> arr[] = {std::forward<FactoryMethods>(args)...};

    std::vector<Result> results;
    for (auto&& creator : arr) {
      auto [obj, error] = creator(*this);
      if (error != std::nullopt) {
        return std::nullopt;
      }
      results.push_back({std::move(obj), std::nullopt});
    }
    return results;
  }

  Result parseDefinition() {
    return {std::make_unique<VarDef>(true, L"foo", L"string", nullptr), std::nullopt};
  }

  Result parseNullNode() { return {nullptr, ErrorType::EXPECTED_LBRACE}; };

  std::pair<std::unique_ptr<ASTNode>, std::optional<ErrorType>> parseStatement() {
    return expectOneOf(&Parser::parseDefinition);
  }

 private:
  Lexer& m_lexer;
  ErrorHandlerBase& m_errorHandler;

  Token m_token;
};
