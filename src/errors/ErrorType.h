#pragma once

#include <string>
#include <unordered_map>

enum class ErrorType {
  // Lexical Errors
  INVALID_NUMBER_LITERAL = 0,
  INVALID_CHAR_LITERAL,
  MISSING_CLOSING_QUOTE,
  UNEXPECTED_CHARACTER,
  UNEXPECTED_END_OF_FILE,

  /* ------------------------------ Syntax Errors ----------------------------- */

  // Variable Definition
  VARDEF_EXPECTED_IDENTIFIER,
  VARDEF_EXPECTED_COLON,
  VARDEF_EXPECTED_TYPE_IDENTIFIER,
  VARDEF_EXPECTED_ASSIGNMENT,
  VARDEF_EXPECTED_EXPRESSION,
  VARDEF_EXPECTED_SEMICOLON,

  // Const Definition
  CONSTDEF_EXPECTED_IDENTIFIER,
  CONSTDEF_EXPECTED_COLON,
  CONSTDEF_EXPECTED_TYPE_IDENTIFIER,
  CONSTDEF_EXPECTED_ASSIGNMENT,
  CONSTDEF_EXPECTED_EXPRESSION,
  CONSTDEF_EXPECTED_SEMICOLON,

  // Struct Definition
  STRUCTDEF_EXPECTED_IDENTIFIER,
  STRUCTDEF_EXPECTED_LBRACE,
  STRUCTDEF_EXPECTED_RBRACE,
  STRUCTDEF_EXPECTED_SEMICOLON,
  STRUCTMEMBER_EXPECTED_COLON,
  STRUCTMEMBER_EXPECTED_TYPE_IDENTIFIER,
  STRUCTMEMBER_EXPECTED_SEMICOLON,

  // Variant Definition
  VARIANTDEF_EXPECTED_IDENTIFIER,
  VARIANTDEF_EXPECTED_LBRACE,
  VARIANTDEF_EXPECTED_RBRACE,
  VARIANTDEF_EXPECTED_SEMICOLON,

  // Fn Definition
  FNDEF_EXPECTED_IDENTIFIER,
  FNDEF_EXPECTED_LPAREN,
  FNDEF_EXPECTED_RPAREN,
  FNDEF_EXPECTED_ARROW,
  FNDEF_EXPECTED_RETURN_TYPE,
  FNDEF_EXPECTED_BLOCK,
  FNPARAM_EXPECTED_IDENTIFIER,
  FNPARAM_EXPECTED_COLON,
  FNPARAM_EXPECTED_TYPE_IDENTIFIER,
  FNRETURNTYPE_EXPECTED_TYPE_IDENTIFIER,

  // Binary Expression
  BINARYEXPRESSION_EXPECTED_RHS,

  // Unary Expression
  UNARYEXPRESSION_EXPECTED_EXPR,

  // Cast Expression
  CASTEXPR_EXPECTED_LPAREN,
  CASTEXPR_EXPECTED_EXPRESSION,
  CASTEXPR_EXPECTED_RPAREN,

  // Paren Expression
  PARENEXPR_EXPECTED_EXPRESSION,
  PARENEXPR_EXPECTED_RPAREN,

  // Object
  OBJECT_EXPECTED_RBRACE,
  OBJECTMEMBER_EXPECTED_COLON,
  OBJECTMEMBER_EXPECTED_EXPRESSION,

  // Fn Call
  FNCALL_EXPECTED_ARGUMENT,
  FNCALL_EXPECTED_RPAREN,

  // Member Access
  MEMBERACCESS_EXPECTED_IDENTIFIER,

  // Variant Access
  VARIANTACCESS_EXPECTED_TYPE_IDENTIFIER,

  // ExpressionOrAssignmentStmt
  ASSIGNMENTSTMT_EXPECTED_EXPRESSION,
  ASSIGNMENTSTMT_EXPECTED_SEMICOLON,
  EXPRESSIONSTMT_EXPECTED_SEMICOLON,

  // Insertion/Extraction
  STDOUTINSERTION_EXPECTED_EXPRESSION,
  STDOUTINSERTION_EXPECTED_SEMICOLON,
  STDINEXTRACTION_EXPECTED_EXPRESSION,
  STDINEXTRACTION_EXPECTED_SEMICOLON,

  // Variant Match
  VARIANTMATCHCASE_EXPECTED_TYPE,
  VARIANTMATCHCASE_EXPECTED_ARROW,
  VARIANTMATCHCASE_EXPECTED_BLOCK,
  VARIANTMATCH_EXPECTED_EXPRESSION,
  VARIANTMATCH_EXPECTED_LBRACE,
  VARIANTMATCH_EXPECTED_RBRACE,

  // Flow control statements
  ELIF_EXPECTED_CONDITION,
  ELIF_EXPECTED_BLOCK,
  ELSE_EXPECTED_BLOCK,
  IF_EXPECTED_CONDITION,
  IF_EXPECTED_BLOCK,

  FORRANGE_EXPECTED_UNTIL,
  FORRANGE_EXPECTED_END_EXPR,
  FOR_EXPECTED_IDENTIFIER,
  FOR_EXPECTED_IN,
  FOR_EXPECTED_RANGE,
  FOR_EXPECTED_BLOCK,
  WHILE_EXPECTED_CONDITION,
  WHILE_EXPECTED_BLOCK,
  CONTINUE_EXPECTED_SEMICOLON,
  BREAK_EXPECTED_SEMICOLON,
  RETURN_EXPECTED_SEMICOLON,

  // Semantic Errors
  EXPECTED_MAIN_FUNCTION_DEF,
  VARIABLE_REDEFINITION,
  TYPE_REDEFINITION,
  UNDEFINED_VARIABLE,
  UNDEFINED_TYPE,
  REDEFINITION,
  STRUCTMEMBER_REDEFINITION,
  VARIANTTYPE_REDEFINITION,
  FNPARAM_REDEFINITION,
  OBJECTMEMBER_REDEFINITION,
  VARIANTMATCHCASE_REDEFINITION,

  // Internal Errors
  TOKEN_INVARIANT_VIOLATION,
};

const auto LEXICAL_ERROR = std::string("Lexical Error");
const auto SYNTAX_ERROR = std::string("Syntax Error");
const auto SEMANTIC_ERROR = std::string("Semantic Error");
const auto INTERNAL_ERROR = std::string("Internal Error");

using ErrorInfo = std::pair<std::string, std::string>;  // pairs of <ErrorTypeStr, ErrorMsg>

static const std::unordered_map<ErrorType, ErrorInfo> Errors = {

    /* ----------------------------- Internal Errors ---------------------------- */

    {ErrorType::TOKEN_INVARIANT_VIOLATION, {INTERNAL_ERROR, "Token data is not integral!"}},

    /* ----------------------------- Lexical Errors ----------------------------- */

    {ErrorType::INVALID_NUMBER_LITERAL, {LEXICAL_ERROR, "Invalid number literal!"}},
    {ErrorType::INVALID_CHAR_LITERAL,
     {LEXICAL_ERROR,
      "Invalid char literal! Char literals must contain exactly one character "
      "enclosed in single quotes!"}},
    {ErrorType::MISSING_CLOSING_QUOTE, {LEXICAL_ERROR, "Missing closing quote!"}},
    {ErrorType::UNEXPECTED_CHARACTER, {LEXICAL_ERROR, "Unexpected character encountered!"}},
    {ErrorType::UNEXPECTED_END_OF_FILE, {LEXICAL_ERROR, "Unexpected end of file reached!"}},

    /* ------------------------------ Syntax Errors ----------------------------- */

    // Variable Definition
    {ErrorType::VARDEF_EXPECTED_IDENTIFIER,
     {SYNTAX_ERROR, "Expected identifier in variable definition!"}},
    {ErrorType::VARDEF_EXPECTED_COLON, {SYNTAX_ERROR, "Expected colon in variable definition!"}},
    {ErrorType::VARDEF_EXPECTED_TYPE_IDENTIFIER,
     {SYNTAX_ERROR, "Expected type identifier in variable definition!"}},
    {ErrorType::VARDEF_EXPECTED_ASSIGNMENT,
     {SYNTAX_ERROR, "Expected assignment in variable definition!"}},
    {ErrorType::VARDEF_EXPECTED_EXPRESSION,
     {SYNTAX_ERROR, "Expected expression in variable definition!"}},
    {ErrorType::VARDEF_EXPECTED_SEMICOLON,
     {SYNTAX_ERROR, "Missing semicolon at the end of variable definition!"}},

    // Const Definition
    {ErrorType::CONSTDEF_EXPECTED_IDENTIFIER,
     {SYNTAX_ERROR, "Expected identifier in const definition!"}},
    {ErrorType::CONSTDEF_EXPECTED_COLON, {SYNTAX_ERROR, "Expected colon in const definition!"}},
    {ErrorType::CONSTDEF_EXPECTED_TYPE_IDENTIFIER,
     {SYNTAX_ERROR, "Expected type identifier in const definition!"}},
    {ErrorType::CONSTDEF_EXPECTED_ASSIGNMENT,
     {SYNTAX_ERROR, "Expected assignment in const definition!"}},
    {ErrorType::CONSTDEF_EXPECTED_EXPRESSION,
     {SYNTAX_ERROR, "Expected expression in const definition!"}},
    {ErrorType::CONSTDEF_EXPECTED_SEMICOLON,
     {SYNTAX_ERROR, "Missing semicolon at the end of const definition!"}},

    // Struct Definition
    {ErrorType::STRUCTDEF_EXPECTED_IDENTIFIER,
     {SYNTAX_ERROR, "Expected identifier in struct definition!"}},
    {ErrorType::STRUCTDEF_EXPECTED_LBRACE,
     {SYNTAX_ERROR, "Expected opening brace in struct definition!"}},
    {ErrorType::STRUCTDEF_EXPECTED_RBRACE,
     {SYNTAX_ERROR, "Expected closing brace in struct definition!"}},
    {ErrorType::STRUCTDEF_EXPECTED_SEMICOLON,
     {SYNTAX_ERROR, "Missing semicolon at the end of struct definition!"}},
    {ErrorType::STRUCTMEMBER_EXPECTED_COLON,
     {SYNTAX_ERROR, "Expected colon in struct member definition!"}},
    {ErrorType::STRUCTMEMBER_EXPECTED_TYPE_IDENTIFIER,
     {SYNTAX_ERROR, "Expected type identifier in struct member definition!"}},
    {ErrorType::STRUCTMEMBER_EXPECTED_SEMICOLON,
     {SYNTAX_ERROR, "Missing semicolon at the end of struct member definition!"}},

    // Variant Definition
    {ErrorType::VARIANTDEF_EXPECTED_IDENTIFIER,
     {SYNTAX_ERROR, "Expected identifier in variant definition!"}},
    {ErrorType::VARIANTDEF_EXPECTED_LBRACE,
     {SYNTAX_ERROR, "Expected opening brace in variant definition!"}},
    {ErrorType::VARIANTDEF_EXPECTED_RBRACE,
     {SYNTAX_ERROR, "Expected closing brace in variant definition!"}},
    {ErrorType::VARIANTDEF_EXPECTED_SEMICOLON,
     {SYNTAX_ERROR, "Missing semicolon at the end of variant definition!"}},

    // Fn Definition
    {ErrorType::FNDEF_EXPECTED_IDENTIFIER,
     {SYNTAX_ERROR, "Expected identifier in function definition!"}},
    {ErrorType::FNDEF_EXPECTED_LPAREN,
     {SYNTAX_ERROR, "Expected opening parenthesis in function definition!"}},
    {ErrorType::FNDEF_EXPECTED_RPAREN,
     {SYNTAX_ERROR, "Expected closing parenthesis in function definition!"}},
    {ErrorType::FNDEF_EXPECTED_ARROW,
     {SYNTAX_ERROR, "Expected return type specifier in function definition!"}},
    {ErrorType::FNDEF_EXPECTED_RETURN_TYPE,
     {SYNTAX_ERROR, "Expected return type specifier in function definition!"}},
    {ErrorType::FNDEF_EXPECTED_BLOCK, {SYNTAX_ERROR, "Expected block in function definition!"}},
    {ErrorType::FNPARAM_EXPECTED_IDENTIFIER,
     {SYNTAX_ERROR, "Expected identifier in function parameter definition!"}},
    {ErrorType::FNPARAM_EXPECTED_COLON,
     {SYNTAX_ERROR, "Expected colon in function parameter definition!"}},
    {ErrorType::FNPARAM_EXPECTED_TYPE_IDENTIFIER,
     {SYNTAX_ERROR, "Expected type identifier in function parameter definition!"}},
    {ErrorType::FNRETURNTYPE_EXPECTED_TYPE_IDENTIFIER,
     {SYNTAX_ERROR, "Expected type identifier in function return type definition!"}},

    // Binary Expression
    {ErrorType::BINARYEXPRESSION_EXPECTED_RHS,
     {SYNTAX_ERROR, "Expected right-hand side in binary expression!"}},

    // Unary Expression
    {ErrorType::UNARYEXPRESSION_EXPECTED_EXPR,
     {SYNTAX_ERROR, "Expected expression in unary expression!"}},

    // Cast Expression
    {ErrorType::CASTEXPR_EXPECTED_LPAREN,
     {SYNTAX_ERROR, "Missing opening parenthesis in cast expression!"}},
    {ErrorType::CASTEXPR_EXPECTED_EXPRESSION,
     {SYNTAX_ERROR, "Expected expression in cast expression!"}},
    {ErrorType::CASTEXPR_EXPECTED_RPAREN,
     {SYNTAX_ERROR, "Missing closing parenthesis in cast expression!"}},

    // Paren Expression
    {ErrorType::PARENEXPR_EXPECTED_RPAREN,
     {SYNTAX_ERROR, "Missing closing parenthesis in parenthesized expression!"}},

    // Object
    {ErrorType::OBJECT_EXPECTED_RBRACE, {SYNTAX_ERROR, "Expected closing brace in object!"}},
    {ErrorType::OBJECTMEMBER_EXPECTED_EXPRESSION,
     {SYNTAX_ERROR, "Expected expression in object member definition!"}},
    {ErrorType::OBJECTMEMBER_EXPECTED_COLON,
     {SYNTAX_ERROR, "Expected colon in object member definition!"}},

    // Fn Call
    {ErrorType::FNCALL_EXPECTED_ARGUMENT, {SYNTAX_ERROR, "Expected argument in function call!"}},
    {ErrorType::FNCALL_EXPECTED_RPAREN,
     {SYNTAX_ERROR, "Missing closing parenthesis in function call!"}},

    // Member Access
    {ErrorType::MEMBERACCESS_EXPECTED_IDENTIFIER,
     {SYNTAX_ERROR, "Expected identifier in member access!"}},

    // Variant Access
    {ErrorType::VARIANTACCESS_EXPECTED_TYPE_IDENTIFIER,
     {SYNTAX_ERROR, "Expected type identifier in variant access!"}},

    // ExpressionOrAssignmentStmt
    {ErrorType::ASSIGNMENTSTMT_EXPECTED_EXPRESSION,
     {SYNTAX_ERROR, "Expected expression in assignment statement!"}},
    {ErrorType::ASSIGNMENTSTMT_EXPECTED_SEMICOLON,
     {SYNTAX_ERROR, "Missing semicolon at the end of assignment statement!"}},
    {ErrorType::EXPRESSIONSTMT_EXPECTED_SEMICOLON,
     {SYNTAX_ERROR, "Missing semicolon at the end of expression statement!"}},

    // Insertion/Extraction
    {ErrorType::STDOUTINSERTION_EXPECTED_EXPRESSION,
     {SYNTAX_ERROR, "Expected expression in stdout insertion!"}},
    {ErrorType::STDOUTINSERTION_EXPECTED_SEMICOLON,
     {SYNTAX_ERROR, "Missing semicolon at the end of stdout insertion!"}},
    {ErrorType::STDINEXTRACTION_EXPECTED_EXPRESSION,
     {SYNTAX_ERROR, "Expected expression in stdin extraction!"}},
    {ErrorType::STDINEXTRACTION_EXPECTED_SEMICOLON,
     {SYNTAX_ERROR, "Missing semicolon at the end of stdin extraction!"}},

    // Variant Match
    {ErrorType::VARIANTMATCHCASE_EXPECTED_TYPE,
     {SYNTAX_ERROR, "Expected type identifier in variant match case!"}},
    {ErrorType::VARIANTMATCHCASE_EXPECTED_ARROW,
     {SYNTAX_ERROR, "Expected arrow in variant match case!"}},
    {ErrorType::VARIANTMATCHCASE_EXPECTED_BLOCK,
     {SYNTAX_ERROR, "Expected block in variant match case!"}},
    {ErrorType::VARIANTMATCH_EXPECTED_EXPRESSION,
     {SYNTAX_ERROR, "Expected expression in variant match!"}},
    {ErrorType::VARIANTMATCH_EXPECTED_LBRACE,
     {SYNTAX_ERROR, "Expected opening brace in variant match!"}},
    {ErrorType::VARIANTMATCH_EXPECTED_RBRACE,
     {SYNTAX_ERROR, "Expected closing brace in variant match!"}},

    // Flow control statements
    {ErrorType::ELIF_EXPECTED_CONDITION, {SYNTAX_ERROR, "Expected condition in elif statement!"}},
    {ErrorType::ELIF_EXPECTED_BLOCK, {SYNTAX_ERROR, "Expected block in elif statement!"}},
    {ErrorType::IF_EXPECTED_CONDITION, {SYNTAX_ERROR, "Expected condition in if statement!"}},
    {ErrorType::IF_EXPECTED_BLOCK, {SYNTAX_ERROR, "Expected block in if statement!"}},
    {ErrorType::ELSE_EXPECTED_BLOCK, {SYNTAX_ERROR, "Expected block in else statement!"}},
    {ErrorType::FORRANGE_EXPECTED_UNTIL, {SYNTAX_ERROR, "Expected 'until' in for statement!"}},
    {ErrorType::FORRANGE_EXPECTED_END_EXPR,
     {SYNTAX_ERROR, "Expected end expression in for statement!"}},
    {ErrorType::FOR_EXPECTED_IDENTIFIER, {SYNTAX_ERROR, "Expected identifier in for statement!"}},
    {ErrorType::FOR_EXPECTED_IN, {SYNTAX_ERROR, "Expected 'in' in for statement!"}},
    {ErrorType::FOR_EXPECTED_RANGE, {SYNTAX_ERROR, "Expected range in for statement!"}},
    {ErrorType::FOR_EXPECTED_BLOCK, {SYNTAX_ERROR, "Expected block in for statement!"}},
    {ErrorType::WHILE_EXPECTED_CONDITION, {SYNTAX_ERROR, "Expected condition in while statement!"}},
    {ErrorType::WHILE_EXPECTED_BLOCK, {SYNTAX_ERROR, "Expected block in while statement!"}},
    {ErrorType::CONTINUE_EXPECTED_SEMICOLON,
     {SYNTAX_ERROR, "Missing semicolon at the end of continue statement!"}},
    {ErrorType::BREAK_EXPECTED_SEMICOLON,
     {SYNTAX_ERROR, "Missing semicolon at the end of break statement!"}},
    {ErrorType::RETURN_EXPECTED_SEMICOLON,
     {SYNTAX_ERROR, "Missing semicolon at the end of return statement!"}},

    /* ----------------------------- Semantic Errors ---------------------------- */

    {ErrorType::EXPECTED_MAIN_FUNCTION_DEF,
     {SEMANTIC_ERROR, "Expected main function definition (fn main() -> int { return 0; }) !"}},
    {ErrorType::REDEFINITION,
     {SEMANTIC_ERROR, "Redefinition, identifier with that name already exists!"}},
    {ErrorType::VARIABLE_REDEFINITION,
     {SEMANTIC_ERROR, "Variable redefinition, identifier with that name already exists!"}},
    {ErrorType::TYPE_REDEFINITION,
     {SEMANTIC_ERROR, "Type redefinition, identifier with that name already exists!"}},
    {ErrorType::UNDEFINED_VARIABLE, {SEMANTIC_ERROR, "Use of undefined variable!"}},
    {ErrorType::UNDEFINED_TYPE, {SEMANTIC_ERROR, "Use of undefined type!"}},
    {ErrorType::STRUCTMEMBER_REDEFINITION, {SEMANTIC_ERROR, "Struct member redefinition!"}},
    {ErrorType::VARIANTMATCHCASE_REDEFINITION,
     {SEMANTIC_ERROR, "Variant match case redefinition!"}},
    {ErrorType::OBJECTMEMBER_REDEFINITION, {SEMANTIC_ERROR, "Object member redefinition!"}},
    {ErrorType::VARIANTTYPE_REDEFINITION, {SEMANTIC_ERROR, "Variant type redefinition!"}},
    {ErrorType::FNPARAM_REDEFINITION, {SEMANTIC_ERROR, "Function parameter redefinition!"}},
};
