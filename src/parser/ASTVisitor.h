#pragma once

#include <string>

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
  virtual void visit([[maybe_unused]] Program& program) {}

  virtual void visit([[maybe_unused]] VarDef& def) {}
  virtual void visit([[maybe_unused]] ConstDef& def) {}
  virtual void visit([[maybe_unused]] StructDef& def) {}
  virtual void visit([[maybe_unused]] StructMember& member) {}
  virtual void visit([[maybe_unused]] VariantDef& def) {}
  virtual void visit([[maybe_unused]] FnDef& def) {}
  virtual void visit([[maybe_unused]] FnParam& param) {}

  virtual void visit([[maybe_unused]] BinaryExpression& expr) {}
  virtual void visit([[maybe_unused]] UnaryExpression& expr) {}
  virtual void visit([[maybe_unused]] FunctionalExpression& expr) {}
  virtual void visit([[maybe_unused]] MemberAccessPostfix& postfix) {}
  virtual void visit([[maybe_unused]] VariantAccessPostfix& postfix) {}
  virtual void visit([[maybe_unused]] FnCallPostfix& postfix) {}
  virtual void visit([[maybe_unused]] IdentifierExpr& expr) {}
  virtual void visit([[maybe_unused]] Literal<int>& expr) {}
  virtual void visit([[maybe_unused]] Literal<float>& expr) {}
  virtual void visit([[maybe_unused]] Literal<bool>& expr) {}
  virtual void visit([[maybe_unused]] Literal<wchar_t>& expr) {}
  virtual void visit([[maybe_unused]] Literal<std::wstring>& expr) {}
  virtual void visit([[maybe_unused]] Object& expr) {}
  virtual void visit([[maybe_unused]] ObjectMember& expr) {}
  virtual void visit([[maybe_unused]] ParenExpr& expr) {}
  virtual void visit([[maybe_unused]] CastExpr& expr) {}

  virtual void visit([[maybe_unused]] BlockStmt& stmt) {}
  virtual void visit([[maybe_unused]] ExpressionStmt& stmt) {}
  virtual void visit([[maybe_unused]] AssignmentStmt& stmt) {}
  virtual void visit([[maybe_unused]] StdinExtractionStmt& stmt) {}
  virtual void visit([[maybe_unused]] StdoutInsertionStmt& stmt) {}
  virtual void visit([[maybe_unused]] VariantMatchStmt& stmt) {}
  virtual void visit([[maybe_unused]] VariantMatchCase& stmt) {}
  virtual void visit([[maybe_unused]] IfStmt& stmt) {}
  virtual void visit([[maybe_unused]] Else& stmt) {}
  virtual void visit([[maybe_unused]] Elif& stmt) {}
  virtual void visit([[maybe_unused]] ForStmt& stmt) {}
  virtual void visit([[maybe_unused]] Range& stmt) {}
  virtual void visit([[maybe_unused]] WhileStmt& stmt) {}
  virtual void visit([[maybe_unused]] ContinueStmt& stmt) {}
  virtual void visit([[maybe_unused]] BreakStmt& stmt) {}
  virtual void visit([[maybe_unused]] ReturnStmt& stmt) {}
};
