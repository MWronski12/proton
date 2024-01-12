#include "Interpreter.h"

namespace Interpreter {

Interpreter::Interpreter(Environment& environment, ::ErrorHandler& error)
    : m_env(environment), m_error(error) {}

void Interpreter::visit(::VarDef& varDef) {

}

void Interpreter::visit(::ConstDef&) {}
void Interpreter::visit(::StructDef&) {}
void Interpreter::visit(::StructMember&) {}
void Interpreter::visit(::VariantDef&) {}
void Interpreter::visit(::FnDef&) {}
void Interpreter::visit(::FnParam&) {}

void Interpreter::visit(::BinaryExpression&) {}
void Interpreter::visit(::UnaryExpression&) {}
void Interpreter::visit(::FunctionalExpression&) {}
void Interpreter::visit(::MemberAccessPostfix&) {}
void Interpreter::visit(::VariantAccessPostfix&) {}
void Interpreter::visit(::FnCallPostfix&) {}
void Interpreter::visit(::IdentifierExpr&) {}
void Interpreter::visit(::Literal<int>&) {}
void Interpreter::visit(::Literal<float>&) {}
void Interpreter::visit(::Literal<bool>&) {}
void Interpreter::visit(::Literal<wchar_t>&) {}
void Interpreter::visit(::Literal<std::wstring>&) {}
void Interpreter::visit(::Object&) {}
void Interpreter::visit(::ObjectMember&) {}
void Interpreter::visit(::ParenExpr&) {}
void Interpreter::visit(::CastExpr&) {}

void Interpreter::visit(::BlockStmt&) {}
void Interpreter::visit(::ExpressionStmt&) {}
void Interpreter::visit(::AssignmentStmt&) {}
void Interpreter::visit(::StdinExtractionStmt&) {}
void Interpreter::visit(::StdoutInsertionStmt&) {}
void Interpreter::visit(::VariantMatchStmt&) {}
void Interpreter::visit(::VariantMatchCase&) {}
void Interpreter::visit(::IfStmt&) {}
void Interpreter::visit(::Else&) {}
void Interpreter::visit(::Elif&) {}
void Interpreter::visit(::ForStmt&) {}
void Interpreter::visit(::Range&) {}
void Interpreter::visit(::WhileStmt&) {}
void Interpreter::visit(::ContinueStmt&) {}
void Interpreter::visit(::BreakStmt&) {}
void Interpreter::visit(::ReturnStmt&) {}

}  // namespace Interpreter
