
#include <optional>

#include "DelegateCommon.test.h"

namespace Delegate
{

auto unaryDelegateBindKinds = ::testing::ValuesIn(DelegateTestF::makeDelegateBindKindParamSet());
auto binaryDelegateBindKinds = ::testing::Combine(unaryDelegateBindKinds, unaryDelegateBindKinds);
INSTANTIATE_TEST_SUITE_P(DelegateBindKind, UnaryDelegateTestF, unaryDelegateBindKinds, UnaryDelegateTestF::makeTestName);
INSTANTIATE_TEST_SUITE_P(DualDelegateBindKind, BinaryDelegateTestF, binaryDelegateBindKinds, BinaryDelegateTestF::makeTestName);

TEST_P(UnaryDelegateTestF, CtorDtor)
{
  if (!m_isBindKindEmpty)
  {
    ASSERT_TRUE((bool)m_delegate);
  }
  else
  {
    ASSERT_FALSE((bool)m_delegate);
  }

   std::destroy_at(&m_delegate);
  ASSERT_FALSE((bool)m_delegate);
}

TEST_P(UnaryDelegateTestF, CopyCtor)
{
  Delegate_T delegateCopy{m_delegate};

  if (!m_isBindKindEmpty)
  {
    ASSERT_TRUE((bool)m_delegate);
    ASSERT_TRUE((bool)delegateCopy);

    int value = 28;
    delegateCopy.invoke(value);
    ASSERT_EQ(value, getTestValue());
  }
  else
  {
    ASSERT_FALSE((bool)m_delegate);
    ASSERT_FALSE((bool)delegateCopy);
  }

  ASSERT_EQ(m_delegate, delegateCopy);
}

TEST_P(UnaryDelegateTestF, MoveCtor)
{
  Delegate_T delegateMove{std::move(m_delegate)};

  if (!m_isBindKindEmpty)
  {
    ASSERT_TRUE((bool)m_delegate); // This realy is a valid unspecified state, becaused moved from
    ASSERT_TRUE((bool)delegateMove);

    int value = 28;
    delegateMove.invoke(value);
    ASSERT_EQ(value, getTestValue());
  }
  else
  {
    ASSERT_FALSE((bool)m_delegate); // This realy is a valid unspecified state, becaused moved from
    ASSERT_FALSE((bool)delegateMove);
  }
}

TEST_P(UnaryDelegateTestF, isBound)
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

TEST_P(UnaryDelegateTestF, operatorEqual)
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

//TEST_P(UnaryDelegateTestF, reset)
//{
//  if (!m_isBindKindEmpty)
//  {
//    ASSERT_TRUE(m_delegate);
//  }
//  else
//  {
//    ASSERT_FALSE(m_delegate);
//  }
//
//  m_delegate.reset();
//  ASSERT_FALSE(m_delegate);
//}

TEST_P(UnaryDelegateTestF, unbind)
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

TEST_P(UnaryDelegateTestF, invoke)
{
  int value = 28;
  if (!m_isBindKindEmpty)
  {
    m_delegate.invoke(value);
    ASSERT_EQ(value, getTestValue());
  }
}

TEST_P(UnaryDelegateTestF, functionalOperator)
{
  int value = 28;
  if (!m_isBindKindEmpty)
  {
    m_delegate(value);
    ASSERT_EQ(value, getTestValue());
  }
}

TEST(DelegateTest, invokeSafe)
{
  TestStruct testStruct;
  int value = 28;
  Delegate<int(int&)> delegate;
  ASSERT_FALSE((bool)delegate);
  auto handle = delegate.bind<&TestStruct::fctReturn>(testStruct);
  ASSERT_TRUE((bool)delegate);
  ASSERT_TRUE(delegate.isBound());

  auto result = delegate.invokeSafe(value);
  ASSERT_NE(result, std::nullopt);

  ASSERT_EQ(result.value(), TestStruct::m_staticValue);
}

TEST_P(BinaryDelegateTestF, CopyAssign)
{
  m_delegateB = m_delegateA;

  if (!m_isDelegateABindKindEmpty)
  {
    ASSERT_TRUE((bool)m_delegateA);
    ASSERT_TRUE((bool)m_delegateB);

    int value = 28;
    m_delegateA.invoke(value);
    ASSERT_EQ(value, getTestValueA());

    m_delegateB.invoke(value);
    ASSERT_EQ(value, getTestValueA());
  }
  else
  {
    ASSERT_FALSE((bool)m_delegateA);
    ASSERT_FALSE((bool)m_delegateB);
  }

  ASSERT_EQ(m_delegateA, m_delegateB);
}

TEST_P(BinaryDelegateTestF, MoveAssign)
{
  m_delegateB = std::move(m_delegateA);

  if (!m_isDelegateABindKindEmpty)
  {
    ASSERT_TRUE((bool)m_delegateA); // This realy is a valid unspecified state, becaused moved from
    ASSERT_TRUE((bool)m_delegateB);

    int value = 28;
    m_delegateA.invoke(value);
    ASSERT_EQ(value, getTestValueA());

    m_delegateB.invoke(value);
    ASSERT_EQ(value, getTestValueA());
  }
  else
  {
    ASSERT_FALSE((bool)m_delegateA); // This realy is a valid unspecified state, becaused moved from
    ASSERT_FALSE((bool)m_delegateB);
    ASSERT_EQ(m_delegateA, m_delegateB); // This realy is a valid unspecified state, becaused moved from
  }
}

TEST_P(BinaryDelegateTestF, Swap)
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
    ASSERT_EQ(value, getTestValueB());
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
    ASSERT_EQ(value, getTestValueA());
  }
  else
  {
    ASSERT_FALSE((bool)m_delegateB);
  }

  ASSERT_EQ(m_delegateA, delegateBCopy);
  ASSERT_EQ(m_delegateB, delegateACopy);
}
} // namespace Delegate