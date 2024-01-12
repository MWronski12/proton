#pragma once

#include "ASTVisitor.h"
#include "Environment.h"
#include "ErrorHandler.h"

namespace Interpreter {

class Interpreter : ASTVisitor {
 private:
  Environment& m_env;
  ErrorHandler& m_error;

 public:
  Interpreter(Environment& environment, ::ErrorHandler& error);

  void visit(::Program&) override;

  void visit(::VarDef&) override;
  void visit(::ConstDef&) override;
  void visit(::StructDef&) override;
  void visit(::StructMember&) override;
  void visit(::VariantDef&) override;
  void visit(::FnDef&) override;
  void visit(::FnParam&) override;

  void visit(::BinaryExpression&) override;
  void visit(::UnaryExpression&) override;
  void visit(::FunctionalExpression&) override;
  void visit(::MemberAccessPostfix&) override;
  void visit(::VariantAccessPostfix&) override;
  void visit(::FnCallPostfix&) override;
  void visit(::IdentifierExpr&) override;
  void visit(::Literal<int>&) override;
  void visit(::Literal<float>&) override;
  void visit(::Literal<bool>&) override;
  void visit(::Literal<wchar_t>&) override;
  void visit(::Literal<std::wstring>&) override;
  void visit(::Object&) override;
  void visit(::ObjectMember&) override;
  void visit(::ParenExpr&) override;
  void visit(::CastExpr&) override;

  void visit(::BlockStmt&) override;
  void visit(::ExpressionStmt&) override;
  void visit(::AssignmentStmt&) override;
  void visit(::StdinExtractionStmt&) override;
  void visit(::StdoutInsertionStmt&) override;
  void visit(::VariantMatchStmt&) override;
  void visit(::VariantMatchCase&) override;
  void visit(::IfStmt&) override;
  void visit(::Else&) override;
  void visit(::Elif&) override;
  void visit(::ForStmt&) override;
  void visit(::Range&) override;
  void visit(::WhileStmt&) override;
  void visit(::ContinueStmt&) override;
  void visit(::BreakStmt&) override;
  void visit(::ReturnStmt&) override;
};

}  // namespace Interpreter
