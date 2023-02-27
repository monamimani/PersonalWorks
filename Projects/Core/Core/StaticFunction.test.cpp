#include <algorithm>
#include <array>
#include <optional>
#include <utility>

#include "Delegates/DelegateCommon.test.h"

import StaticFunction;

namespace StaticFunctionTests
{

using namespace DelegateLikeTests;

class StaticFctOp1ArgF: public OpArity1DelegateLikeTestF
{};

class StaticFctOp2ArgF: public OpArity2DelegateLikeTestF
{};

using StaticFctT = StaticFunction<void(int&)>;

INSTANTIATE_TEST_SUITE_P(Op1Arg, StaticFctOp1ArgF, OpAr1Arg, OpArity1DelegateLikeTestF::makeTestName);
INSTANTIATE_TEST_SUITE_P(Op2Arg, StaticFctOp2ArgF, OpAr2Arg, OpArity2DelegateLikeTestF::makeTestName);

TEST_P(StaticFctOp1ArgF, CtorDtor)
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

TEST_P(StaticFctOp1ArgF, CopyCtor)
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
    EXPECT_EQ(value, getExpectedTestValue(isFctConst));

    staticFunctionCopy.invoke(value);
    EXPECT_EQ(value, getExpectedTestValue(isFctConst));
  }
  else
  {
    ASSERT_FALSE((bool)staticFunction);
    ASSERT_FALSE((bool)staticFunctionCopy);
  }
}

TEST_P(StaticFctOp1ArgF, MoveCtor)
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
    EXPECT_EQ(value, getExpectedTestValue(isFctConst));

    staticFunctionMove.invoke(value);
    EXPECT_EQ(value, getExpectedTestValue(isFctConst));
  }
  else
  {
    ASSERT_FALSE((bool)staticFunction); // This realy is a valid unspecified state, becaused moved from
    ASSERT_FALSE((bool)staticFunctionMove);
  }
}

TEST_P(StaticFctOp1ArgF, isBound)
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

TEST_P(StaticFctOp1ArgF, EqualityOp)
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

TEST_P(StaticFctOp1ArgF, reset)
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

TEST_P(StaticFctOp1ArgF, unbind)
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

TEST_P(StaticFctOp1ArgF, Invoke)
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
    EXPECT_EQ(value, getExpectedTestValue(isFctConst));
  }
  else
  {
    ASSERT_FALSE((bool)staticFunction);
  }
}

TEST_P(StaticFctOp1ArgF, functionalOperator)
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
    EXPECT_EQ(value, getExpectedTestValue(isFctConst));
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

TEST_P(StaticFctOp2ArgF, CopyAssign)
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
    EXPECT_EQ(value, getExpectedTestValue(isFctConstA));

    staticFunctionB.invoke(value);
    EXPECT_EQ(value, getExpectedTestValue(isFctConstA));
  }
  else
  {
    ASSERT_FALSE((bool)staticFunctionA);
    ASSERT_FALSE((bool)staticFunctionB);
  }

  ASSERT_EQ(staticFunctionA, staticFunctionB);
}

TEST_P(StaticFctOp2ArgF, MoveAssign)
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
    EXPECT_EQ(value, getExpectedTestValue(isFctConstA));

    staticFunctionB.invoke(value);
    EXPECT_EQ(value, getExpectedTestValue(isFctConstA));
  }
  else
  {
    ASSERT_FALSE((bool)staticFunctionA); // This really is a valid unspecified state, because moved from
    ASSERT_FALSE((bool)staticFunctionB);
    ASSERT_EQ(staticFunctionA, staticFunctionB); // This really is a valid unspecified state, because moved from
  }
}

TEST_P(StaticFctOp2ArgF, Swap)
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
    EXPECT_EQ(value, getExpectedTestValue(isFctConstB));
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
    EXPECT_EQ(value, getExpectedTestValue(isFctConstA));
  }
  else
  {
    ASSERT_FALSE((bool)staticFunctionB);
  }

  ASSERT_EQ(staticFunctionA, staticFunctionBCopy);
  ASSERT_EQ(staticFunctionB, staticFunctionACopy);
}

} // namespace StaticFunctionTest