

#include "DelegateCommon.test.h"

namespace DelegateTests
{
using namespace DelegateLikeTests;

using Delegate_T = Delegate::Delegate<void(int&)>;
using DelegateOp1F= OpArity1DelegateLikeTestF<Delegate_T, Delegate_T::Connection>;
using DelegateOp2F= OpArity2DelegateLikeTestF<Delegate_T, Delegate_T::Connection>;

INSTANTIATE_TEST_SUITE_P(DelegateOp1Arg, DelegateOp1F, OpAr1Arg, DelegateOp1F::makeTestName);
INSTANTIATE_TEST_SUITE_P(DelegateOp2Arg, DelegateOp2F, OpAr2Arg, DelegateOp2F::makeTestName);

TEST_P(DelegateOp1F, CtorDtor)
{
  if (!m_isBindKindEmpty)
  {
    ASSERT_TRUE((bool)m_delegate);
  }
  else
  {
    ASSERT_FALSE((bool)m_delegate);
  }
}

TEST_P(DelegateOp1F, CopyCtor)
{
  Delegate_T delegateCopy{m_delegate};

  if (!m_isBindKindEmpty)
  {
    ASSERT_TRUE((bool)m_delegate);
    ASSERT_TRUE((bool)delegateCopy);

    int value = 28;
    delegateCopy.invoke(value);
    ASSERT_EQ(value, getExpectedTestValue(m_isConst));
  }
  else
  {
    ASSERT_FALSE((bool)m_delegate);
    ASSERT_FALSE((bool)delegateCopy);
  }

  ASSERT_EQ(m_delegate, delegateCopy);
}

TEST_P(DelegateOp1F, MoveCtor)
{
  Delegate_T delegateMove{std::move(m_delegate)};

  if (!m_isBindKindEmpty)
  {
    ASSERT_FALSE((bool)m_delegate); // This really is a valid unspecified state, because moved from
    ASSERT_TRUE((bool)delegateMove);

    int value = 28;
    delegateMove.invoke(value);
    ASSERT_EQ(value, getExpectedTestValue(m_isConst));
  }
  else
  {
    ASSERT_FALSE((bool)m_delegate); // This really is a valid unspecified state, because moved from
    ASSERT_FALSE((bool)delegateMove);
  }
}

TEST_P(DelegateOp1F, isBound)
{
  if (!m_isBindKindEmpty)
  {
    ASSERT_TRUE((bool)m_delegate);
    ASSERT_TRUE(m_delegate.isBound());
    ASSERT_TRUE(m_delegate);
  }
  else
  {
    ASSERT_FALSE((bool)m_delegate);
    ASSERT_FALSE(m_delegate.isBound());
    ASSERT_FALSE(m_delegate);
  }
}

TEST_P(DelegateOp1F, operatorEqual)
{

  Delegate_T delegateCopy = m_delegate;

  if (!m_isBindKindEmpty)
  {
    ASSERT_TRUE(delegateCopy);
  }
  else
  {
    ASSERT_FALSE(delegateCopy);
  }
  ASSERT_EQ(m_delegate, delegateCopy);
}

// TEST_P(DelegateOp1F, reset)
//{
//   auto [isLValue, isFctConst, bindKind] = GetParam();
// auto m_isBindKindEmpty = bindKind == BindKind::Empty;

// TestStruct testStruct;
// Delegate_T m_delegate;

// auto connection = bind(m_delegate, testStruct, bindKind, isLValue, isFctConst);
// SpecialFunctionCallCounter counters;
// TestStruct::resetStaticCounters();
////
//   if (!m_isBindKindEmpty)
//   {
//     ASSERT_TRUE(m_delegate);
//   }
//   else
//   {
//     ASSERT_FALSE(m_delegate);
//   }
//
//   m_delegate.reset();
//   ASSERT_FALSE(m_delegate);
// }

TEST_P(DelegateOp1F, unbind)
{

  if (!m_isBindKindEmpty)
  {
    ASSERT_TRUE(m_delegate);
  }
  else
  {
    ASSERT_FALSE(m_delegate);
  }

  m_delegate.unbind();
  ASSERT_FALSE(m_delegate);
}

TEST_P(DelegateOp1F, invoke)
{

  int value = 28;
  if (!m_isBindKindEmpty)
  {
    m_delegate.invoke(value);
    ASSERT_EQ(value, getExpectedTestValue(m_isConst));
  }
}

TEST_P(DelegateOp1F, functionalOperator)
{

  int value = 28;
  if (!m_isBindKindEmpty)
  {
    m_delegate(value);
    ASSERT_EQ(value, getExpectedTestValue(m_isConst));
  }
}

TEST(DelegateOp1F, invokeSafe)
{

  TestStruct testStruct;
  int value = 28;
  Delegate::Delegate<int(int&)> m_delegate;
  ASSERT_FALSE((bool)m_delegate);
  auto handle = m_delegate.bind<&TestStruct::fctReturn>(testStruct);
  ASSERT_TRUE((bool)m_delegate);
  ASSERT_TRUE(m_delegate.isBound());

  auto result = m_delegate.invokeSafe(value);
  ASSERT_NE(result, std::nullopt);

  ASSERT_EQ(result.value(), TestStruct::m_staticValue);
}

TEST_P(DelegateOp2F, CopyAssign)
{
  m_delegateB = m_delegateA;

  if (!m_isDelegateABindKindEmpty)
  {
    ASSERT_TRUE((bool)m_delegateA);
    ASSERT_TRUE((bool)m_delegateB);

    int value = 28;
    m_delegateA.invoke(value);
    ASSERT_EQ(value, getExpectedTestValue(m_isDelegateAConst));

    m_delegateB.invoke(value);
    ASSERT_EQ(value, getExpectedTestValue(m_isDelegateAConst));
  }
  else
  {
    ASSERT_FALSE((bool)m_delegateA);
    ASSERT_FALSE((bool)m_delegateB);
  }

  ASSERT_EQ(m_delegateA, m_delegateB);
}

TEST_P(DelegateOp2F, MoveAssign)
{
  m_delegateB = std::move(m_delegateA);

  if (!m_isDelegateABindKindEmpty)
  {
    ASSERT_FALSE((bool)m_delegateA); // This realy is a valid unspecified state, becaused moved from
    ASSERT_TRUE((bool)m_delegateB);

    int value = 28;
    m_delegateB.invoke(value);
    ASSERT_EQ(value, getExpectedTestValue(m_isDelegateAConst));
  }
  else
  {
    ASSERT_FALSE((bool)m_delegateA); // This realy is a valid unspecified state, becaused moved from
    ASSERT_FALSE((bool)m_delegateB);
    ASSERT_EQ(m_delegateA, m_delegateB); // This realy is a valid unspecified state, becaused moved from
  }
}

TEST_P(DelegateOp2F, Swap)
{
  Delegate_T delegateACopy = m_delegateA;
  if (!m_isDelegateABindKindEmpty)
  {
    ASSERT_TRUE((bool)delegateACopy);
  }
  else
  {
    ASSERT_FALSE((bool)delegateACopy);
  }

  Delegate_T delegateBCopy = m_delegateB;
  if (!m_isDelegateBBindKindEmpty)
  {
    ASSERT_TRUE((bool)delegateBCopy);
  }
  else
  {
    ASSERT_FALSE((bool)delegateBCopy);
  }

  using std::swap;
  swap(m_delegateA, m_delegateB);

  if (!m_isDelegateBBindKindEmpty)
  {
    ASSERT_TRUE((bool)m_delegateA);

    int value = 28;
    m_delegateA.invoke(value);
    ASSERT_EQ(value, getExpectedTestValue(m_isDelegateBConst));
  }
  else
  {
    ASSERT_FALSE((bool)m_delegateA);
  }

  if (!m_isDelegateABindKindEmpty)
  {
    ASSERT_TRUE((bool)m_delegateB);
    int value = 28;
    m_delegateB.invoke(value);
    ASSERT_EQ(value, getExpectedTestValue(m_isDelegateAConst));
  }
  else
  {
    ASSERT_FALSE((bool)m_delegateB);
  }

  ASSERT_EQ(m_delegateA, delegateBCopy);
  ASSERT_EQ(m_delegateB, delegateACopy);
}
} // namespace DelegateTests
