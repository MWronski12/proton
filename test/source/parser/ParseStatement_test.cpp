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
  m_reader.load(L"match { case int -> {} case float -> {} }");
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
  m_reader.load(L"elif {}");
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
  m_reader.load(L"if {}");
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
  m_reader.load(L"for x in {}");
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

/* ------------------------------ ContinueStmt ------------------------------ */

/* ------------------------------ BreakStmt --------------------------------- */

/* ------------------------------- ReturnStmt ------------------------------- */

/* ------------------------------ BlockStmt --------------------------------- */

/* ------------------------ ExpressionOrAssignmentStmt ---------------------- */

/* -------------------------------- Statement ------------------------------- */
