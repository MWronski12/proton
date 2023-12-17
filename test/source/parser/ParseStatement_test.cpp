#include "ParserTest.h"

/* --------------------------- StdinExtractionStmt -------------------------- */

TEST_F(ParserTest, ParserHandlesStdinExtractionStmt) {
  m_reader.load(L">> x;");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  auto stmt = parseStdinExtractionStmt();
  ASSERT_TRUE(stmt != nullptr);
}

TEST_F(ParserTest, ParserHandlesEmptyStdinExtractionStmt) {
  m_reader.load(L"");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  auto stmt = parseStdinExtractionStmt();
  ASSERT_TRUE(stmt == nullptr);
}

TEST_F(ParserTest, ParserHandlesStdinMissingExpr) {
  m_reader.load(L">> ;");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::STDINEXTRACTION_EXPECTED_EXPRESSION, _))
      .Times(1);
  auto stmt = parseStdinExtractionStmt();
  ASSERT_TRUE(stmt == nullptr);
}

TEST_F(ParserTest, ParserHandlesStdinMissingSemicolon) {
  m_reader.load(L">> x");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::STDINEXTRACTION_EXPECTED_SEMICOLON, _))
      .Times(1);
  auto stmt = parseStdinExtractionStmt();
  ASSERT_TRUE(stmt == nullptr);
}

/* --------------------------- StdoutInsertionStmt -------------------------- */

TEST_F(ParserTest, ParserHandlesStdoutInsertionStmt) {
  m_reader.load(L"<< x;");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  auto stmt = parseStdoutInsertionStmt();
  ASSERT_TRUE(stmt != nullptr);
}

TEST_F(ParserTest, ParserHandlesEmptyStdoutInsertionStmt) {
  m_reader.load(L"");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  auto stmt = parseStdoutInsertionStmt();
  ASSERT_TRUE(stmt == nullptr);
}

TEST_F(ParserTest, ParserHandlesStdoutMissingExpr) {
  m_reader.load(L"<< ;");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::STDOUTINSERTION_EXPECTED_EXPRESSION, _))
      .Times(1);
  auto stmt = parseStdoutInsertionStmt();
  ASSERT_TRUE(stmt == nullptr);
}

TEST_F(ParserTest, ParserHandlesStdoutMissingSemicolon) {
  m_reader.load(L"<< 5");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::STDOUTINSERTION_EXPECTED_SEMICOLON, _))
      .Times(1);
  auto stmt = parseStdoutInsertionStmt();
  ASSERT_TRUE(stmt == nullptr);
}

/* --------------------------- VariantMatch::Case --------------------------- */

TEST_F(ParserTest, ParserHandlesEmptyVariantMatchCase) {
  m_reader.load(L"");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  auto stmt = parseVariantMatchCase();
  ASSERT_TRUE(stmt == std::nullopt);
}

TEST_F(ParserTest, ParserHandlesVariantMatchCase) {
  m_reader.load(L"case int -> {}");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  auto stmt = parseVariantMatchCase();
  ASSERT_TRUE(stmt.has_value());
}

TEST_F(ParserTest, ParserHandlesVariantMatchCaseMissingType) {
  m_reader.load(L"case -> {}");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::VARIANTMATCHCASE_EXPECTED_TYPE, _)).Times(1);

  auto stmt = parseVariantMatchCase();
  ASSERT_FALSE(stmt.has_value());
}

TEST_F(ParserTest, ParserHandlesVariantMatchCaseMissingArrow) {
  m_reader.load(L"case int {}");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::VARIANTMATCHCASE_EXPECTED_ARROW, _)).Times(1);
  auto stmt = parseVariantMatchCase();
  ASSERT_FALSE(stmt.has_value());
}

TEST_F(ParserTest, ParserHandlesVariantMatchCaseMissingBlock) {
  m_reader.load(L"case int ->");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::VARIANTMATCHCASE_EXPECTED_BLOCK, _)).Times(1);
  auto stmt = parseVariantMatchCase();
  ASSERT_FALSE(stmt.has_value());
}

/* ------------------------- VariantMatchStmt::Cases ------------------------ */

TEST_F(ParserTest, ParserHandlesEmptyVariantCases) {
  m_reader.load(L"");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  auto cases = parseVariantMatchCases();
  ASSERT_TRUE(cases.has_value());
  ASSERT_EQ(cases->size(), 0);
}

TEST_F(ParserTest, ParserHandlesVariantCases) {
  m_reader.load(L"case int -> {} case float -> {}");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  auto cases = parseVariantMatchCases();
  ASSERT_TRUE(cases.has_value());
  ASSERT_EQ(cases->size(), 2);
}

/* ---------------------------- VariantMatchStmt ---------------------------- */

TEST_F(ParserTest, ParserHandlesEmptyVariantMatchStmt) {
  m_reader.load(L"");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  auto stmt = parseVariantMatchStmt();
  ASSERT_TRUE(stmt == nullptr);
}

TEST_F(ParserTest, ParserHandlesVariantMatchStmt) {
  m_reader.load(L"match x { case int -> {} case float -> {} }");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  auto stmt = parseVariantMatchStmt();
  ASSERT_TRUE(stmt != nullptr);
}

TEST_F(ParserTest, ParserHandlesVariantMatchStmtMissingExpr) {
  // instead of expression, we have to insert something that cannot be an expression
  m_reader.load(L"match var { case int -> {} case float -> {} }");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::VARIANTMATCH_EXPECTED_EXPRESSION, _)).Times(1);
  auto stmt = parseVariantMatchStmt();
  ASSERT_TRUE(stmt == nullptr);
}

TEST_F(ParserTest, ParserHandlesVariantMatchStmtMissingLBrace) {
  m_reader.load(L"match x case int -> {} case float -> {} }");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::VARIANTMATCH_EXPECTED_LBRACE, _)).Times(1);
  auto stmt = parseVariantMatchStmt();
  ASSERT_TRUE(stmt == nullptr);
}

TEST_F(ParserTest, ParserHandlesVariantMatchStmtMissingRBrace) {
  m_reader.load(L"match x { case int -> {} case float -> {}");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::VARIANTMATCH_EXPECTED_RBRACE, _)).Times(1);
  auto stmt = parseVariantMatchStmt();
  ASSERT_TRUE(stmt == nullptr);
}

/* ------------------------------ IfStmt::Elif ------------------------------ */

TEST_F(ParserTest, ParserHandlesEmptyElif) {
  m_reader.load(L"");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  auto stmt = parseElif();
  ASSERT_TRUE(stmt == std::nullopt);
}

TEST_F(ParserTest, ParserHandlesElif) {
  m_reader.load(L"elif x {}");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  auto stmt = parseElif();
  ASSERT_TRUE(stmt.has_value());
}

TEST_F(ParserTest, ParserHandlesElifMissingExpr) {
  // instead of expression, we have to insert something that cannot be an expression
  m_reader.load(L"elif . {}");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::ELIF_EXPECTED_CONDITION, _)).Times(1);
  auto stmt = parseElif();
  ASSERT_FALSE(stmt.has_value());
}

TEST_F(ParserTest, ParserHandlesElifMissingBlock) {
  m_reader.load(L"elif x");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::ELIF_EXPECTED_BLOCK, _)).Times(1);
  auto stmt = parseElif();
  ASSERT_FALSE(stmt.has_value());
}

/* ------------------------------- IfStmtElifs ------------------------------ */

TEST_F(ParserTest, ParserHandlesEmptyElifs) {
  m_reader.load(L"");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  auto stmt = parseElifs();
  EXPECT_EQ(stmt.size(), 0);
}

TEST_F(ParserTest, ParserHandlesElifs) {
  m_reader.load(L"elif x {} elif y {}");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  auto stmt = parseElifs();
  EXPECT_EQ(stmt.size(), 2);
}

/* ------------------------------ IfStmt::Else ------------------------------ */

TEST_F(ParserTest, ParserHandlesEmptyElse) {
  m_reader.load(L"");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  auto stmt = parseElse();
  ASSERT_TRUE(stmt == std::nullopt);
}

TEST_F(ParserTest, ParserHandlesElse) {
  m_reader.load(L"else {}");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  auto stmt = parseElse();
  ASSERT_TRUE(stmt.has_value());
}

TEST_F(ParserTest, ParserHandlesElseMissingBlock) {
  m_reader.load(L"else");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::ELSE_EXPECTED_BLOCK, _)).Times(1);
  auto stmt = parseElse();
  ASSERT_FALSE(stmt.has_value());
}

/* --------------------------------- IfStmt --------------------------------- */

TEST_F(ParserTest, ParserHandlesEmptyIfStmt) {
  m_reader.load(L"");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  auto stmt = parseIfStmt();
  ASSERT_TRUE(stmt == nullptr);
}

TEST_F(ParserTest, ParserHandlesIfStmt) {
  m_reader.load(L"if x {}");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  auto stmt = parseIfStmt();
  ASSERT_TRUE(stmt != nullptr);
}

TEST_F(ParserTest, ParserHandlesIfStmtMissingExpr) {
  // instead of expression, we have to insert something that cannot be an expression
  m_reader.load(L"if << {}");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::IF_EXPECTED_CONDITION, _)).Times(1);
  auto stmt = parseIfStmt();
  ASSERT_TRUE(stmt == nullptr);
}

TEST_F(ParserTest, ParserHandlesIfStmtMissingBlock) {
  m_reader.load(L"if x");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::IF_EXPECTED_BLOCK, _)).Times(1);
  auto stmt = parseIfStmt();
  ASSERT_TRUE(stmt == nullptr);
}

/* ------------------------------ ForStmt::Range ---------------------------- */

TEST_F(ParserTest, ParserHandlesEmptyForRange) {
  m_reader.load(L"");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  auto stmt = parseForRange();
  ASSERT_TRUE(stmt == std::nullopt);
}

TEST_F(ParserTest, ParserHandlesForRange) {
  m_reader.load(L"1 until 10");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  auto stmt = parseForRange();
  ASSERT_TRUE(stmt.has_value());
}

TEST_F(ParserTest, ParserHandlesForRangeMissingStartExpr) {
  m_reader.load(L" until 10");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  auto stmt = parseForRange();
  ASSERT_TRUE(stmt == std::nullopt);
}

TEST_F(ParserTest, ParserHandlesForRangeMissingUntil) {
  m_reader.load(L"1 10");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::FORRANGE_EXPECTED_UNTIL, _)).Times(1);
  auto stmt = parseForRange();
  ASSERT_TRUE(stmt == std::nullopt);
}

TEST_F(ParserTest, ParserHandlesForRangeMissingEndExpr) {
  m_reader.load(L"1 until");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::FORRANGE_EXPECTED_END_EXPR, _)).Times(1);
  auto stmt = parseForRange();
  ASSERT_TRUE(stmt == std::nullopt);
}

/* -------------------------------- ForStmt --------------------------------- */

TEST_F(ParserTest, ParserHandlesEmptyForStmt) {
  m_reader.load(L"");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  auto stmt = parseForStmt();
  ASSERT_TRUE(stmt == nullptr);
}

TEST_F(ParserTest, ParserHandlesForStmt) {
  m_reader.load(L"for x in 1 until 10 {}");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  auto stmt = parseForStmt();
  ASSERT_TRUE(stmt != nullptr);
}

TEST_F(ParserTest, ParserHandlesForStmtMissingIdentifier) {
  m_reader.load(L"for in 1 until 10 {}");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::FOR_EXPECTED_IDENTIFIER, _)).Times(1);
  auto stmt = parseForStmt();
  ASSERT_TRUE(stmt == nullptr);
}

TEST_F(ParserTest, ParserHandlesForStmtMissingIn) {
  m_reader.load(L"for x 1 until 10 {}");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::FOR_EXPECTED_IN, _)).Times(1);
  auto stmt = parseForStmt();
  ASSERT_TRUE(stmt == nullptr);
}

TEST_F(ParserTest, ParserHandlesForStmtMissingRange) {
  m_reader.load(L"for x in ... {}");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::FOR_EXPECTED_RANGE, _)).Times(1);
  auto stmt = parseForStmt();
  ASSERT_TRUE(stmt == nullptr);
}

TEST_F(ParserTest, ParserHandlesForStmtMissingBlock) {
  m_reader.load(L"for x in 1 until 10");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::FOR_EXPECTED_BLOCK, _)).Times(1);
  auto stmt = parseForStmt();
  ASSERT_TRUE(stmt == nullptr);
}

/* -------------------------------- WhileStmt ------------------------------- */

TEST_F(ParserTest, ParserHandlesEmptyWhileStmt) {
  m_reader.load(L"");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  auto stmt = parseWhileStmt();
  ASSERT_TRUE(stmt == nullptr);
}

TEST_F(ParserTest, ParserHandlesWhileStmt) {
  m_reader.load(L"while x {}");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  auto stmt = parseWhileStmt();
  ASSERT_TRUE(stmt != nullptr);
}

TEST_F(ParserTest, ParserHandlesWhileStmtMissingExpr) {
  // instead of expression, we have to insert something that cannot be an expression
  m_reader.load(L"while * {}");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::WHILE_EXPECTED_CONDITION, _)).Times(1);
  auto stmt = parseWhileStmt();
  ASSERT_TRUE(stmt == nullptr);
}

TEST_F(ParserTest, ParserHandlesWhileStmtMissingBlock) {
  m_reader.load(L"while x");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::WHILE_EXPECTED_BLOCK, _)).Times(1);
  auto stmt = parseWhileStmt();
  ASSERT_TRUE(stmt == nullptr);
}

/* ------------------------------ ContinueStmt ------------------------------ */

TEST_F(ParserTest, ParserHandlesEmptyContinueStmt) {
  m_reader.load(L"");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  auto stmt = parseContinueStmt();
  ASSERT_TRUE(stmt == nullptr);
}

TEST_F(ParserTest, ParserHandlesContinueStmt) {
  m_reader.load(L"continue;");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  auto stmt = parseContinueStmt();
  ASSERT_TRUE(stmt != nullptr);
}

TEST_F(ParserTest, ParserHandlesContinueStmtMissingSemicolon) {
  m_reader.load(L"continue");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::CONTINUE_EXPECTED_SEMICOLON, _)).Times(1);
  auto stmt = parseContinueStmt();
  ASSERT_TRUE(stmt == nullptr);
}

/* ------------------------------ BreakStmt --------------------------------- */

TEST_F(ParserTest, ParserHandlesEmptyBreakStmt) {
  m_reader.load(L"");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  auto stmt = parseBreakStmt();
  ASSERT_TRUE(stmt == nullptr);
}

TEST_F(ParserTest, ParserHandlesBreakStmt) {
  m_reader.load(L"break;");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  auto stmt = parseBreakStmt();
  ASSERT_TRUE(stmt != nullptr);
}

TEST_F(ParserTest, ParserHandlesBreakStmtMissingSemicolon) {
  m_reader.load(L"break");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::BREAK_EXPECTED_SEMICOLON, _)).Times(1);
  auto stmt = parseBreakStmt();
  ASSERT_TRUE(stmt == nullptr);
}

/* ------------------------------- ReturnStmt ------------------------------- */

TEST_F(ParserTest, ParserHandlesEmptyReturnStmt) {
  m_reader.load(L"");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  auto stmt = parseReturnStmt();
  ASSERT_TRUE(stmt == nullptr);
}

TEST_F(ParserTest, ParserHandlesReturnStmt) {
  m_reader.load(L"return;");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  auto stmt = parseReturnStmt();
  ASSERT_TRUE(stmt != nullptr);
}

TEST_F(ParserTest, ParserHandlesReturnStmtMissingSemicolon) {
  m_reader.load(L"return");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::RETURN_EXPECTED_SEMICOLON, _)).Times(1);
  auto stmt = parseReturnStmt();
  ASSERT_TRUE(stmt == nullptr);
}

/* ------------------------------ BlockStmt --------------------------------- */

TEST_F(ParserTest, ParserHandlesEmptyBlockStmt) {
  m_reader.load(L"");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  auto stmt = parseBlockStmt();
  ASSERT_TRUE(stmt == nullptr);
}

TEST_F(ParserTest, ParserHandlesBlockStmt) {
  m_reader.load(L"{ 5 + 5; foo(); }");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  auto stmt = parseBlockStmt();
  ASSERT_TRUE(stmt != nullptr);

  auto block = dynamic_cast<BlockStmt *>(stmt.get());
  EXPECT_EQ(block->statements.size(), 2);
}

/* ------------------------ ExpressionOrAssignmentStmt ---------------------- */

TEST_F(ParserTest, ParserHandlesEmptyExpressionOrAssignmentStmt) {
  m_reader.load(L"");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  auto stmt = parseExpressionOrAssignmentStmt();
  ASSERT_TRUE(stmt == nullptr);
}

TEST_F(ParserTest, ParserHandlesExpressionStmt) {
  m_reader.load(L"foo();");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  auto stmt = parseExpressionOrAssignmentStmt();
  ASSERT_TRUE(stmt != nullptr);

  auto expr = dynamic_cast<ExpressionStmt *>(stmt.get());
  ASSERT_TRUE(expr != nullptr);
}

TEST_F(ParserTest, ParserHandlesExpressionStmtMissingSemicolon) {
  m_reader.load(L"foo()");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::EXPRESSIONSTMT_EXPECTED_SEMICOLON, _))
      .Times(1);
  auto stmt = parseExpressionOrAssignmentStmt();
  ASSERT_TRUE(stmt == nullptr);
}

TEST_F(ParserTest, ParserHandlesAssignmentStmt) {
  m_reader.load(L"x = 5;");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  auto stmt = parseExpressionOrAssignmentStmt();
  ASSERT_TRUE(stmt != nullptr);

  auto assign = dynamic_cast<AssignmentStmt *>(stmt.get());
  ASSERT_TRUE(assign != nullptr);
}

TEST_F(ParserTest, ParserHandlesAssignmentStmtMissingExpr) {
  m_reader.load(L"x = ;");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::ASSIGNMENTSTMT_EXPECTED_EXPRESSION, _))
      .Times(1);
  auto stmt = parseExpressionOrAssignmentStmt();
  ASSERT_TRUE(stmt == nullptr);
}

TEST_F(ParserTest, ParserHandlesAssignmentStmtMissingSemicolon) {
  m_reader.load(L"x = 5");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::ASSIGNMENTSTMT_EXPECTED_SEMICOLON, _))
      .Times(1);
  auto stmt = parseExpressionOrAssignmentStmt();
  ASSERT_TRUE(stmt == nullptr);
}

/* -------------------------------- Statement ------------------------------- */

TEST_F(ParserTest, ParserHandlesAllStatements) {
  m_reader.load(
      L"fn main() -> int { return 1; }"
      L"{ 5 + 5; foo(); }"
      L"foo();"
      L"x = 5;"
      L">> x;"
      L"<< x;"
      L"match x { case int -> {} case float -> {} }"
      L"if x {} elif y {} else {}"
      L"for x in 1 until 10 {}"
      L"while x {}"
      L"continue;"
      L"break;"
      L"return;");

  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);

  // fn main() -> int { return 1; }
  auto stmt = parseStatement();
  EXPECT_TRUE(stmt != nullptr);
  // { 5 + 5; foo(); }
  stmt = parseStatement();
  EXPECT_TRUE(stmt != nullptr);
  // foo();
  stmt = parseStatement();
  EXPECT_TRUE(stmt != nullptr);
  // x = 5;
  stmt = parseStatement();
  EXPECT_TRUE(stmt != nullptr);
  // >> x;
  stmt = parseStatement();
  EXPECT_TRUE(stmt != nullptr);
  // << x;
  stmt = parseStatement();
  EXPECT_TRUE(stmt != nullptr);
  // match x { case int -> {} case float -> {} }
  stmt = parseStatement();
  EXPECT_TRUE(stmt != nullptr);
  // if x {} elif y {} else {}
  stmt = parseStatement();
  EXPECT_TRUE(stmt != nullptr);
  // for x in 1 until 10 {}
  stmt = parseStatement();
  EXPECT_TRUE(stmt != nullptr);
  // while x {}
  stmt = parseStatement();
  EXPECT_TRUE(stmt != nullptr);
  // continue;
  stmt = parseStatement();
  EXPECT_TRUE(stmt != nullptr);
  // break;
  stmt = parseStatement();
  EXPECT_TRUE(stmt != nullptr);
  // return;
  stmt = parseStatement();
  EXPECT_TRUE(stmt != nullptr);
}
