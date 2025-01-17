#include "Definition.h"
#include "ParserTest.h"

/* ------------------------------ VariantTypes ------------------------------ */

TEST_F(ParserTest, ParserHandlesEmptyVariantTypes) {
  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  m_reader.load(L"");
  consumeToken();

  auto result = parseVariantTypes();
  ASSERT_TRUE(result != std::nullopt);
  ASSERT_TRUE(result->empty());
}

TEST_F(ParserTest, ParserHandlesVariantTypes) {
  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  m_reader.load(L"int, float, string");
  consumeToken();

  auto result = parseVariantTypes();
  ASSERT_TRUE(result != std::nullopt);
  ASSERT_EQ(result->size(), 3);
  EXPECT_EQ(result->at(0), TypeIdentifier(L"int"));
  EXPECT_EQ(result->at(1), TypeIdentifier(L"float"));
  EXPECT_EQ(result->at(2), TypeIdentifier(L"string"));
}

TEST_F(ParserTest, ParserHandlesVariantTypeRedefintion) {
  EXPECT_CALL(m_errorHandler, handleError(ErrorType::VARIANTTYPE_REDEFINITION, _)).Times(1);
  m_reader.load(L"int, int");
  consumeToken();

  auto result = parseVariantTypes();
  ASSERT_TRUE(result == std::nullopt);
}

/* ------------------------------- VariantDef ------------------------------- */

TEST_F(ParserTest, ParserHandlesEmptyVariantDef) {
  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  m_reader.load(L"");
  consumeToken();

  auto result = parseVariantDef();
  EXPECT_EQ(result, nullptr);
}

TEST_F(ParserTest, ParserHandlesVariantDef) {
  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  m_reader.load(L"variant Foo { int, float, string };");
  consumeToken();

  auto result = parseVariantDef();
  ASSERT_TRUE(result != nullptr);
  VariantDef* def = dynamic_cast<VariantDef*>(result.get());
  ASSERT_TRUE(def != nullptr);
  ASSERT_TRUE(def->types.size() == 3);
}

TEST_F(ParserTest, ParserHandlesVariantDefAllowsEmptyVariant) {
  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  m_reader.load(L"variant Foo { };");
  consumeToken();

  auto result = parseVariantDef();
  ASSERT_TRUE(result != nullptr);
  VariantDef* def = dynamic_cast<VariantDef*>(result.get());
  ASSERT_TRUE(def != nullptr);
  ASSERT_TRUE(def->types.size() == 0);
}

TEST_F(ParserTest, ParserHandlesVariantDefAllowsTrailingComma) {
  EXPECT_CALL(m_errorHandler, handleError(_, _)).Times(0);
  m_reader.load(L"variant Foo { int, float, };");
  consumeToken();

  auto result = parseVariantDef();
  ASSERT_TRUE(result != nullptr);
  VariantDef* def = dynamic_cast<VariantDef*>(result.get());
  ASSERT_TRUE(def != nullptr);
  ASSERT_TRUE(def->types.size() == 2);
}

TEST_F(ParserTest, ParserHandlesVariantDefDoesntAllowCommaOnly) {
  m_reader.load(L"variant Foo { , };");
  consumeToken();

  EXPECT_CALL(m_errorHandler, handleError(ErrorType::VARIANTDEF_EXPECTED_RBRACE, _)).Times(1);
  auto result = parseVariantDef();
  ASSERT_TRUE(result == nullptr);
}
