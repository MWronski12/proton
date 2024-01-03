#pragma once

// #include <string>

// // Todo remove
// #include <iostream>

// #include "ASTNode.h"
// #include "ASTVisitor.h"
// #include "Definition.h"
// #include "Environment.h"
// #include "ErrorHandler.h"
// #include "Expression.h"
// #include "Program.h"
// #include "Statement.h"

// namespace Interpreter {

// using namespace std::string_literals;

// /**
//  * @brief ScopeAnalyzer checks if all referenced symbols are defined and accessible in scope.
//  */
// class ScopeAnalyzer : public ::ASTVisitor {
//   void expect(bool condition, ErrorType error, Position position) {
//     if (!condition) {
//       m_error(error, position);
//     }
//   }

//  public:
//   ScopeAnalyzer(Environment& environment, ::ErrorHandler& error)
//       : m_env{environment}, m_error{error} {}

//   void visit(::Program& program) override {
//     for (auto& [name, definition] : program.definitions) {
//       definition->accept(*this);
//     }
//   }

//   void visit(::VarDef& var) override {
//     expect(!m_env.varIsDeclared(var.name), ErrorType::REDEFINITION, var.position);
//     expect(m_env.typeIsDefined(var.type), ErrorType::UNDEFINED_TYPE, var.position);
//     m_env.declareVar(var.name, var.type);
//   }

//   void visit(::ConstDef& constant) override {
//     expect(!m_env.varIsDeclared(constant.name), ErrorType::REDEFINITION, constant.position);
//     expect(m_env.typeIsDefined(constant.type), ErrorType::UNDEFINED_TYPE, constant.position);
//     m_env.declareVar(constant.name, constant.type, {Modifier::CONST});
//   };

//   void visit(::StructDef& structure) override {
//     expect(!m_env.typeIsDeclared(structure.name), ErrorType::REDEFINITION, structure.position);
//     m_env.declareType(structure.name);
//     for (auto& member : structure.members) {
//       member.second.accept(*this);
//     }
//   };

//   void visit(::StructMember& member) override {
//     expect(m_env.typeIsDefined(member.type), ErrorType::UNDEFINED_TYPE, member.position);
//   };

//   void visit(::VariantDef& variant) override {
//     expect(!m_env.typeIsDeclared(variant.name), ErrorType::REDEFINITION, variant.position);
//     m_env.declareType(variant.name);
//     for (auto& type : variant.types) {
//       expect(m_env.typeIsDefined(type), ErrorType::UNDEFINED_TYPE, variant.position);
//     }
//   };

//   void visit(::FnDef& def) override {
//     expect(!m_env.varIsDeclared(def.name), ErrorType::REDEFINITION, def.position);
//     expect(m_env.typeIsDefined(def.returnType), ErrorType::UNDEFINED_TYPE, def.position);

//     m_env.pushStackFrame(def.name);
//     for (auto& [_, param] : def.parameters) {
//       param.accept(*this);
//     }
//     m_env.declareType(def.name);
//     def.body.accept(*this);
//   };

//   void visit(::FnParam& param) override {
//     expect(m_env.typeIsDefined(param.type), ErrorType::UNDEFINED_TYPE, param.position);
//     expect(!m_env.varIsDeclared(param.name), ErrorType::REDEFINITION, param.position);

//     if (param.isConst) {
//       m_env.declareVar(param.name, param.type, {Modifier::CONST});
//     } else {
//       m_env.declareVar(param.name, param.type);
//     }
//   };

//   // void visit(::BinaryExpression& expr) override;
//   // void visit(::UnaryExpression& expr) override;
//   // void visit(::FunctionalExpression& expr) override;
//   // void visit(::MemberAccessPostfix& postfix) override;
//   // void visit(::VariantAccessPostfix& postfix) override;
//   // void visit(::FnCallPostfix& postfix) override;
//   // void visit(::IdentifierExpr& expr) override;
//   // void visit(::Literal<int>& expr) override;
//   // void visit(::Literal<float>& expr) override;
//   // void visit(::Literal<bool>& expr) override;
//   // void visit(::Literal<wchar_t>& expr) override;
//   // void visit(::Literal<std::wstring>& expr) override;
//   // void visit(::Object& expr) override;
//   // void visit(::ObjectMember& expr) override;
//   // void visit(::ParenExpr& expr) override;
//   // void visit(::CastExpr& expr) override;

//   void visit(::BlockStmt& stmt) override {
//     m_env.enterScope();
//     std::cout << stmt.statements.size() << std::endl;
//     m_env.exitScope();
//   }

//   // void visit(::ExpressionStmt& stmt) override;
//   // void visit(::AssignmentStmt& stmt) override;
//   // void visit(::StdinExtractionStmt& stmt) override;
//   // void visit(::StdoutInsertionStmt& stmt) override;
//   // void visit(::VariantMatchStmt& stmt) override;
//   // void visit(::VariantMatchCase& stmt) override;
//   // void visit(::IfStmt& stmt) override;
//   // void visit(::Else& stmt) override;
//   // void visit(::Elif& stmt) override;
//   // void visit(::ForStmt& stmt) override;
//   // void visit(::Range& stmt) override;
//   // void visit(::WhileStmt& stmt) override;
//   // void visit(::ContinueStmt& stmt) override;
//   // void visit(::BreakStmt& stmt) override;
//   // void visit(::ReturnStmt& stmt) override;

//  private:
//   Environment& m_env;
//   ErrorHandler& m_error;
// };

// }  // namespace Interpreter