#pragma once

struct Program;

struct VarDef;
struct ConstDef;
struct StructDef;
struct StructMember;
struct VariantDef;
struct FnDef;
struct FnParam;

struct BinaryExpression;
struct UnaryExpression;
struct FunctionalExpression;
struct MemberAccessPostfix;
struct VariantAccessPostfix;
struct FnCallPostfix;
struct IdentifierExpr;
template <typename T>
struct Literal;
struct Object;
struct ObjectMember;
struct ParenExpr;
struct CastExpr;

struct BlockStmt;
struct ExpressionStmt;
struct AssignmentStmt;
struct StdinExtractionStmt;
struct StdoutInsertionStmt;
struct VariantMatchStmt;
struct VariantMatchCase;
struct IfStmt;
struct Else;
struct Elif;
struct ForStmt;
struct Range;
struct WhileStmt;
struct ContinueStmt;
struct BreakStmt;
struct ReturnStmt;

class ASTVisitor {
 public:
  virtual void visit(Program& program) = 0;

  virtual void visit(VarDef& def) = 0;
  virtual void visit(ConstDef& def) = 0;
  virtual void visit(StructDef& def) = 0;
  virtual void visit(StructMember& member) = 0;
  virtual void visit(VariantDef& def) = 0;
  virtual void visit(FnDef& def) = 0;
  virtual void visit(FnParam& param) = 0;

  virtual void visit(BinaryExpression& expr) = 0;
  virtual void visit(UnaryExpression& expr) = 0;
  virtual void visit(FunctionalExpression& expr) = 0;
  virtual void visit(MemberAccessPostfix& postfix) = 0;
  virtual void visit(VariantAccessPostfix& postfix) = 0;
  virtual void visit(FnCallPostfix& postfix) = 0;
  virtual void visit(IdentifierExpr& expr) = 0;
  virtual void visit(Literal<int>& expr) = 0;
  virtual void visit(Literal<float>& expr) = 0;
  virtual void visit(Literal<bool>& expr) = 0;
  virtual void visit(Literal<wchar_t>& expr) = 0;
  virtual void visit(Literal<std::wstring>& expr) = 0;
  virtual void visit(Object& expr) = 0;
  virtual void visit(ObjectMember& expr) = 0;
  virtual void visit(ParenExpr& expr) = 0;
  virtual void visit(CastExpr& expr) = 0;

  virtual void visit(BlockStmt& stmt) = 0;
  virtual void visit(ExpressionStmt& stmt) = 0;
  virtual void visit(AssignmentStmt& stmt) = 0;
  virtual void visit(StdinExtractionStmt& stmt) = 0;
  virtual void visit(StdoutInsertionStmt& stmt) = 0;
  virtual void visit(VariantMatchStmt& stmt) = 0;
  virtual void visit(VariantMatchCase& stmt) = 0;
  virtual void visit(IfStmt& stmt) = 0;
  virtual void visit(Else& stmt) = 0;
  virtual void visit(Elif& stmt) = 0;
  virtual void visit(ForStmt& stmt) = 0;
  virtual void visit(Range& stmt) = 0;
  virtual void visit(WhileStmt& stmt) = 0;
  virtual void visit(ContinueStmt& stmt) = 0;
  virtual void visit(BreakStmt& stmt) = 0;
  virtual void visit(ReturnStmt& stmt) = 0;
};
