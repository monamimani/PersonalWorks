#pragma once

#include <algorithm>
#include <array>
// #include <format>

#include "fmt/format.h"
#include "TestUtilities/GoogleTest.h"
#include "TestUtilities/TestStruct.test.h"
using namespace TestUtilities;

import StaticFunction;
import Delegate;
import DelegateMulticast;

namespace DelegateLikeTests
{
using namespace TestUtilities;

using IsLValue = bool;
using IsFctConst = bool;

enum class BindKind
{
  Empty,
  FreeFunction,
  Functor,
  MemberFct,
  MemberFctTemplate,
  MemberFctConstOverloaded,
  MemberFctParamOverloaded
};

using ParamSetType = std::tuple<IsLValue, IsFctConst, BindKind>;

// typedef Delegate_T::Connection Createdelegate(Delegate_T&, TestStruct&, IsLValue, IsFctConst);

// template<class DelegateT>
// concept DelegateLike = requires(DelegateT delegate) {
//   {delegate.bind<&freeFunction>() };
//   {TestStruct testStruct; delegate.bind(TestStruct)};
//   {delegate.bind(std::declval<const TestStruct&>())};
//   {delegate.bind(std::declval<TestStruct&&>())};
//   {delegate.bind(std::declval<const TestStruct&&>())};
// };

static auto bindFreeFunction(auto& delegate, [[maybe_unused]] TestStruct&, [[maybe_unused]] IsLValue, [[maybe_unused]] IsFctConst)
{
  return delegate.bind<&freeFunction>();
}

static auto bindFunctor(auto& delegate, TestStruct& testStruct, IsLValue isLValue, IsFctConst isFctConst)
{
  if (isLValue)
  {
    if (isFctConst)
    {
      return delegate.bind(const_cast<const TestStruct&>(testStruct));
    }
    else
    {
      return delegate.bind(testStruct);
    }
  }
  else
  {
    if (isFctConst)
    {
      return delegate.bind(const_cast<const TestStruct&&>(TestStruct{}));
    }
    else
    {
      return delegate.bind(TestStruct{});
    }
  }
}

static auto bindMemberFct(auto& delegate, TestStruct& testStruct, IsLValue isLValue, IsFctConst isFctConst)
{
  if (!isLValue)
  {
    if (isFctConst)
    {
      return delegate.bind<&TestStruct::fctConst>(const_cast<const TestStruct&&>(TestStruct{}));
    }
    else
    {
      return delegate.bind<&TestStruct::fct>(TestStruct{});
    }
  }
  else
  {
    if (isFctConst)
    {
      return delegate.bind<&TestStruct::fctConst>(const_cast<const TestStruct&>(testStruct));
    }
    else
    {
      return delegate.bind<&TestStruct::fct>(testStruct);
    }
  }
}

template<typename DelegateLikeT>
static auto bindMemberFctTemplate(DelegateLikeT& delegate, TestStruct& testStruct, IsLValue isLValue, IsFctConst isFctConst)
{
  if (isLValue)
  {
    if (isFctConst)
    {
      return delegate.bind<DelegateLikeT::asFnConstPtr(&TestStruct::fctTemplate<int>)>(testStruct);
    }
    else
    {
      return delegate.bind<DelegateLikeT::asFnPtr(&TestStruct::fctTemplate<int>)>(testStruct);
    }
  }
  else
  {
    if (isFctConst)
    {
      return delegate.bind<DelegateLikeT::asFnConstPtr(&TestStruct::fctTemplate<int>)>(const_cast<const TestStruct&&>(std::move(testStruct)));
    }
    else
    {
      return delegate.bind<DelegateLikeT::asFnPtr(&TestStruct::fctTemplate<int>)>(std::move(testStruct));
    }
  }
}

template<typename DelegateLikeT>
static auto bindMemberFctConstOverloaded(DelegateLikeT& delegate, TestStruct& testStruct, IsLValue isLValue, IsFctConst isFctConst)
{
  if (!isLValue)
  {
    if (isFctConst)
    {
      return delegate.bind<DelegateLikeT::asFnConstPtr(&TestStruct::fctConstOverloaded)>(const_cast<const TestStruct&&>(std::move(testStruct)));
    }
    else
    {
      return delegate.bind<DelegateLikeT::asFnPtr(&TestStruct::fctConstOverloaded)>(std::move(testStruct));
    }
  }
  else
  {
    if (isFctConst)
    {
      return delegate.bind<DelegateLikeT::asFnConstPtr(&TestStruct::fctConstOverloaded)>(testStruct);
    }
    else
    {
      return delegate.bind<DelegateLikeT::asFnPtr(&TestStruct::fctConstOverloaded)>(testStruct);
    }
  }
}

template<typename DelegateLikeT>
static auto bindMemberFctParamOverloaded(DelegateLikeT& delegate, TestStruct& testStruct, IsLValue isLValue, IsFctConst isFctConst)
{
  if (isLValue)
  {
    if (!isFctConst)
    {
      return delegate.bind<DelegateLikeT::asFnPtr(&TestStruct::fctParamOverloaded)>(testStruct);
    }
    else
    {
      return delegate.bind<DelegateLikeT::asFnConstPtr(&TestStruct::fctParamOverloaded)>(testStruct);
    }
  }
  else
  {
    if (isFctConst)
    {
      return delegate.bind<DelegateLikeT::asFnConstPtr(&TestStruct::fctParamOverloaded)>(const_cast<const TestStruct&&>(std::move(testStruct)));
    }
    else
    {
      return delegate.bind<DelegateLikeT::asFnPtr(&TestStruct::fctParamOverloaded)>(std::move(testStruct));
    }
  }
}

template<typename DelegateLikeT>
static auto bind(DelegateLikeT& delegate, TestStruct& testStruct, BindKind bindKind, IsLValue isLValue, IsFctConst isFctConst)
{
  switch (bindKind)
  {
    case BindKind::Empty:
      if constexpr (requires { typename DelegateLikeT::Connection; })
      {
        return DelegateLikeT::Connection();
      }
      else
      {
        return;
      }
    case BindKind::FreeFunction:
      return bindFreeFunction(delegate, testStruct, isLValue, isFctConst);
    case BindKind::Functor:
      return bindFunctor(delegate, testStruct, isLValue, isFctConst);
    case BindKind::MemberFct:
      return bindMemberFct(delegate, testStruct, isLValue, isFctConst);
    case BindKind::MemberFctTemplate:
      return bindMemberFctTemplate(delegate, testStruct, isLValue, isFctConst);
    case BindKind::MemberFctConstOverloaded:
      return bindMemberFctConstOverloaded(delegate, testStruct, isLValue, isFctConst);
    case BindKind::MemberFctParamOverloaded:
      return bindMemberFctParamOverloaded(delegate, testStruct, isLValue, isFctConst);
  }

  std::unreachable();
}

class DelegateLikeTestF
{
public:
  static std::string getTestNamePart(IsLValue isLValue, IsFctConst isFctConst, BindKind bindkind)
  {
    static constexpr auto bindkindTuples = std::to_array<std::tuple<BindKind, const char*>>({
        {                   BindKind::Empty,                 "Empty"},
        {            BindKind::FreeFunction,               "FreeFct"},
        {                 BindKind::Functor,               "Functor"},
        {               BindKind::MemberFct,                "MemFct"},
        {       BindKind::MemberFctTemplate,               "MemFctT"},
        {BindKind::MemberFctConstOverloaded, "MemFctConstOverloaded"},
        {BindKind::MemberFctParamOverloaded,   "MemFctArgOverloaded"}
    });

    auto isRequestedBindKind = [bindkind](const std::tuple<BindKind, const char*>& element) {
      return bindkind == std::get<0>(element);
    };

    auto result = std::find_if(bindkindTuples.begin(), bindkindTuples.end(), isRequestedBindKind);
    auto isLValueStr = isLValue ? "LVal" : "RVal";
    auto isConstStr = isFctConst ? "Const" : "";
    auto bindkindStr = (result != bindkindTuples.end()) ? std::get<1>(*result) : "Unknown";
    return fmt::format("{}{}{}", isLValueStr, bindkindStr, isConstStr);
  }

  static auto makeDelegateBindKindParamSet()
  {
    static constexpr std::array fctConstness = {true, false};
    static constexpr std::array valueKinds = {true, false};
    static constexpr std::array memberFctBindKind = {BindKind::Functor, BindKind::MemberFct, BindKind::MemberFctTemplate, BindKind::MemberFctConstOverloaded, BindKind::MemberFctParamOverloaded};
    static constexpr auto memFctParamSetPartSize = valueKinds.size() * fctConstness.size() * memberFctBindKind.size();

    auto paramSetPart = std::vector<ParamSetType>();
    paramSetPart.reserve(memFctParamSetPartSize);
    std::ranges::for_each(memberFctBindKind, [&paramSetPart](BindKind bindKind) {
      paramSetPart.emplace_back(IsLValue(true), IsFctConst(true), bindKind);
      paramSetPart.emplace_back(IsLValue(false), IsFctConst(false), bindKind);
      paramSetPart.emplace_back(IsLValue(true), IsFctConst(false), bindKind);
      paramSetPart.emplace_back(IsLValue(false), IsFctConst(true), bindKind);
    });

    paramSetPart.emplace_back(IsLValue(true), IsFctConst(false), BindKind::FreeFunction);
    paramSetPart.emplace_back(IsLValue(true), IsFctConst(false), BindKind::Empty);

    return paramSetPart;
  }

protected:
  auto getExpectedTestValue(bool isConst) -> decltype(TestStruct::m_staticValue)
  {
    if (isConst)
    {
      return TestStruct::m_staticValueConst;
    }
    else
    {
      return TestStruct::m_staticValue;
    }
  }
};

static const auto OpAr1Arg = ::testing::ValuesIn(DelegateLikeTestF::makeDelegateBindKindParamSet());
static const auto OpAr2Arg = ::testing::Combine(OpAr1Arg, OpAr1Arg);

template<typename DelegateLikeT, typename HandleT>
class OpArity1DelegateLikeTestF: public DelegateLikeTestF, public testing::TestWithParam<ParamSetType>
{

public:
  static constexpr auto makeTestName = [](const testing::TestParamInfo<ParamType>& info) {
    auto [isLValueA, isFctConstA, bindKind] = info.param;

    std::string name;

    auto delegateTypeAName = getTestNamePart(isLValueA, isFctConstA, bindKind);

    return delegateTypeAName;
  };

protected:
  using DelegateLike = DelegateLikeT;
  bool m_isConst = false;
  bool m_isBindKindEmpty = false;

  TestStruct m_testStruct;
  SpecialFunctionCallCounter m_counters;

  DelegateLikeT m_delegate;
  HandleT m_handle;

private:
  void SetUp() override
  {
    auto [isLValue, isFctConst, bindKind] = GetParam();
    m_isConst = isFctConst;
    m_isBindKindEmpty = bindKind == BindKind::Empty;

    if constexpr (std::is_void_v<decltype(bind(m_delegate, m_testStruct, bindKind, isLValue, m_isConst))>)
    {
      bind(m_delegate, m_testStruct, bindKind, isLValue, isFctConst);
    }
    else
    {
      m_handle = bind(m_delegate, m_testStruct, bindKind, isLValue, isFctConst);
    }

    TestStruct::resetStaticCounters();
  }

  void TearDown() override
  {
    if constexpr (requires { m_delegate.unbind(); })
    {
      m_delegate.unbind();
    }
    else
    {
      m_delegate.unbindAll();
    }

    TestStruct::resetStaticCounters();
  }
};

template<typename DelegateLikeT, typename HandleT>
class OpArity2DelegateLikeTestF: public DelegateLikeTestF, public testing::TestWithParam<std::tuple<ParamSetType, ParamSetType>>
{
public:
  static constexpr auto makeTestName = [](const testing::TestParamInfo<ParamType>& info) {
    auto [paramA, paramB] = info.param;
    auto [isLValueA, isFctConstA, delegateBinderA] = paramA;
    auto [isLValueB, isFctConstB, delegateBinderB] = paramB;

    auto delegateTypeAName = getTestNamePart(isLValueA, isFctConstA, delegateBinderA);
    auto delegateTypeBName = getTestNamePart(isLValueB, isFctConstB, delegateBinderB);

    return fmt::format("{}_{}", delegateTypeAName, delegateTypeBName);
  };

protected:
  using DelegateLike = DelegateLikeT;

  bool m_isDelegateAConst = false;
  bool m_isDelegateABindKindEmpty = false;

  TestStruct m_testStruct;
  SpecialFunctionCallCounter m_counters;

  DelegateLikeT m_delegateA;
  HandleT m_handleA;

  bool m_isDelegateBConst = false;
  bool m_isDelegateBBindKindEmpty = false;

  DelegateLikeT m_delegateB;
  HandleT m_handleB;

  void SetUp() override
  {
    auto [paramA, paramB] = GetParam();

    auto [isLValueA, isDelegateAConst, bindKindA] = paramA;
    auto [isLValueB, isDelegateBConst, bindKindB] = paramB;

    m_isDelegateAConst = isDelegateAConst;
    m_isDelegateBConst = isDelegateBConst;
    m_isDelegateABindKindEmpty = bindKindA == BindKind::Empty;
    m_isDelegateBBindKindEmpty = bindKindB == BindKind::Empty;

    if constexpr (std::is_void_v<decltype(bind(m_delegateA, m_testStruct, bindKindA, isLValueA, m_isDelegateAConst))>)
    {
      bind(m_delegateA, m_testStruct, bindKindA, isLValueA, m_isDelegateAConst);
      bind(m_delegateB, m_testStruct, bindKindB, isLValueB, m_isDelegateBConst);
    }
    else
    {
      m_handleA = bind(m_delegateA, m_testStruct, bindKindA, isLValueA, m_isDelegateAConst);
      m_handleB = bind(m_delegateB, m_testStruct, bindKindB, isLValueB, m_isDelegateBConst);
    }

    TestStruct::resetStaticCounters();
  }

  void TearDown() override
  {
    TestStruct::resetStaticCounters();
  }
};

} // namespace DelegateLikeTests
