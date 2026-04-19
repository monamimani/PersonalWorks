#include "Delegates/DelegateUtils.test.h"

import StaticFunction;

namespace StaticFunctionTests
{
using namespace DelegateTests;

template <typename T>
class StaticFunctionLifecycle : public DelegateTest {};

TYPED_TEST_SUITE_P(StaticFunctionLifecycle);

TYPED_TEST_P(StaticFunctionLifecycle, DefaultConstructor)
{
  Core::StaticFunction<FctSignature> delegate;
  EXPECT_FALSE(delegate);
}

TYPED_TEST_P(StaticFunctionLifecycle, BindAndInvoke)
{
  auto [delegate, connection] = this->template createBound<TypeParam, Core::StaticFunction<FctSignature>>();

  if constexpr (std::is_same_v<TypeParam, EmptyPolicy>)
  {
    EXPECT_FALSE(delegate);
  }
  else
  {
    ASSERT_TRUE(delegate);
    int value = 0;
    delegate.invoke(value);
    EXPECT_EQ(value, DelegateTest::getExpectedValue(TypeParam::IsConst));
  }
}

TYPED_TEST_P(StaticFunctionLifecycle, CopyConstructor)
{
  auto [delegate, connection] = this->template createBound<TypeParam, Core::StaticFunction<FctSignature>>();

  Core::StaticFunction<FctSignature> copy(delegate);
  EXPECT_EQ(delegate, copy);

  if constexpr (!std::is_same_v<TypeParam, EmptyPolicy>)
  {
    ASSERT_TRUE(copy);
    int value = 0;
    copy.invoke(value);
    EXPECT_EQ(value, DelegateTest::getExpectedValue(TypeParam::IsConst));
  }
}

TYPED_TEST_P(StaticFunctionLifecycle, MoveConstructor)
{
  auto [delegate, connection] = this->template createBound<TypeParam, Core::StaticFunction<FctSignature>>();

  Core::StaticFunction<FctSignature> moved(std::move(delegate));
  
  if constexpr (std::is_same_v<TypeParam, EmptyPolicy>)
  {
    EXPECT_FALSE(moved);
  }
  else
  {
    ASSERT_TRUE(moved);
    int value = 0;
    moved.invoke(value);
    EXPECT_EQ(value, DelegateTest::getExpectedValue(TypeParam::IsConst));
  }
}

TYPED_TEST_P(StaticFunctionLifecycle, Equality)
{
  auto [delegate1, connection1] = this->template createBound<TypeParam, Core::StaticFunction<FctSignature>>();
  auto [delegate2, connection2] = this->template createBound<TypeParam, Core::StaticFunction<FctSignature>>();

  EXPECT_EQ(delegate1, delegate2);
  EXPECT_FALSE(delegate1 != delegate2);

  Core::StaticFunction<FctSignature> emptyDelegate;
  if constexpr (std::is_same_v<TypeParam, EmptyPolicy>)
  {
    EXPECT_EQ(delegate1, emptyDelegate);
  }
  else
  {
    EXPECT_NE(delegate1, emptyDelegate);
  }
}

TYPED_TEST_P(StaticFunctionLifecycle, Assignment)
{
  auto [delegate1, connection1] = this->template createBound<TypeParam, Core::StaticFunction<FctSignature>>();

  Core::StaticFunction<FctSignature> delegate2;
  delegate2 = delegate1;
  EXPECT_EQ(delegate1, delegate2);

  Core::StaticFunction<FctSignature> delegate3;
  delegate3 = std::move(delegate1);
  EXPECT_EQ(delegate2, delegate3);
}

TYPED_TEST_P(StaticFunctionLifecycle, Swap)
{
  auto [delegate1, connection1] = this->template createBound<TypeParam, Core::StaticFunction<FctSignature>>();
  Core::StaticFunction<FctSignature> delegate2; // Empty

  using std::swap;
  swap(delegate1, delegate2);

  if constexpr (std::is_same_v<TypeParam, EmptyPolicy>)
  {
    EXPECT_FALSE(delegate1);
    EXPECT_FALSE(delegate2);
  }
  else
  {
    EXPECT_FALSE(delegate1);
    EXPECT_TRUE(delegate2);
    int value = 0;
    delegate2.invoke(value);
    EXPECT_EQ(value, DelegateTest::getExpectedValue(TypeParam::IsConst));
  }
}

REGISTER_TYPED_TEST_SUITE_P(StaticFunctionLifecycle,
  DefaultConstructor,
  BindAndInvoke,
  CopyConstructor,
  MoveConstructor,
  Equality,
  Assignment,
  Swap
);

INSTANTIATE_TYPED_TEST_SUITE_P(StaticFunction, StaticFunctionLifecycle, DelegatePolicies);

// --- Functional Tests ---

class StaticFunctionTest : public DelegateTest {};

TEST_F(StaticFunctionTest, invokeSafe)
{
  auto [delegate, connection] = createBound<EmptyPolicy, Core::StaticFunction<int(int&)>>();
  int value = 0;

  auto result = delegate.invokeSafe(value);
  EXPECT_EQ(result, std::nullopt);

  delegate.bind<&TestStruct::fctReturn>(m_testStruct);
  ASSERT_TRUE(delegate.isBound());
  result = delegate.invokeSafe(value);
  EXPECT_NE(result, std::nullopt);
  EXPECT_EQ(result.value(), TestStruct::m_staticValue);
}

} // namespace StaticFunctionTests
