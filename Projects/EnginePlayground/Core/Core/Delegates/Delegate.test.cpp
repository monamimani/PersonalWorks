#include "DelegateUtils.test.h"

namespace DelegateTests
{

template <typename T>
class DelegateSingleLifecycle : public DelegateTest {};

TYPED_TEST_SUITE_P(DelegateSingleLifecycle);

TYPED_TEST_P(DelegateSingleLifecycle, DefaultConstructor)
{
  Delegate::Delegate<FctSignature> delegate;
  EXPECT_FALSE(delegate);
}

TYPED_TEST_P(DelegateSingleLifecycle, BindAndInvoke)
{
  auto [delegate, connection] = this->template createBound<TypeParam>();

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

TYPED_TEST_P(DelegateSingleLifecycle, CopyConstructor)
{
  auto [delegate, connection] = this->template createBound<TypeParam>();

  Delegate::Delegate<FctSignature> copy(delegate);
  EXPECT_EQ(delegate, copy);

  if constexpr (!std::is_same_v<TypeParam, EmptyPolicy>)
  {
    ASSERT_TRUE(copy);
    int value = 0;
    copy.invoke(value);
    EXPECT_EQ(value, DelegateTest::getExpectedValue(TypeParam::IsConst));
  }
}

TYPED_TEST_P(DelegateSingleLifecycle, MoveConstructor)
{
  auto [delegate, connection] = this->template createBound<TypeParam>();

  Delegate::Delegate<FctSignature> moved(std::move(delegate));
  
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

TYPED_TEST_P(DelegateSingleLifecycle, Equality)
{
  auto [delegate1, connection1] = this->template createBound<TypeParam>();
  auto [delegate2, connection2] = this->template createBound<TypeParam>();

  EXPECT_EQ(delegate1, delegate2);
  EXPECT_FALSE(delegate1 != delegate2);

  Delegate::Delegate<FctSignature> emptyDelegate;
  if constexpr (std::is_same_v<TypeParam, EmptyPolicy>)
  {
    EXPECT_EQ(delegate1, emptyDelegate);
  }
  else
  {
    EXPECT_NE(delegate1, emptyDelegate);
  }
}

TYPED_TEST_P(DelegateSingleLifecycle, Assignment)
{
  auto [delegate1, connection1] = this->template createBound<TypeParam>();

  Delegate::Delegate<FctSignature> delegate2;
  delegate2 = delegate1;
  EXPECT_EQ(delegate1, delegate2);

  Delegate::Delegate<FctSignature> delegate3;
  delegate3 = std::move(delegate1);
  EXPECT_EQ(delegate2, delegate3);
}

TYPED_TEST_P(DelegateSingleLifecycle, Swap)
{
  auto [delegate1, connection1] = this->template createBound<TypeParam>();
  Delegate::Delegate<FctSignature> delegate2; // Empty

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

REGISTER_TYPED_TEST_SUITE_P(DelegateSingleLifecycle,
  DefaultConstructor,
  BindAndInvoke,
  CopyConstructor,
  MoveConstructor,
  Equality,
  Assignment,
  Swap
);

INSTANTIATE_TYPED_TEST_SUITE_P(DelegateSingle, DelegateSingleLifecycle, DelegatePolicies);

// --- Functional Tests ---

class DelegateSingle : public DelegateTest {};

TEST_F(DelegateSingle, Unbinding)
{
  Delegate::Delegate<FctSignature> delegate;
  {
    auto [tempDelegate, connection] = createBound<FreeFunctionPolicy>();
    delegate = std::move(tempDelegate);
    EXPECT_TRUE(delegate);
  } // connection goes out of scope and unbinds

  EXPECT_FALSE(delegate);
}

TEST_F(DelegateSingle, ExplicitUnbind)
{
  auto [delegate, connection] = createBound<FreeFunctionPolicy>();
  EXPECT_TRUE(delegate);

  connection.unbind();
  EXPECT_FALSE(delegate);
}

TEST_F(DelegateSingle, Rebinding)
{
  auto [delegate, connection1] = createBound<FreeFunctionPolicy>();
  EXPECT_TRUE(delegate);

  // Rebinding with a new connection should replace the old one
  auto connection2 = MemberFunctionPolicy::bind(delegate, m_testStruct);
  EXPECT_TRUE(delegate);

  int value = 0;
  delegate.invoke(value);
  EXPECT_EQ(value, getExpectedValue(MemberFunctionPolicy::IsConst));

  connection2.unbind();
  EXPECT_FALSE(delegate);
}

TEST_F(DelegateSingle, LambdaWithCapture)
{
  auto [delegate, connection0] = createBound<EmptyPolicy>();
  int capture = 10;
  auto connection = delegate.bind([capture](int& value) { value = capture; });

  int result = 0;
  delegate.invoke(result);
  EXPECT_EQ(result, 10);
}

} // namespace DelegateTests
