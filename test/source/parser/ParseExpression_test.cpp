#include "ParserTest.h"

TEST_F(ParserTest, ParserHandlesBasicBinaryExpression) {
  m_reader.load(L"5 - 5");
  consumeToken();

  auto expr = parseExpression();
  auto binExpr = dynamic_cast<BinaryExpression *>(expr.get());
  ASSERT_TRUE(binExpr != nullptr);
  ASSERT_EQ(binExpr->op, Operator::Sub);

  auto left = dynamic_cast<Literal<int> *>(binExpr->left.get());
  auto right = dynamic_cast<Literal<int> *>(binExpr->right.get());
  ASSERT_TRUE(left != nullptr);
  ASSERT_TRUE(right != nullptr);
}

TEST_F(ParserTest, ParserHandlesOperatorPrecedence) {
  m_reader.load(L"6 - 3 * 2");
  consumeToken();

  auto expr = parseExpression();
  auto binExpr = dynamic_cast<BinaryExpression *>(expr.get());
  ASSERT_TRUE(binExpr != nullptr);
  ASSERT_EQ(binExpr->op, Operator::Sub);

  auto left = dynamic_cast<Literal<int> *>(binExpr->left.get());
  auto right = dynamic_cast<BinaryExpression *>(binExpr->right.get());
  ASSERT_TRUE(left != nullptr);
  ASSERT_TRUE(right != nullptr);

  auto rightLeft = dynamic_cast<Literal<int> *>(right->left.get());
  auto rightRight = dynamic_cast<Literal<int> *>(right->right.get());
  ASSERT_TRUE(rightLeft != nullptr);
  ASSERT_TRUE(rightRight != nullptr);
}

TEST_F(ParserTest, ParserHandlesComplexExpressions) {
  m_reader.load(L" (3 * squareRoot(pow(3, 2) + pow(4, 2)) + 3 * pi() * pow(r, 2)) / 2");
  consumeToken();

  auto expr = parseExpression();
  auto binExpr = dynamic_cast<BinaryExpression *>(expr.get());
  ASSERT_TRUE(binExpr != nullptr);
  ASSERT_EQ(binExpr->op, Operator::Div);
}

TEST_F(ParserTest, ParserHandlesOperatorUnaryExpressionsPrescedence) {
  m_reader.load(L"3 * -pow(3, 2)");
  consumeToken();

  auto expr = parseExpression();
  auto binExpr = dynamic_cast<BinaryExpression *>(expr.get());
  ASSERT_TRUE(binExpr != nullptr);
  ASSERT_EQ(binExpr->op, Operator::Mul);

  auto left = dynamic_cast<Literal<int> *>(binExpr->left.get());
  auto right = dynamic_cast<UnaryExpression *>(binExpr->right.get());
  ASSERT_TRUE(left != nullptr);
  ASSERT_TRUE(right != nullptr);

  auto fnCall = dynamic_cast<FunctionalExpression *>(right->expr.get());
  ASSERT_TRUE(fnCall != nullptr);
}

TEST_F(ParserTest, ParserHandlesChainedExpressions) {
  m_reader.load(L"foo().bar as int");
  consumeToken();

  auto expr = parseExpression();

  auto variantAccess = dynamic_cast<FunctionalExpression *>(expr.get());
  ASSERT_TRUE(variantAccess != nullptr);
  auto variantAccessPostfix = dynamic_cast<VariantAccess::Postfix *>(variantAccess->postfix.get());
  ASSERT_TRUE(variantAccessPostfix != nullptr);

  auto memberAccess = dynamic_cast<FunctionalExpression *>(variantAccess->expr.get());
  ASSERT_TRUE(memberAccess != nullptr);
  auto memberAccessPostfix = dynamic_cast<MemberAccess::Postfix *>(memberAccess->postfix.get());
  ASSERT_TRUE(memberAccessPostfix != nullptr);

  auto fnCall = dynamic_cast<FunctionalExpression *>(memberAccess->expr.get());
  ASSERT_TRUE(fnCall != nullptr);
  auto fnCallPostfix = dynamic_cast<FnCall::Postfix *>(fnCall->postfix.get());
  ASSERT_TRUE(fnCallPostfix != nullptr);
}
