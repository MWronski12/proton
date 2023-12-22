#include "ParserTest.h"

TEST_F(ParserTest, parserHandlesBasicBinaryExpression) {
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
