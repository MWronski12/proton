#pragma once

#include <list>
#include <string>

#include "ASTNode.h"
#include "ASTVisitor.h"
#include "Definition.h"
#include "Environment.h"
#include "ErrorHandler.h"
#include "Expression.h"
#include "Program.h"
#include "Statement.h"
#include "TypeCastHandler.h"
#include "TypeOperatorsHandler.h"

namespace Interpreter {

class SemanticAnalyzer : public ::ASTVisitor {
 private:
  Environment& m_env;
  ErrorHandler& m_error;

  TypeCastHandler m_castHandler;
  TypeOperatorsHandler m_operatorsHandler;

  bool m_fnHasReturnStmt = false;

  void expect(bool condition, ErrorType error, Position position) const;

  bool isAssignable(const TypePtr& objType, const TypePtr& assignedTo) const;
  bool exprIsAssignable(const std::unique_ptr<::Expression>& expr) const;

 public:
  SemanticAnalyzer(Environment& environment, ::ErrorHandler& error);

  // Todo:
  // virtual void visit(Interpreter::BuiltinFunctionBody&) override;

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