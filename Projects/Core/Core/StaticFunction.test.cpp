#include <algorithm>
#include <array>
#include <optional>
#include <utility>

#include "fmt/format.h"
#include "TestUtilities/GoogleTest.h"
#include "TestUtilities/TestStruct.test.h"

import StaticFunction;

namespace StaticFunctionTest
{
using namespace Core;
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

static auto bindFreeFunction(auto& delegateLike, [[maybe_unused]] TestStruct&, [[maybe_unused]] IsLValue, [[maybe_unused]] IsFctConst)
{
  return delegateLike.bind<&freeFunction>();
}

static auto bindFunctor(auto& delegateLike, TestStruct& testStruct, IsLValue isLValue, IsFctConst isFctConst)
{
  if (isLValue)
  {
    if (isFctConst)
    {
      return delegateLike.bind(const_cast<const TestStruct&>(testStruct));
    }
    else
    {
      return delegateLike.bind(testStruct);
    }
  }
  else
  {
    if (isFctConst)
    {
      return delegateLike.bind(const_cast<const TestStruct&&>(TestStruct{}));
    }
    else
    {
      return delegateLike.bind(TestStruct{});
    }
  }
}

static auto bindMemberFct(auto& delegateLike, TestStruct& testStruct, IsLValue isLValue, IsFctConst isFctConst)
{
  if (!isLValue)
  {
    if (isFctConst)
    {
      return delegateLike.bind<&TestStruct::fctConst>(const_cast<const TestStruct&&>(TestStruct{}));
    }
    else
    {
      return delegateLike.bind<&TestStruct::fct>(TestStruct{});
    }
  }
  else
  {
    if (isFctConst)
    {
      return delegateLike.bind<&TestStruct::fctConst>(const_cast<const TestStruct&>(testStruct));
    }
    else
    {
      return delegateLike.bind<&TestStruct::fct>(testStruct);
    }
  }
}

template <typename DelegateLikeT>
static auto bindMemberFctTemplate(DelegateLikeT& delegateLike, TestStruct& testStruct, IsLValue isLValue, IsFctConst isFctConst)
{
  if (isLValue)
  {
    if (isFctConst)
    {
      return delegateLike.bind<DelegateLikeT::asFnConstPtr(&TestStruct::fctTemplate<int>)>(testStruct);
    }
    else
    {
      return delegateLike.bind<DelegateLikeT::asFnPtr(&TestStruct::fctTemplate<int>)>(testStruct);
    }
  }
  else
  {
    if (isFctConst)
    {
      return delegateLike.bind<DelegateLikeT::asFnConstPtr(&TestStruct::fctTemplate<int>)>(const_cast<const TestStruct&&>(std::move(testStruct)));
    }
    else
    {
      return delegateLike.bind<DelegateLikeT::asFnPtr(&TestStruct::fctTemplate<int>)>(std::move(testStruct));
    }
  }
}

template <typename DelegateLikeT>
static auto bindMemberFctConstOverloaded(DelegateLikeT& delegateLike, TestStruct& testStruct, IsLValue isLValue, IsFctConst isFctConst)
{
  if (!isLValue)
  {
    if (isFctConst)
    {
      return delegateLike.bind<DelegateLikeT::asFnConstPtr(&TestStruct::fctConstOverloaded)>(const_cast<const TestStruct&&>(std::move(testStruct)));
    }
    else
    {
      return delegateLike.bind<DelegateLikeT::asFnPtr(&TestStruct::fctConstOverloaded)>(std::move(testStruct));
    }
  }
  else
  {
    if (isFctConst)
    {
      return delegateLike.bind<DelegateLikeT::asFnConstPtr(&TestStruct::fctConstOverloaded)>(testStruct);
    }
    else
    {
      return delegateLike.bind<DelegateLikeT::asFnPtr(&TestStruct::fctConstOverloaded)>(testStruct);
    }
  }
}

template <typename DelegateLikeT>
static auto bindMemberFctParamOverloaded(DelegateLikeT& delegateLike, TestStruct& testStruct, IsLValue isLValue, IsFctConst isFctConst)
{
  if (isLValue)
  {
    if (!isFctConst)
    {
      return delegateLike.bind<DelegateLikeT::asFnPtr(&TestStruct::fctParamOverloaded)>(testStruct);
    }
    else
    {
      return delegateLike.bind<DelegateLikeT::asFnConstPtr(&TestStruct::fctParamOverloaded)>(testStruct);
    }
  }
  else
  {
    if (isFctConst)
    {
      return delegateLike.bind<DelegateLikeT::asFnConstPtr(&TestStruct::fctParamOverloaded)>(const_cast<const TestStruct&&>(std::move(testStruct)));
    }
    else
    {
      return delegateLike.bind<DelegateLikeT::asFnPtr(&TestStruct::fctParamOverloaded)>(std::move(testStruct));
    }
  }
}

template <typename DelegateLikeT>
static auto bind(DelegateLikeT& delegateLike, TestStruct& testStruct, BindKind bindKind, IsLValue isLValue, IsFctConst isFctConst)
{
  switch (bindKind)
  {
    case BindKind::Empty:
      if constexpr (requires { typename DelegateLikeT::DelegateRAII; })
      {
        return DelegateLikeT::DelegateRAII();
      }
      else
      {
        return;
      }
    case BindKind::FreeFunction:
      return bindFreeFunction(delegateLike, testStruct, isLValue, isFctConst);
    case BindKind::Functor:
      return bindFunctor(delegateLike, testStruct, isLValue, isFctConst);
    case BindKind::MemberFct:
      return bindMemberFct(delegateLike, testStruct, isLValue, isFctConst);
    case BindKind::MemberFctTemplate:
      return bindMemberFctTemplate(delegateLike, testStruct, isLValue, isFctConst);
    case BindKind::MemberFctConstOverloaded:
      return bindMemberFctConstOverloaded(delegateLike, testStruct, isLValue, isFctConst);
    case BindKind::MemberFctParamOverloaded:
      return bindMemberFctParamOverloaded(delegateLike, testStruct, isLValue, isFctConst);
  }

  std::unreachable();
}

class DelegateLikeTestF
{
public:
  static std::string getTestNamePart(IsLValue isLValue, IsFctConst isFctConst, BindKind bindkind)
  {
    static constexpr auto bindkindTuples = std::to_array<std::tuple<BindKind, const char*>>({
        {                   BindKind::Empty,                     "Empty"},
        {            BindKind::FreeFunction,                   "FreeFct"},
        {                 BindKind::Functor,                   "Functor"},
        {               BindKind::MemberFct,                "BindMemFct"},
        {       BindKind::MemberFctTemplate,        "BindMemFctTemplate"},
        {BindKind::MemberFctConstOverloaded, "BindMemFctConstOverloaded"},
        {BindKind::MemberFctParamOverloaded, "BindMemFctParamOverloaded"}
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

class OpArity1StaticFctTestF: public DelegateLikeTestF, public testing::TestWithParam<ParamSetType>
{
public:
  static constexpr auto makeTestName = [](const testing::TestParamInfo<ParamType>& info)
  {
    auto [isLValueA, isFctConstA, bindKind] = info.param;

    auto delegateTypeAName = getTestNamePart(isLValueA, isFctConstA, bindKind);

    return delegateTypeAName;
  };

protected:
  auto getExpectedTestValue() -> decltype(TestStruct::m_staticValue)
  {
    auto [isLValueA, isFctConstA, bindKind] = GetParam();

    if (isFctConstA)
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

class OpArity2StaticFctTestF: public DelegateLikeTestF, public testing::TestWithParam<std::tuple<ParamSetType, ParamSetType>>
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
  auto getExpectedTestValueA() -> decltype(TestStruct::m_staticValue)
  {
    auto [paramA, paramB] = GetParam();
    auto [isLValueA, isFctConstA, delegateBinderA] = paramA;
    if (isFctConstA)
    {
      return TestStruct::m_staticValueConst;
    }
    else
    {
      return TestStruct::m_staticValue;
    }
  }

  auto getExpectedTestValueB() -> decltype(TestStruct::m_staticValue)
  {
    auto [paramA, paramB] = GetParam();
    auto [isLValueB, isFctConstB, delegateBinderB] = paramB;
    if (isFctConstB)
    {
      return TestStruct::m_staticValueConst;
    }
    else
    {
      return TestStruct::m_staticValue;
    }
  }
};

using StaticFctT = StaticFunction<void(int&)>;

auto OpArity1BindKinds = ::testing::ValuesIn(DelegateLikeTestF::makeDelegateBindKindParamSet());
auto OpArity2BindKinds = ::testing::Combine(OpArity1BindKinds, OpArity1BindKinds);
INSTANTIATE_TEST_SUITE_P(OpArity1StaticFctBindKind, OpArity1StaticFctTestF, OpArity1BindKinds, OpArity1StaticFctTestF::makeTestName);
INSTANTIATE_TEST_SUITE_P(OpArity2StaticFctBindKind, OpArity2StaticFctTestF, OpArity2BindKinds, OpArity2StaticFctTestF::makeTestName);

TEST_P(OpArity1StaticFctTestF, CtorDtor)
{
  auto [isLValue, isFctConst, bindKind] = GetParam();

  TestStruct testStruct;
  StaticFctT staticFunction;
  ASSERT_FALSE((bool)staticFunction);
  bind(staticFunction, testStruct, bindKind, isLValue, isFctConst);

  if (bindKind != BindKind::Empty)
  {
    ASSERT_TRUE((bool)staticFunction);
  }
  else
  {
    ASSERT_FALSE((bool)staticFunction);
  }
}

TEST_P(OpArity1StaticFctTestF, CopyCtor)
{
  auto [isLValue, isFctConst, bindKind] = GetParam();

  TestStruct testStruct;
  StaticFctT staticFunction;
  ASSERT_FALSE((bool)staticFunction);
  bind(staticFunction, testStruct, bindKind, isLValue, isFctConst);

  StaticFctT staticFunctionCopy{staticFunction};

  if (bindKind != BindKind::Empty)
  {
    ASSERT_TRUE((bool)staticFunction);
    ASSERT_TRUE((bool)staticFunctionCopy);

    int value = 28;
    staticFunction.invoke(value);
    EXPECT_EQ(value, getExpectedTestValue());

    staticFunctionCopy.invoke(value);
    EXPECT_EQ(value, getExpectedTestValue());
  }
  else
  {
    ASSERT_FALSE((bool)staticFunction);
    ASSERT_FALSE((bool)staticFunctionCopy);
  }
}

TEST_P(OpArity1StaticFctTestF, MoveCtor)
{
  auto [isLValue, isFctConst, bindKind] = GetParam();

  TestStruct testStruct;
  StaticFctT staticFunction;
  ASSERT_FALSE((bool)staticFunction);
  bind(staticFunction, testStruct, bindKind, isLValue, isFctConst);

  StaticFctT staticFunctionMove{std::move(staticFunction)};

  if (bindKind != BindKind::Empty)
  {
    ASSERT_TRUE((bool)staticFunction); // This realy is a valid unspecified state, becaused moved from
    ASSERT_TRUE((bool)staticFunctionMove);

    int value = 28;
    staticFunction.invoke(value);
    EXPECT_EQ(value, getExpectedTestValue());

    staticFunctionMove.invoke(value);
    EXPECT_EQ(value, getExpectedTestValue());
  }
  else
  {
    ASSERT_FALSE((bool)staticFunction); // This realy is a valid unspecified state, becaused moved from
    ASSERT_FALSE((bool)staticFunctionMove);
  }
}

TEST_P(OpArity1StaticFctTestF, isBound)
{
  auto [isLValue, isFctConst, bindKind] = GetParam();

  TestStruct testStruct;
  StaticFctT staticFunction;
  ASSERT_FALSE((bool)staticFunction);
  bind(staticFunction, testStruct, bindKind, isLValue, isFctConst);

  if (bindKind != BindKind::Empty)
  {
    ASSERT_TRUE((bool)staticFunction);
    ASSERT_TRUE(staticFunction);
    EXPECT_TRUE(staticFunction.isBound());
  }
  else
  {
    ASSERT_FALSE((bool)staticFunction);
    ASSERT_FALSE(staticFunction);
    EXPECT_FALSE(staticFunction.isBound());
  }
}

TEST_P(OpArity1StaticFctTestF, EqualityOp)
{
  auto [isLValue, isFctConst, bindKind] = GetParam();

  TestStruct testStruct;
  StaticFctT staticFunction;
  ASSERT_FALSE((bool)staticFunction);
  bind(staticFunction, testStruct, bindKind, isLValue, isFctConst);
  StaticFctT staticFunctionCopy{staticFunction};

  if (bindKind != BindKind::Empty)
  {
    ASSERT_TRUE(staticFunction);
    ASSERT_TRUE(staticFunctionCopy);
  }
  else
  {
    ASSERT_FALSE(staticFunction);
    ASSERT_FALSE(staticFunctionCopy);
  }
  EXPECT_EQ(staticFunction, staticFunctionCopy);
}

TEST_P(OpArity1StaticFctTestF, reset)
{
  auto [isLValue, isFctConst, bindKind] = GetParam();

  TestStruct testStruct;
  StaticFctT staticFunction;
  ASSERT_FALSE((bool)staticFunction);
  bind(staticFunction, testStruct, bindKind, isLValue, isFctConst);

  if (bindKind != BindKind::Empty)
  {
    ASSERT_TRUE(staticFunction);
  }
  else
  {
    ASSERT_FALSE(staticFunction);
  }

  staticFunction.reset();
  ASSERT_FALSE(staticFunction);
}

TEST_P(OpArity1StaticFctTestF, unbind)
{
  auto [isLValue, isFctConst, bindKind] = GetParam();

  TestStruct testStruct;
  StaticFctT staticFunction;
  ASSERT_FALSE((bool)staticFunction);
  bind(staticFunction, testStruct, bindKind, isLValue, isFctConst);

  if (bindKind != BindKind::Empty)
  {
    ASSERT_TRUE(staticFunction);
  }
  else
  {
    ASSERT_FALSE(staticFunction);
  }

  staticFunction.unbind();
  ASSERT_FALSE(staticFunction);
}

TEST_P(OpArity1StaticFctTestF, Invoke)
{
  auto [isLValue, isFctConst, bindKind] = GetParam();

  TestStruct testStruct;
  StaticFctT staticFunction;
  ASSERT_FALSE((bool)staticFunction);
  bind(staticFunction, testStruct, bindKind, isLValue, isFctConst);

  if (bindKind != BindKind::Empty)
  {
    ASSERT_TRUE((bool)staticFunction);

    int value = 28;
    staticFunction.invoke(value);
    EXPECT_EQ(value, getExpectedTestValue());
  }
  else
  {
    ASSERT_FALSE((bool)staticFunction);
  }
}

TEST_P(OpArity1StaticFctTestF, functionalOperator)
{
  auto [isLValue, isFctConst, bindKind] = GetParam();

  TestStruct testStruct;
  StaticFctT staticFunction;
  ASSERT_FALSE((bool)staticFunction);
  bind(staticFunction, testStruct, bindKind, isLValue, isFctConst);

  if (bindKind != BindKind::Empty)
  {
    ASSERT_TRUE((bool)staticFunction);

    int value = 28;
    staticFunction(value);
    EXPECT_EQ(value, getExpectedTestValue());
  }
}

TEST(StaticFunctionTest, invokeSafe)
{
  int value = 28;
  TestStruct testStruct;
  StaticFunction<int(int&)> staticFunction;
  ASSERT_FALSE((bool)staticFunction);

  auto result = staticFunction.invokeSafe(value);
  EXPECT_EQ(result, std::nullopt);

  staticFunction.bind<&TestStruct::fctReturn>(testStruct);
  ASSERT_TRUE((bool)staticFunction);
  ASSERT_TRUE(staticFunction.isBound());
  result = staticFunction.invokeSafe(value);
  EXPECT_NE(result, std::nullopt);
  EXPECT_EQ(result.value(), TestStruct::m_staticValue);
}

TEST_P(OpArity2StaticFctTestF, CopyAssign)
{

  auto [paramA, paramB] = GetParam();
  auto [isLValueA, isFctConstA, bindKindA] = paramA;
  auto [isLValueB, isFctConstB, bindKindB] = paramB;

  TestStruct testStructA;
  StaticFctT staticFunctionA;
  ASSERT_FALSE((bool)staticFunctionA);
  bind(staticFunctionA, testStructA, bindKindA, isLValueA, isFctConstA);

  TestStruct testStructB;
  StaticFctT staticFunctionB;
  ASSERT_FALSE((bool)staticFunctionB);

  staticFunctionB = staticFunctionA;

  if (bindKindA != BindKind::Empty)
  {
    ASSERT_TRUE((bool)staticFunctionA);
    ASSERT_TRUE((bool)staticFunctionB);

    int value = 28;
    staticFunctionA.invoke(value);
    EXPECT_EQ(value, getExpectedTestValueA());

    staticFunctionB.invoke(value);
    EXPECT_EQ(value, getExpectedTestValueA());
  }
  else
  {
    ASSERT_FALSE((bool)staticFunctionA);
    ASSERT_FALSE((bool)staticFunctionB);
  }

  ASSERT_EQ(staticFunctionA, staticFunctionB);
}

TEST_P(OpArity2StaticFctTestF, MoveAssign)
{
  auto [paramA, paramB] = GetParam();
  auto [isLValueA, isFctConstA, bindKindA] = paramA;
  auto [isLValueB, isFctConstB, bindKindB] = paramB;

  TestStruct testStructA;
  StaticFctT staticFunctionA;
  ASSERT_FALSE((bool)staticFunctionA);
  bind(staticFunctionA, testStructA, bindKindA, isLValueA, isFctConstA);

  TestStruct testStructB;
  StaticFctT staticFunctionB;
  ASSERT_FALSE((bool)staticFunctionB);

  staticFunctionB = std::move(staticFunctionA);

  if (bindKindA != BindKind::Empty)
  {
    ASSERT_TRUE((bool)staticFunctionA); // This really is a valid unspecified state, because moved from
    ASSERT_TRUE((bool)staticFunctionB);

    int value = 28;
    staticFunctionA.invoke(value);
    EXPECT_EQ(value, getExpectedTestValueA());

    staticFunctionB.invoke(value);
    EXPECT_EQ(value, getExpectedTestValueA());
  }
  else
  {
    ASSERT_FALSE((bool)staticFunctionA); // This really is a valid unspecified state, because moved from
    ASSERT_FALSE((bool)staticFunctionB);
    ASSERT_EQ(staticFunctionA, staticFunctionB); // This really is a valid unspecified state, because moved from
  }
}

TEST_P(OpArity2StaticFctTestF, Swap)
{
  auto [paramA, paramB] = GetParam();
  auto [isLValueA, isFctConstA, bindKindA] = paramA;
  auto [isLValueB, isFctConstB, bindKindB] = paramB;

  TestStruct testStructA;
  StaticFctT staticFunctionA;
  ASSERT_FALSE((bool)staticFunctionA);
  bind(staticFunctionA, testStructA, bindKindA, isLValueA, isFctConstA);

  TestStruct testStructB;
  StaticFctT staticFunctionB;
  ASSERT_FALSE((bool)staticFunctionB);
  bind(staticFunctionB, testStructB, bindKindB, isLValueB, isFctConstB);

  StaticFctT staticFunctionACopy = staticFunctionA;
  if (bindKindA != BindKind::Empty)
  {
    ASSERT_TRUE((bool)staticFunctionACopy);
  }
  else
  {
    ASSERT_FALSE((bool)staticFunctionACopy);
  }
  ASSERT_EQ(staticFunctionA, staticFunctionACopy);

  StaticFctT staticFunctionBCopy = staticFunctionB;
  if (bindKindB != BindKind::Empty)
  {
    ASSERT_TRUE((bool)staticFunctionBCopy);
  }
  else
  {
    ASSERT_FALSE((bool)staticFunctionBCopy);
  }
  ASSERT_EQ(staticFunctionB, staticFunctionBCopy);

  using std::swap;
  swap(staticFunctionA, staticFunctionB);

  if (bindKindB != BindKind::Empty)
  {
    ASSERT_TRUE((bool)staticFunctionA);
    int value = 28;
    staticFunctionA.invoke(value);
    EXPECT_EQ(value, getExpectedTestValueB());
  }
  else
  {
    ASSERT_FALSE((bool)staticFunctionA);
  }

  if (bindKindA != BindKind::Empty)
  {
    ASSERT_TRUE((bool)staticFunctionB);
    int value = 28;
    staticFunctionB.invoke(value);
    EXPECT_EQ(value, getExpectedTestValueA());
  }
  else
  {
    ASSERT_FALSE((bool)staticFunctionB);
  }

  ASSERT_EQ(staticFunctionA, staticFunctionBCopy);
  ASSERT_EQ(staticFunctionB, staticFunctionACopy);
}

} // namespace StaticFunctionTest