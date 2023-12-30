
ScopeChecker::ScopeChecker(ErrorHandler& errorHandler) : m_errorHandler{errorHandler} {}

// void ScopeChecker::visit(Program& program) {}

// void ScopeChecker::visit(Definition& def) {}
// void ScopeChecker::visit(VarDef& def) {}
// void ScopeChecker::visit(ConstDef& def) {}
// void ScopeChecker::visit(StructDef& def) {}
// void ScopeChecker::visit(VariantDef& def) {}
// void ScopeChecker::visit(FnDef& def) {}

// void ScopeChecker::visit(Expression& expr) {}
// void ScopeChecker::visit(BinaryExpression& expr) {}
// void ScopeChecker::visit(UnaryExpression& expr) {}
// void ScopeChecker::visit(FunctionalExpression& expr) {}
// void ScopeChecker::visit(FunctionalExpression::Postfix& postfix) {}
// void ScopeChecker::visit(MemberAccess::Postfix& postfix) {}
// void ScopeChecker::visit(VariantAccess::Postfix& postfix) {}
// void ScopeChecker::visit(FnCall::Postfix& postfix) {}
// void ScopeChecker::visit(PrimaryExpression& expr) {}
// void ScopeChecker::visit(IdentifierExpr& expr) {}
// void ScopeChecker::visit(Literal<int>& expr) {}
// void ScopeChecker::visit(Literal<float>& expr) {}
// void ScopeChecker::visit(Literal<bool>& expr) {}
// void ScopeChecker::visit(Literal<wchar_t>& expr) {}
// void ScopeChecker::visit(Literal<std::wstring>& expr) {}
// void ScopeChecker::visit(Object& expr) {}
// void ScopeChecker::visit(ParenExpr& expr) {}
// void ScopeChecker::visit(CastExpr& expr) {}

// void ScopeChecker::visit(Statement& stmt) {}
// void ScopeChecker::visit(BlockStmt& stmt) {}
// void ScopeChecker::visit(AssignmentStmt& stmt) {}
// void ScopeChecker::visit(StdinExtractionStmt& stmt) {}
// void ScopeChecker::visit(StdoutInsertionStmt& stmt) {}
// void ScopeChecker::visit(VariantMatchStmt& stmt) {}
// void ScopeChecker::visit(VariantMatchStmt::Case& stmt) {}
// void ScopeChecker::visit(IfStmt& stmt) {}
// void ScopeChecker::visit(IfStmt::Elif& stmt) {}
// void ScopeChecker::visit(IfStmt::Else& stmt) {}
// void ScopeChecker::visit(ForStmt& stmt) {}
// void ScopeChecker::visit(ForStmt::Range& stmt) {}
// void ScopeChecker::visit(WhileStmt& stmt) {}
// void ScopeChecker::visit(ContinueStmt& stmt) {}
// void ScopeChecker::visit(BreakStmt& stmt) {}
// void ScopeChecker::visit(ReturnStmt& stmt) {}