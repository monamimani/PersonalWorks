#pragma once

#include <algorithm>
#include <array>
// #include <format>
#include <tuple>

#include "fmt/format.h"
#include "TestUtilities/GoogleTest.h"
#include "TestUtilities/TestStruct.test.h"
using namespace TestUtilities;

import StaticFunction;
import Delegate;

namespace Delegate
{
using Delegate_T = Delegate<void(int&)>;

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

//typedef Delegate_T::Connection Createdelegate(Delegate_T&, TestStruct&, IsLValue, IsFctConst);

//template<class DelegateT>
//concept DelegateLike = requires(DelegateT delegate) {
//  {delegate.bind<&freeFunction>() };
//  {TestStruct testStruct; delegate.bind(TestStruct)};
//  {delegate.bind(std::declval<const TestStruct&>())};
//  {delegate.bind(std::declval<TestStruct&&>())};
//  {delegate.bind(std::declval<const TestStruct&&>())};
//};

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

static auto bindMemberFctTemplate(Delegate_T& delegate, TestStruct& testStruct, IsLValue isLValue, IsFctConst isFctConst)
{
  if (isLValue)
  {
    if (isFctConst)
    {
      return delegate.bind<Delegate_T::asFnConstPtr(&TestStruct::fctTemplate<int>)>(testStruct);
    }
    else
    {
      return delegate.bind<Delegate_T::asFnPtr(&TestStruct::fctTemplate<int>)>(testStruct);
    }
  }
  else
  {
    if (isFctConst)
    {
      return delegate.bind<Delegate_T::asFnConstPtr(&TestStruct::fctTemplate<int>)>(const_cast<const TestStruct&&>(std::move(testStruct)));
    }
    else
    {
      return delegate.bind<Delegate_T::asFnPtr(&TestStruct::fctTemplate<int>)>(std::move(testStruct));
    }
  }
}

static auto bindMemberFctConstOverloaded(Delegate_T& delegate, TestStruct& testStruct, IsLValue isLValue, IsFctConst isFctConst)
{
  if (!isLValue)
  {
    if (isFctConst)
    {
      return delegate.bind<Delegate_T::asFnConstPtr(&TestStruct::fctConstOverloaded)>(const_cast<const TestStruct&&>(std::move(testStruct)));
    }
    else
    {
      return delegate.bind<Delegate_T::asFnPtr(&TestStruct::fctConstOverloaded)>(std::move(testStruct));
    }
  }
  else
  {
    if (isFctConst)
    {
      return delegate.bind<Delegate_T::asFnConstPtr(&TestStruct::fctConstOverloaded)>(testStruct);
    }
    else
    {
      return delegate.bind<Delegate_T::asFnPtr(&TestStruct::fctConstOverloaded)>(testStruct);
    }
  }
}

static auto bindMemberFctParamOverloaded(Delegate_T& delegate, TestStruct& testStruct, IsLValue isLValue, IsFctConst isFctConst)
{
  if (isLValue)
  {
    if (!isFctConst)
    {
      return delegate.bind<Delegate_T::asFnPtr(&TestStruct::fctParamOverloaded)>(testStruct);
    }
    else
    {
      return delegate.bind<Delegate_T::asFnConstPtr(&TestStruct::fctParamOverloaded)>(testStruct);
    }
  }
  else
  {
    if (isFctConst)
    {
      return delegate.bind<Delegate_T::asFnConstPtr(&TestStruct::fctParamOverloaded)>(const_cast<const TestStruct&&>(std::move(testStruct)));
    }
    else
    {
      return delegate.bind<Delegate_T::asFnPtr(&TestStruct::fctParamOverloaded)>(std::move(testStruct));
    }
  }
}

static auto bindDelegate(Delegate_T& delegate, TestStruct& testStruct, BindKind bindKind, IsLValue isLValue, IsFctConst isFctConst)
{
  switch (bindKind)
  {
    case BindKind::Empty:
      return Delegate_T::Connection{};
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

class DelegateTestF
{
public:
  static std::string getTestNamePart(IsLValue isLValue, IsFctConst isFctConst, BindKind bindkind)
  {
    static constexpr auto bindkindTuples = std::to_array<std::tuple<BindKind, const char*>>({
        {BindKind::Empty, "Empty"},
        {BindKind::FreeFunction, "FreeFct"},
        {BindKind::Functor, "Functor"},
        {BindKind::MemberFct, "BindMemFct"},
        {BindKind::MemberFctTemplate, "BindMemFctTemplate"},
        {BindKind::MemberFctConstOverloaded, "BindObjectMemFctConstOverloaded"},
        {BindKind::MemberFctParamOverloaded, "BindObjectMemFctParamOverloaded"}
    });

    auto isRequestedBindKind = [bindkind](const std::tuple<BindKind, const char*>& element)
    {
      return bindkind == std::get<0>(element);
    };

    auto result = std::find_if(bindkindTuples.begin(), bindkindTuples.end(), isRequestedBindKind);
    auto isLValueStr = isLValue ? "LValue" : "RValue";
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
    std::ranges::for_each(memberFctBindKind,
                          [&paramSetPart](BindKind bindKind)
                          {
                            paramSetPart.emplace_back(IsLValue(true), IsFctConst(true), bindKind);
                            paramSetPart.emplace_back(IsLValue(false), IsFctConst(false), bindKind);
                            paramSetPart.emplace_back(IsLValue(true), IsFctConst(false), bindKind);
                            paramSetPart.emplace_back(IsLValue(false), IsFctConst(true), bindKind);
                          });

    paramSetPart.emplace_back(IsLValue(true), IsFctConst(false), BindKind::FreeFunction);
    paramSetPart.emplace_back(IsLValue(true), IsFctConst(false), BindKind::Empty);

    return paramSetPart;
  }
};

class UnaryDelegateTestF: public DelegateTestF, public testing::TestWithParam<ParamSetType>
{
public:
  static constexpr auto makeTestName = [](const testing::TestParamInfo<ParamType>& info)
  {
    auto [isLValueA, isFctConstA, bindKind] = info.param;

    std::string name;

    auto delegateTypeAName = getTestNamePart(isLValueA, isFctConstA, bindKind);

    return delegateTypeAName;
  };

protected:
  TestStruct m_testStruct;
  bool m_isConst = false;
  bool m_isBindKindEmpty = false;
  Delegate_T m_delegate;
  Delegate_T::Connection m_handle;
  SpecialFunctionCallCounter m_counters;

  // static void SetUpTestSuite() {
  // }

  // static void TearDownTestSuite() {
  // }

  void SetUp() override
  {
    auto [isLValue, isFctConst, bindKind] = GetParam();
    m_isConst = isFctConst;
    m_isBindKindEmpty = bindKind == BindKind::Empty;

    m_handle = bindDelegate(m_delegate, m_testStruct, bindKind, isLValue, isFctConst);

    TestStruct::resetStaticCounters();
  }

  void TearDown() override
  {
    m_delegate.unbind();
    TestStruct::resetStaticCounters();
  }

  auto getTestValue() -> decltype(TestStruct::m_staticValue)
  {
    if (m_isConst)
    {
      return TestStruct::m_staticValueConst;
    }
    else
    {
      return TestStruct::m_staticValue;
    }
  }

private:
};

class BinaryDelegateTestF: public DelegateTestF, public testing::TestWithParam<std::tuple<ParamSetType, ParamSetType>>
{

public:
  static constexpr auto makeTestName = [](const testing::TestParamInfo<ParamType>& info)
  {
    auto [paramA, paramB] = info.param;
    auto [isLValueA, isFctConstA, delegateBinderA] = paramA;
    auto [isLValueB, isFctConstB, delegateBinderB] = paramB;

    auto delegateTypeAName = getTestNamePart(isLValueA, isFctConstA, delegateBinderA);
    auto delegateTypeBName = getTestNamePart(isLValueB, isFctConstB, delegateBinderB);

    return fmt::format("{}_{}", delegateTypeAName, delegateTypeBName);
  };

protected:
  TestStruct m_testStruct;
  Delegate_T m_delegateA;
  Delegate_T::Connection m_handleA;
  bool m_isDelegateAConst = false;
  bool m_isDelegateABindKindEmpty = false;

  Delegate_T m_delegateB;
  Delegate_T::Connection m_handleB;
  bool m_isDelegateBConst = false;
  bool m_isDelegateBBindKindEmpty = false;

  SpecialFunctionCallCounter m_counters;

  void SetUp() override
  {
    auto [paramA, paramB] = GetParam();

    auto [isLValueA, isDelegateAConst, bindKindA] = paramA;
    auto [isLValueB, isDelegateBConst, bindKindB] = paramB;

    m_isDelegateAConst = isDelegateAConst;
    m_isDelegateBConst = isDelegateBConst;
    m_isDelegateABindKindEmpty = bindKindA == BindKind::Empty;
    m_isDelegateBBindKindEmpty = bindKindB == BindKind::Empty;

    m_handleA = bindDelegate(m_delegateA, m_testStruct, bindKindA, isLValueA, m_isDelegateAConst);
    m_handleB = bindDelegate(m_delegateB, m_testStruct, bindKindB, isLValueB, m_isDelegateBConst);

    TestStruct::resetStaticCounters();
  }

  void TearDown() override
  {
    TestStruct::resetStaticCounters();
  }

  auto getTestValueA() -> decltype(TestStruct::m_staticValue)
  {
    if (m_isDelegateAConst)
    {
      return TestStruct::m_staticValueConst;
    }
    else
    {
      return TestStruct::m_staticValue;
    }
  }

  auto getTestValueB() -> decltype(TestStruct::m_staticValue)
  {
    if (m_isDelegateBConst)
    {
      return TestStruct::m_staticValueConst;
    }
    else
    {
      return TestStruct::m_staticValue;
    }
  }
};

} // namespace Delegate