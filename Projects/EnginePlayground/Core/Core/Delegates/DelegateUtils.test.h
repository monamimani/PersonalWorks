#pragma once

#include "TestUtilities/GoogleTest.h"
#include "TestUtilities/TestStruct.test.h"

import Delegate;
import DelegateMulticast;

namespace DelegateTests
{
using namespace TestUtilities;

/**
 * @brief Helper struct to hold a delegate and its connection handle.
 */
template<typename DelegateT>
struct BoundDelegate
{
  DelegateT delegate;
  typename DelegateT::Connection connection;
};

/**
 * @brief Base fixture for Delegate and MulticastDelegate tests.
 * Handles common setup/teardown for TestStruct and call counters.
 */
class DelegateTest : public ::testing::Test
{
protected:
  TestStruct m_testStruct;

  void SetUp() override
  {
    TestStruct::resetStaticCounters();
  }

  void TearDown() override
  {
    TestStruct::resetStaticCounters();
  }

  /**
   * @brief Helper to get expected value based on whether we bound to a const function.
   */
  static uint8_t getExpectedValue(bool isConst)
  {
    return isConst ? TestStruct::m_staticValueConst : TestStruct::m_staticValue;
  }

  /**
   * @brief Factory to create a delegate and its connection handle for a given policy.
   */
  template<typename PolicyT, typename DelegateT = Delegate::Delegate<FctSignature>>
  auto createBound()
  {
    DelegateT delegate;
    auto connection = PolicyT::bind(delegate, m_testStruct);
    return BoundDelegate<DelegateT>{std::move(delegate), std::move(connection)};
  }
};

/**
 * @brief Policy for binding a free function.
 */
struct FreeFunctionPolicy
{
  static constexpr bool IsConst = false;
  static auto bind(auto& delegate, TestStruct&)
  {
    return delegate.template bind<&freeFunction>();
  }
};

/**
 * @brief Policy for binding a member function.
 */
struct MemberFunctionPolicy
{
  static constexpr bool IsConst = false;
  static auto bind(auto& delegate, TestStruct& ts)
  {
    return delegate.template bind<&TestStruct::fct>(ts);
  }
};

/**
 * @brief Policy for binding a const member function.
 */
struct MemberFunctionConstPolicy
{
  static constexpr bool IsConst = true;
  static auto bind(auto& delegate, TestStruct& ts)
  {
    return delegate.template bind<&TestStruct::fctConst>(ts);
  }
};

/**
 * @brief Policy for binding a functor (L-Value).
 */
struct FunctorLValuePolicy
{
  static constexpr bool IsConst = false;
  static auto bind(auto& delegate, TestStruct& ts)
  {
    return delegate.bind(ts);
  }
};

/**
 * @brief Policy for binding a functor (R-Value).
 */
struct FunctorRValuePolicy
{
  static constexpr bool IsConst = false;
  static auto bind(auto& delegate, TestStruct&)
  {
    return delegate.bind(TestStruct{});
  }
};

/**
 * @brief Policy for binding a template member function.
 */
struct TemplateMemberPolicy
{
  static constexpr bool IsConst = false;
  static auto bind(auto& delegate, TestStruct& ts)
  {
    using DelegateType = std::remove_cvref_t<decltype(delegate)>;
    return delegate.template bind<DelegateType::asFnPtr(&TestStruct::fctTemplate<int>)>(ts);
  }
};

/**
 * @brief Policy for binding an overloaded member function.
 */
struct OverloadPolicy
{
  static constexpr bool IsConst = false;
  static auto bind(auto& delegate, TestStruct& ts)
  {
    return delegate.template bind<static_cast<void (TestStruct::*)(int&)>(&TestStruct::fctParamOverloaded)>(ts);
  }
};

/**
 * @brief Policy for an empty delegate.
 */
struct EmptyPolicy
{
  static constexpr bool IsConst = false;
  static auto bind(auto& delegate, TestStruct&)
  {
    using DelegateT = std::remove_cvref_t<decltype(delegate)>;
    return typename DelegateT::Connection();
  }
};

/**
 * @brief Policy for binding a lambda with capture.
 */
struct LambdaPolicy
{
  static constexpr bool IsConst = false;
  static auto bind(auto& delegate, TestStruct& ts)
  {
    return delegate.bind([&ts](int& value) { ts.fct(value); });
  }
};

/**
 * @brief Standard policies to be used for all delegate-like types.
 */
using DelegatePolicies = ::testing::Types<
  EmptyPolicy,
  FreeFunctionPolicy,
  MemberFunctionPolicy,
  MemberFunctionConstPolicy,
  FunctorLValuePolicy,
  FunctorRValuePolicy,
  TemplateMemberPolicy,
  OverloadPolicy,
  LambdaPolicy
>;

using MulticastDelegatePolicies = DelegatePolicies;

} // namespace DelegateTests
