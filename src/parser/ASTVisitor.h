#include "ASTNode.h"
#include "Definition.h"
#include "ErrorHandler.h"
#include "Expression.h"
#include "Program.h"
#include "Statement.h"

class ASTVisitor {
 public:
  ASTVisitor() = default;
  virtual ~ASTVisitor() = default;

  virtual void visit(Program& program) = 0;

  virtual void visit(VarDef& def) = 0;
  virtual void visit(ConstDef& def) = 0;
  virtual void visit(StructDef& def) = 0;
  virtual void visit(VariantDef& def) = 0;
  virtual void visit(FnDef& def) = 0;

  virtual void visit(BinaryExpression& expr) = 0;
  virtual void visit(UnaryExpression& expr) = 0;
  virtual void visit(FunctionalExpression& expr) = 0;
  virtual void visit(FunctionalExpression::Postfix& postfix) = 0;
  virtual void visit(MemberAccess::Postfix& postfix) = 0;
  virtual void visit(VariantAccess::Postfix& postfix) = 0;
  virtual void visit(FnCall::Postfix& postfix) = 0;
  virtual void visit(PrimaryExpression& expr) = 0;
  virtual void visit(IdentifierExpr& expr) = 0;
  virtual void visit(Literal<int>& expr) = 0;
  virtual void visit(Literal<float>& expr) = 0;
  virtual void visit(Literal<bool>& expr) = 0;
  virtual void visit(Literal<wchar_t>& expr) = 0;
  virtual void visit(Literal<std::wstring>& expr) = 0;
  virtual void visit(Object& expr) = 0;
  virtual void visit(ParenExpr& expr) = 0;
  virtual void visit(CastExpr& expr) = 0;

  virtual void visit(Statement& stmt) = 0;
  virtual void visit(BlockStmt& stmt) = 0;
  virtual void visit(AssignmentStmt& stmt) = 0;
  virtual void visit(StdinExtractionStmt& stmt) = 0;
  virtual void visit(StdoutInsertionStmt& stmt) = 0;
  virtual void visit(VariantMatchStmt& stmt) = 0;
  virtual void visit(VariantMatchStmt::Case& stmt) = 0;
  virtual void visit(IfStmt& stmt) = 0;
  virtual void visit(IfStmt::Elif& stmt) = 0;
  virtual void visit(IfStmt::Else& stmt) = 0;
  virtual void visit(ForStmt& stmt) = 0;
  virtual void visit(ForStmt::Range& stmt) = 0;
  virtual void visit(WhileStmt& stmt) = 0;
  virtual void visit(ContinueStmt& stmt) = 0;
  virtual void visit(BreakStmt& stmt) = 0;
  virtual void visit(ReturnStmt& stmt) = 0;
};
