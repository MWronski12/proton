#pragma once

struct Program;

#include <string>  // For Literal<std::wstring>

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

namespace Interpreter {
struct BuiltinFunction;
}

class ASTVisitor {
 public:
  virtual void visit(Interpreter::BuiltinFunction&) {}

  virtual void visit(Program&) {}

  virtual void visit(VarDef&) {}
  virtual void visit(ConstDef&) {}
  virtual void visit(StructDef&) {}
  virtual void visit(StructMember&) {}
  virtual void visit(VariantDef&) {}
  virtual void visit(FnDef&) {}
  virtual void visit(FnParam&) {}

  virtual void visit(BinaryExpression&) {}
  virtual void visit(UnaryExpression&) {}
  virtual void visit(FunctionalExpression&) {}
  virtual void visit(MemberAccessPostfix&) {}
  virtual void visit(VariantAccessPostfix&) {}
  virtual void visit(FnCallPostfix&) {}
  virtual void visit(IdentifierExpr&) {}
  virtual void visit(Literal<int>&) {}
  virtual void visit(Literal<float>&) {}
  virtual void visit(Literal<bool>&) {}
  virtual void visit(Literal<wchar_t>&) {}
  virtual void visit(Literal<std::wstring>&) {}
  virtual void visit(Object&) {}
  virtual void visit(ObjectMember&) {}
  virtual void visit(ParenExpr&) {}
  virtual void visit(CastExpr&) {}

  virtual void visit(BlockStmt&) {}
  virtual void visit(ExpressionStmt&) {}
  virtual void visit(AssignmentStmt&) {}
  virtual void visit(StdinExtractionStmt&) {}
  virtual void visit(StdoutInsertionStmt&) {}
  virtual void visit(VariantMatchStmt&) {}
  virtual void visit(VariantMatchCase&) {}
  virtual void visit(IfStmt&) {}
  virtual void visit(Else&) {}
  virtual void visit(Elif&) {}
  virtual void visit(ForStmt&) {}
  virtual void visit(Range&) {}
  virtual void visit(WhileStmt&) {}
  virtual void visit(ContinueStmt&) {}
  virtual void visit(BreakStmt&) {}
  virtual void visit(ReturnStmt&) {}
};
