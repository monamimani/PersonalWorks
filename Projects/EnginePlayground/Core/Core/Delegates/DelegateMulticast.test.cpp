
#include "DelegateCommon.test.h"

import DelegateMulticast;

namespace DelegateMulticastTests
{
// The various contexts the class can be used in:
// various function signature.
// LValue and Rvalue.
// Special member function 6 + swap.
// Copy, move, assignment, from empty and from statefull and mix and match Bind functions.
// Bind: free function, functor (const, not const, templated), member fct (const, not const, templated).

// Test UnaryOp
// Construct
// isEmpty
// Broadcast
// unbindAll
// Destroy

// Test BinaryOp (Handles to and from Empty)
// Construct 2 delegate, A and B
// Call bind on A (one for LValue and RValue x Type of fct)
// Move to empty B
// Copy B to A
// Destruct
// <=>
// Swap

// Add delegate while calling the delegate ex:
//  auto handleRAII = delegateList.append([&multicastDelegateList = delegateList, &i]() {
//    if (i == 0)
//    {
//      auto handleRAII = multicastDelegateList.append([&i]() { i++; });
//    }
//    i++;
//  });
//  delegateList();
//  ASSERT_EQ(i, 1);
//
//  delegateList();
//  ASSERT_EQ(i, 3);
//}

// also with threads
// TEST_F(CoreDelegateListMultithreadTest, AppendPrependInDelegate)
//{
//
//  std::mt19937 generator;
//  std::uniform_int_distribution<int> distribution(4, 32);
//  auto threadNumber = distribution(generator);
//
//  std::atomic_uint32_t delegateCount = 0;
//
//  auto appendPrependInDelegateFunctionTest = [&multicastDelegateListA = delegateList, &delegateCount](std::stop_token stopToken) {
//    while (!stopToken.stop_requested())
//    {
//      delegateCount++;
//      auto handleRAII = multicastDelegateListA.append([&multicastDelegateListB = multicastDelegateListA, &delegateCount]() {
//        delegateCount++;
//        auto handleRAII = multicastDelegateListB.append([&multicastDelegateListC = multicastDelegateListB]() {});
//      });
//    }
//  };
//
//  ASSERT_TRUE(delegateList.isEmpty());
//  ASSERT_EQ(getDelegateList().size(), 0);
//
//  std::vector<std::unique_ptr<std::jthread>> threadVector;
//  for (size_t i = 0; i < threadNumber; i++)
//  {
//    threadVector.push_back(std::make_unique<std::jthread>(appendPrependInDelegateFunctionTest));
//  }
//
//  //using namespace std::chrono_literals;
//  //std::this_thread::sleep_for(1ms);
//
//  delegateList.Broadcast();
//  delegateList.Broadcast();
//  delegateList.Broadcast();
//  delegateList.Broadcast();
//  delegateList.Broadcast();
//  delegateList.Broadcast();
//
//
//  for (auto& threadPtr : threadVector)
//  {
//    threadPtr->request_stop();
//    threadPtr->join();
//  }
//
//  ASSERT_FALSE(delegateList.isEmpty());
//  ASSERT_EQ(getDelegateList().size(), delegateCount);
//}

using namespace DelegateLikeTests;
using DelegateMulticast_T = Delegate::DelegateMulticast<FctSignature>;
using DelegateMultiOp1F = OpArity1DelegateLikeTestF<DelegateMulticast_T, DelegateMulticast_T::Connection>;
using DelegateMultiOp2F = OpArity2DelegateLikeTestF<DelegateMulticast_T, DelegateMulticast_T::Connection>;

INSTANTIATE_TEST_SUITE_P(DelegateMultiOp1Arg, DelegateMultiOp1F, OpAr1Arg, DelegateMultiOp1F::makeTestName);
INSTANTIATE_TEST_SUITE_P(DelegateMultiOp2Arg, DelegateMultiOp2F, OpAr2Arg, DelegateMultiOp2F::makeTestName);

#define TESTSUITENAME_OP1 DelegateMultiOp1F
#define TESTSUITENAME_OP2 DelegateMultiOp2F
#include"Core/Delegates/DelegateCommonTestCode.h"

TEST_P(DelegateMultiOp1F, isEmpty)
{
  if (!m_isBindKindEmpty)
  {
    ASSERT_TRUE((bool)m_delegate);
    ASSERT_FALSE(m_delegate.isEmpty());
    ASSERT_TRUE(m_delegate);
  }
  else
  {
    ASSERT_FALSE((bool)m_delegate);
    ASSERT_TRUE(m_delegate.isEmpty());
    ASSERT_FALSE(m_delegate);
  }
}

TEST_P(DelegateMultiOp1F, unbindAll)
{

  if (!m_isBindKindEmpty)
  {
    ASSERT_TRUE(m_delegate);
  }
  else
  {
    ASSERT_FALSE(m_delegate);
  }

  m_delegate.unbindAll();
  ASSERT_TRUE(m_delegate.isEmpty());
}

} // namespace DelegateMulticastTests
