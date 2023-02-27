

#include "DelegateCommon.test.h"

namespace DelegateTests
{
using namespace DelegateLikeTests;

using Delegate_T = Delegate::Delegate<FctSignature>;
using DelegateOp1F = OpArity1DelegateLikeTestF<Delegate_T, Delegate_T::Connection>;
using DelegateOp2F = OpArity2DelegateLikeTestF<Delegate_T, Delegate_T::Connection>;

INSTANTIATE_TEST_SUITE_P(DelegateOp1Arg, DelegateOp1F, OpAr1Arg, DelegateOp1F::makeTestName);
INSTANTIATE_TEST_SUITE_P(DelegateOp2Arg, DelegateOp2F, OpAr2Arg, DelegateOp2F::makeTestName);

#define TESTSUITENAME_OP1 DelegateOp1F
#define TESTSUITENAME_OP2 DelegateOp2F
#include "Core/Delegates/DelegateCommonTestCode.h"

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

//TEST_P(DelegateOp1F, invokeSafe)
//{
//  int value = 28;
//  if (!m_isBindKindEmpty)
//  {
//    auto result = m_delegate.invokeSafe(value);
//    ASSERT_NE(value, getExpectedTestValue(m_isConst));
//  }
//  else
//  {
//    auto result = m_delegate.invokeSafe(value);
//    ASSERT_EQ(result, std::nullopt);
//  }
//}

} // namespace DelegateTests
