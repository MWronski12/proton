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
 private:
  Environment& m_env;
  ErrorHandler& m_error;

  void expect(bool condition, ErrorType error, Position position) const;
  bool operatorIsSupported(const Type& type, ::Operator op) const;
  std::pair<TypeIdentifier, bool> isCastable(const Type& from, ::PrimitiveType to) const;
  bool exprIsAssignable(const std::unique_ptr<Expression>& expr) const;

  std::vector<Type> m_anonymousObjectTypes;

 public:
  SemanticAnalyzer(Environment& environment, ::ErrorHandler& error);

  // virtual void visit(Interpreter::BuiltinFunctionBody&) override;

  void visit(::Program& program) override;

  void visit(::VarDef& var) override;
  void visit(::ConstDef& constant) override;
  void visit(::StructDef& structure) override;
  void visit(::StructMember& member) override;
  void visit(::VariantDef& variant) override;
  void visit(::FnDef& def) override;
  void visit(::FnParam& param) override;

  void visit(::BinaryExpression& expr) override;
  void visit(::UnaryExpression& expr) override;
  void visit(::FunctionalExpression& expr) override;
  void visit(::MemberAccessPostfix& postfix) override;
  void visit(::VariantAccessPostfix& postfix) override;
  void visit(::FnCallPostfix& postfix) override;
  void visit(::IdentifierExpr& expr) override;
  void visit(::Literal<int>&) override;
  void visit(::Literal<float>&) override;
  void visit(::Literal<bool>&) override;
  void visit(::Literal<wchar_t>&) override;
  void visit(::Literal<std::wstring>&) override;
  void visit(::Object& expr) override;
  void visit(::ObjectMember& expr) override;
  void visit(::ParenExpr& expr) override;
  void visit(::CastExpr& expr) override;

  void visit(::BlockStmt& stmt) override;
  void visit(::ExpressionStmt& stmt) override;
  void visit(::AssignmentStmt& stmt) override;
  void visit(::StdinExtractionStmt& stmt) override;
  void visit(::StdoutInsertionStmt& stmt) override;
  void visit(::VariantMatchStmt& stmt) override;
  void visit(::VariantMatchCase& stmt) override;
  void visit(::IfStmt& stmt) override;
  void visit(::Else& stmt) override;
  void visit(::Elif& stmt) override;
  void visit(::ForStmt& stmt) override;
  void visit(::Range& stmt) override;
  void visit(::WhileStmt& stmt) override;
  void visit(::ContinueStmt& stmt) override;
  void visit(::BreakStmt& stmt) override;
  void visit(::ReturnStmt& stmt) override;
};

}  // namespace Interpreter