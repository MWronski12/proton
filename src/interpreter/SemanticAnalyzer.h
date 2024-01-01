#pragma once

// #include "ASTVisitor.h"

// class SymbolTable;

// class SemanticAnalyzer : public ASTVisitor {
//  public:
//   SemanticAnalyzer(SymbolTable& symbolTable) : m_symbolTable{symbolTable} {};

//   virtual void visit(Program& program);

//   virtual void visit(VarDef& def);
//   virtual void visit(ConstDef& def);
//   virtual void visit(StructDef& def);
//   virtual void visit(StructMember& member);
//   virtual void visit(VariantDef& def);
//   virtual void visit(FnDef& def);
//   virtual void visit(FnParam& param);

//   virtual void visit(BinaryExpression& expr);
//   virtual void visit(UnaryExpression& expr);
//   virtual void visit(FunctionalExpression& expr);
//   virtual void visit(MemberAccessPostfix& postfix);
//   virtual void visit(VariantAccessPostfix& postfix);
//   virtual void visit(FnCallPostfix& postfix);
//   virtual void visit(IdentifierExpr& expr);
//   virtual void visit(Literal<int>& expr);
//   virtual void visit(Literal<float>& expr);
//   virtual void visit(Literal<bool>& expr);
//   virtual void visit(Literal<wchar_t>& expr);
//   virtual void visit(Literal<std::wstring>& expr);
//   virtual void visit(Object& expr);
//   virtual void visit(ObjectMember& expr);
//   virtual void visit(ParenExpr& expr);
//   virtual void visit(CastExpr& expr);

//   virtual void visit(BlockStmt& stmt);
//   virtual void visit(ExpressionStmt& stmt);
//   virtual void visit(AssignmentStmt& stmt);
//   virtual void visit(StdinExtractionStmt& stmt);
//   virtual void visit(StdoutInsertionStmt& stmt);
//   virtual void visit(VariantMatchStmt& stmt);
//   virtual void visit(VariantMatchCase& stmt);
//   virtual void visit(IfStmt& stmt);
//   virtual void visit(Else& stmt);
//   virtual void visit(Elif& stmt);
//   virtual void visit(ForStmt& stmt);
//   virtual void visit(Range& stmt);
//   virtual void visit(WhileStmt& stmt);
//   virtual void visit(ContinueStmt& stmt);
//   virtual void visit(BreakStmt& stmt);
//   virtual void visit(ReturnStmt& stmt);

//  private:
//   SymbolTable& m_symbolTable;
// };
