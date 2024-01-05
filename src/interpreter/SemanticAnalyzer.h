#pragma once

#include <string>

// Todo remove
#include <iostream>

#include "ASTNode.h"
#include "ASTVisitor.h"
#include "Definition.h"
#include "Environment.h"
#include "ErrorHandler.h"
#include "Expression.h"
#include "Program.h"
#include "Statement.h"

namespace Interpreter {

class SemanticAnalyzer : public ::ASTVisitor {
  void expect(bool condition, ErrorType error, Position position);

 public:
  SemanticAnalyzer(Environment& environment, ::ErrorHandler& error);

  void visit(::Program& program) override;

  void visit(::VarDef& var) override;
  void visit(::ConstDef& constant) override;
  void visit(::StructDef& structure) override;
  void visit(::StructMember& member) override;
  void visit(::VariantDef& variant) override;
  void visit(::FnDef& def) override;
  void visit(::FnParam& param) override;

  //   void visit(::BinaryExpression& expr) override;
  // void visit(::UnaryExpression& expr) override;
  // void visit(::FunctionalExpression& expr) override;
  // void visit(::MemberAccessPostfix& postfix) override;
  // void visit(::VariantAccessPostfix& postfix) override;
  // void visit(::FnCallPostfix& postfix) override;
  // void visit(::IdentifierExpr& expr) override;
  void visit(::Literal<int>& expr) override;
  void visit(::Literal<float>& expr) override;
  void visit(::Literal<bool>& expr) override;
  void visit(::Literal<wchar_t>& expr) override;
  void visit(::Literal<std::wstring>& expr) override;
  // void visit(::Object& expr) override;
  // void visit(::ObjectMember& expr) override;
  // void visit(::ParenExpr& expr) override;
  // void visit(::CastExpr& expr) override;

  void visit(::BlockStmt& stmt) override;

  // void visit(::ExpressionStmt& stmt) override;
  // void visit(::AssignmentStmt& stmt) override;
  // void visit(::StdinExtractionStmt& stmt) override;
  // void visit(::StdoutInsertionStmt& stmt) override;
  // void visit(::VariantMatchStmt& stmt) override;
  // void visit(::VariantMatchCase& stmt) override;
  // void visit(::IfStmt& stmt) override;
  // void visit(::Else& stmt) override;
  // void visit(::Elif& stmt) override;
  // void visit(::ForStmt& stmt) override;
  // void visit(::Range& stmt) override;
  // void visit(::WhileStmt& stmt) override;
  // void visit(::ContinueStmt& stmt) override;
  // void visit(::BreakStmt& stmt) override;
  // void visit(::ReturnStmt& stmt) override;

 private:
  Environment& m_env;
  ErrorHandler& m_error;
};

}  // namespace Interpreter