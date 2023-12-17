#include "ParserTest.h"

/* -------------------------------- Postfixes ------------------------------- */

// FnCall::Postfix
TEST_F(ParserTest, ParserHandlesValidFnCallPostfix) {
  m_reader.load(L"()");
  consumeToken();
  auto result = parseFnCallPostfix();
  FnCall::Postfix* postfix = dynamic_cast<FnCall::Postfix*>(result.get());
  ASSERT_TRUE(postfix != nullptr);
  EXPECT_EQ(postfix->args.size(), 0);

  m_reader.load(L"(1, 2, 3)");
  consumeToken();
  result = parseFnCallPostfix();
  postfix = dynamic_cast<FnCall::Postfix*>(result.get());
  ASSERT_TRUE(postfix != nullptr);
  EXPECT_EQ(postfix->args.size(), 3);

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::FNCALL_EXPECTED_RPAREN, _)).Times(1);
  m_reader.load(L"(1, 2");
  postfix = dynamic_cast<FnCall::Postfix*>(result.get());
  consumeToken();
  result = parseFnCallPostfix();
  ASSERT_TRUE(result == nullptr);
}

TEST_F(ParserTest, ParserHandlesFnCallArgs) {
  m_reader.load(L"1, 2, 3");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);

  auto args = parseFnCallArgs();
  ASSERT_TRUE(args != std::nullopt);
  EXPECT_EQ(args->size(), 3);

  m_reader.load(L"");
  consumeToken();
  args = parseFnCallArgs();
  ASSERT_TRUE(args != std::nullopt);
  EXPECT_EQ(args->size(), 0);

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::FNCALL_EXPECTED_ARGUMENT, _)).Times(1);
  m_reader.load(L"1 ,");
  consumeToken();
  args = parseFnCallArgs();
  ASSERT_TRUE(args == std::nullopt);
}

// MemberAccess::Postfix
TEST_F(ParserTest, ParserHandlesMemberAccessPostfix) {
  m_reader.load(L".foo.bar");
  consumeToken();
  auto result = parseFunctionalExpressionPostfix();
  MemberAccess::Postfix* postfix = dynamic_cast<MemberAccess::Postfix*>(result.get());
  ASSERT_TRUE(postfix != nullptr);
  EXPECT_EQ(postfix->member, L"foo");

  result = parseFunctionalExpressionPostfix();
  postfix = dynamic_cast<MemberAccess::Postfix*>(result.get());
  ASSERT_TRUE(postfix != nullptr);
  EXPECT_EQ(postfix->member, L"bar");

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::MEMBERACCESS_EXPECTED_IDENTIFIER, _)).Times(1);
  m_reader.load(L".");
  consumeToken();
  result = parseFunctionalExpressionPostfix();
  postfix = dynamic_cast<MemberAccess::Postfix*>(result.get());
  ASSERT_TRUE(postfix == nullptr);
}

// VariantAccess::Postfix
TEST_F(ParserTest, ParserHandlesVariantAccessPostfix) {
  m_reader.load(L"as int as float");
  consumeToken();
  auto result = parseFunctionalExpressionPostfix();
  VariantAccess::Postfix* postfix = dynamic_cast<VariantAccess::Postfix*>(result.get());
  ASSERT_TRUE(postfix != nullptr);
  EXPECT_EQ(postfix->variant, L"int");

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::VARIANTACCESS_EXPECTED_TYPE_IDENTIFIER, _))
      .Times(1);
  m_reader.load(L"as");
  consumeToken();
  result = parseFunctionalExpressionPostfix();
  postfix = dynamic_cast<VariantAccess::Postfix*>(result.get());
  ASSERT_TRUE(postfix == nullptr);
}

/* -------------------------- FunctionalExpression -------------------------- */

TEST_F(ParserTest, ParserHandlesFunctionalExpressions) {
  m_reader.load(L"foo() foo.bar stink as string");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);

  auto result = parseFunctionalExpression();
  ASSERT_TRUE(result != nullptr);
  result = parseFunctionalExpression();
  ASSERT_TRUE(result != nullptr);
  result = parseFunctionalExpression();
  ASSERT_TRUE(result != nullptr);
}

TEST_F(ParserTest, ParserHandlesChainingFunctionalExpressions) {
  m_reader.load(L"Today as much as Me.like().programming() as now as Me.dont.want().anymore");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);

  auto result = parseFunctionalExpression();
  ASSERT_TRUE(result != nullptr);

  result = parseFunctionalExpression();
  ASSERT_TRUE(result == nullptr);
}
