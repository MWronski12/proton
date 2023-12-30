#pragma once

#include <variant>

#include "ErrorHandler.h"
#include "ScopedTable.h"

class ScopeChecker : public ASTVisitor {
 public:
  ScopeChecker(ErrorHandler& errorHandler);

  void visit(Program& program) override;

  void visit(VarDef& def) override;
  void visit(ConstDef& def) override;
  void visit(StructDef& def) override;
  void visit(VariantDef& def) override;
  void visit(FnDef& def) override;

  void visit(BinaryExpression& expr) override;
  void visit(UnaryExpression& expr) override;
  void visit(FunctionalExpression& expr) override;
  void visit(FunctionalExpression::Postfix& postfix) override;
  void visit(MemberAccess::Postfix& postfix) override;
  void visit(VariantAccess::Postfix& postfix) override;
  void visit(FnCall::Postfix& postfix) override;
  void visit(PrimaryExpression& expr) override;
  void visit(IdentifierExpr& expr) override;
  void visit(Literal<int>& expr) override;
  void visit(Literal<float>& expr) override;
  void visit(Literal<bool>& expr) override;
  void visit(Literal<wchar_t>& expr) override;
  void visit(Literal<std::wstring>& expr) override;
  void visit(Object& expr) override;
  void visit(ParenExpr& expr) override;
  void visit(CastExpr& expr) override;

  void visit(BlockStmt& stmt) override;
  void visit(AssignmentStmt& stmt) override;
  void visit(StdinExtractionStmt& stmt) override;
  void visit(StdoutInsertionStmt& stmt) override;
  void visit(VariantMatchStmt& stmt) override;
  void visit(VariantMatchStmt::Case& stmt) override;
  void visit(IfStmt& stmt) override;
  void visit(IfStmt::Elif& stmt) override;
  void visit(IfStmt::Else& stmt) override;
  void visit(ForStmt& stmt) override;
  void visit(ForStmt::Range& stmt) override;
  void visit(WhileStmt& stmt) override;
  void visit(ContinueStmt& stmt) override;
  void visit(BreakStmt& stmt) override;
  void visit(ReturnStmt& stmt) override;

 private:
  ScopedTable<std::monostate> m_stack;

  ErrorHandler& m_errorHandler;
};
