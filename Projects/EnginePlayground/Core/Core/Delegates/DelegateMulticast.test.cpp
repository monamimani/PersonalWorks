#include "DelegateUtils.test.h"

namespace DelegateTests
{

template <typename T>
class DelegateMultiLifecycle : public DelegateTest {};

TYPED_TEST_SUITE_P(DelegateMultiLifecycle);

TYPED_TEST_P(DelegateMultiLifecycle, DefaultConstructor)
{
  Delegate::DelegateMulticast<FctSignature> delegate;
  EXPECT_FALSE(delegate);
}

TYPED_TEST_P(DelegateMultiLifecycle, BindAndInvoke)
{
  auto [delegate, connection] = this->template createBound<TypeParam, Delegate::DelegateMulticast<FctSignature>>();

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

TYPED_TEST_P(DelegateMultiLifecycle, CopyConstructor)
{
  auto [delegate, connection] = this->template createBound<TypeParam, Delegate::DelegateMulticast<FctSignature>>();

  Delegate::DelegateMulticast<FctSignature> copy(delegate);
  EXPECT_EQ(delegate, copy);

  if constexpr (!std::is_same_v<TypeParam, EmptyPolicy>)
  {
    ASSERT_TRUE(copy);
    int value = 0;
    copy.invoke(value);
    EXPECT_EQ(value, DelegateTest::getExpectedValue(TypeParam::IsConst));
  }
}

TYPED_TEST_P(DelegateMultiLifecycle, MoveConstructor)
{
  auto [delegate, connection] = this->template createBound<TypeParam, Delegate::DelegateMulticast<FctSignature>>();

  Delegate::DelegateMulticast<FctSignature> moved(std::move(delegate));
  
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

TYPED_TEST_P(DelegateMultiLifecycle, Equality)
{
  auto [delegate1, connection1] = this->template createBound<TypeParam, Delegate::DelegateMulticast<FctSignature>>();
  auto [delegate2, connection2] = this->template createBound<TypeParam, Delegate::DelegateMulticast<FctSignature>>();

  EXPECT_EQ(delegate1, delegate2);
  EXPECT_FALSE(delegate1 != delegate2);

  Delegate::DelegateMulticast<FctSignature> emptyDelegate;
  if constexpr (std::is_same_v<TypeParam, EmptyPolicy>)
  {
    EXPECT_EQ(delegate1, emptyDelegate);
  }
  else
  {
    EXPECT_NE(delegate1, emptyDelegate);
  }
}

TYPED_TEST_P(DelegateMultiLifecycle, Assignment)
{
  auto [delegate1, connection1] = this->template createBound<TypeParam, Delegate::DelegateMulticast<FctSignature>>();

  Delegate::DelegateMulticast<FctSignature> delegate2;
  delegate2 = delegate1;
  EXPECT_EQ(delegate1, delegate2);

  Delegate::DelegateMulticast<FctSignature> delegate3;
  delegate3 = std::move(delegate1);
  EXPECT_EQ(delegate2, delegate3);
}

TYPED_TEST_P(DelegateMultiLifecycle, Swap)
{
  auto [delegate1, connection1] = this->template createBound<TypeParam, Delegate::DelegateMulticast<FctSignature>>();
  Delegate::DelegateMulticast<FctSignature> delegate2; // Empty

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

REGISTER_TYPED_TEST_SUITE_P(DelegateMultiLifecycle,
  DefaultConstructor,
  BindAndInvoke,
  CopyConstructor,
  MoveConstructor,
  Equality,
  Assignment,
  Swap
);

INSTANTIATE_TYPED_TEST_SUITE_P(DelegateMulti, DelegateMultiLifecycle, MulticastDelegatePolicies);

// --- Functional Tests ---

class DelegateMulti : public DelegateTest {};

TEST_F(DelegateMulti, MultipleBindings)
{
  auto [delegate, connection1] = createBound<FreeFunctionPolicy, Delegate::DelegateMulticast<FctSignature>>();
  int count = 0;
  auto connection2 = delegate.bind([&count](int&) { count++; });

  int value = 0;
  delegate.invoke(value);
  EXPECT_EQ(count, 1);
  EXPECT_EQ(value, TestStruct::m_staticValue);
}

TEST_F(DelegateMulti, InvocationOrder)
{
  auto [delegate, connection0] = createBound<EmptyPolicy, Delegate::DelegateMulticast<FctSignature>>();
  std::vector<int> order;
  auto connection1 = delegate.bind([&order](int&) { order.push_back(1); });
  auto connection2 = delegate.bind([&order](int&) { order.push_back(2); });

  int value = 0;
  delegate.invoke(value);
  ASSERT_EQ(order.size(), 2);
  EXPECT_EQ(order[0], 1);
  EXPECT_EQ(order[1], 2);
}

TEST_F(DelegateMulti, UnbindOne)
{
  auto [delegate, connection1] = createBound<FreeFunctionPolicy, Delegate::DelegateMulticast<FctSignature>>();
  int count = 0;
  auto connection2 = delegate.bind([&count](int&) { count++; });
  {
    auto connection3 = delegate.bind([&count](int&) { count++; });
    EXPECT_TRUE(delegate);
  } // connection3 unbinds

  int value = 0;
  delegate.invoke(value);
  EXPECT_EQ(count, 1);
  EXPECT_EQ(value, TestStruct::m_staticValue);
}

TEST_F(DelegateMulti, UnbindAll)
{
  auto [delegate, connection1] = createBound<FreeFunctionPolicy, Delegate::DelegateMulticast<FctSignature>>();
  auto connection2 = delegate.bind([](int&) {});
  EXPECT_TRUE(delegate);

  delegate.unbindAll();
  EXPECT_FALSE(delegate);
}

TEST_F(DelegateMulti, LambdaWithCapture)
{
  auto [delegate, connection0] = createBound<EmptyPolicy, Delegate::DelegateMulticast<FctSignature>>();
  int capture1 = 10;
  int capture2 = 20;
  auto connection1 = delegate.bind([capture1](int& value) { value += capture1; });
  auto connection2 = delegate.bind([capture2](int& value) { value += capture2; });

  int result = 0;
  delegate.invoke(result);
  EXPECT_EQ(result, 30);
}

} // namespace DelegateTests
