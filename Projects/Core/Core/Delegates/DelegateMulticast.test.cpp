
#include <codeanalysis\warnings.h>

#include <algorithm>
#include <array>
#include <format>
#include <ranges>
#include <tuple>
#include <type_traits>

import DelegateMulticast;

#include "Core/GoogleTest.h"

#include "DelegateMulticastCommon.test.h"

using namespace Test;

namespace Delegate
{
using DelegateMulticast_T = DelegateMulticast<void(int&)>;

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
//TEST_F(CoreDelegateListMultithreadTest, AppendPrependInDelegate)
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

using IsLValue = bool;
using IsFctConst = bool;

typedef DelegateMulticast_T::Delegate_T::DelegateRAII CreateDelegateMulticast(DelegateMulticast_T&, TestStruct&, IsLValue, IsFctConst);

auto bindEmpty([[maybe_unused]] DelegateMulticast_T&, [[maybe_unused]] TestStruct&, [[maybe_unused]] IsLValue, [[maybe_unused]] IsFctConst) {}

auto bindFreeFunction(DelegateMulticast_T& delegateMulticast, [[maybe_unused]] TestStruct&, [[maybe_unused]] IsLValue, [[maybe_unused]] IsFctConst)
{
  return delegateMulticast.bind<&freeFunction>();
}

auto bindFunctor(DelegateMulticast_T& delegateMulticast, TestStruct& testStruct, IsLValue isLValue, IsFctConst isFctConst)
{
  if (isLValue)
  {
    if (isFctConst)
    {
      return delegateMulticast.bind(const_cast<const TestStruct&>(testStruct));
    }
    else
    {
      return delegateMulticast.bind(testStruct);
    }
  }
  else
  {
    if (isFctConst)
    {
      return delegateMulticast.bind(const_cast<const TestStruct&&>(std::move(testStruct)));
    }
    else
    {
      return delegateMulticast.bind(std::move(testStruct));
    }
  }
}

auto bindMemberFct(DelegateMulticast_T& delegateMulticast, TestStruct& testStruct, IsLValue isLValue, IsFctConst isFctConst)
{
  if (!isLValue)
  {
    if (isFctConst)
    {
      return delegateMulticast.bind<&TestStruct::fctConst>(const_cast<const TestStruct&&>(std::move(testStruct)));
    }
    else
    {
      return delegateMulticast.bind<&TestStruct::fct>(std::move(testStruct));
    }
  }
  else
  {
    if (isFctConst)
    {
      return delegateMulticast.bind<&TestStruct::fctConst>(const_cast<const TestStruct&>(testStruct));
    }
    else
    {
      return delegateMulticast.bind<&TestStruct::fct>(testStruct);
    }
  }
}

auto bindObjectMemberFct(DelegateMulticast_T& delegateMulticast, TestStruct& testStruct, IsLValue isLValue, IsFctConst isFctConst)
{
  if (isLValue)
  {
    if (isFctConst)
    {
      return delegateMulticast.bindObject(const_cast<const TestStruct&>(testStruct)).memFn<&TestStruct::fctConst>();
    }
    else
    {
      return delegateMulticast.bindObject(testStruct).memFn<&TestStruct::fct>();
    }
  }
  else
  {
    if (isFctConst)
    {
      return delegateMulticast.bindObject(const_cast<const TestStruct&&>(std::move(testStruct))).memFn<&TestStruct::fctConst>();
    }
    else
    {
      return delegateMulticast.bindObject(std::move(testStruct)).memFn<&TestStruct::fct>();
    }
  }
}

auto bindObjectMemberFctConstOverloaded(DelegateMulticast_T& delegateMulticast, TestStruct& testStruct, IsLValue isLValue, IsFctConst isFctConst)
{
  if (!isLValue)
  {
    if (isFctConst)
    {
      return delegateMulticast.bindObject(const_cast<const TestStruct&&>(std::move(testStruct))).memFn<&TestStruct::fctConstOverloaded>();
    }
    else
    {
      return delegateMulticast.bindObject(std::move(testStruct)).memFn<&TestStruct::fctConstOverloaded>();
    }
  }
  else
  {
    if (isFctConst)
    {
      return delegateMulticast.bindObject(const_cast<const TestStruct&>(testStruct)).memFn<&TestStruct::fctConstOverloaded>();
    }
    else
    {
      return delegateMulticast.bindObject(testStruct).memFn<&TestStruct::fctConstOverloaded>();
    }
  }
}

auto bindObjectMemberFctParamOverloaded(DelegateMulticast_T& delegateMulticast, TestStruct& testStruct, IsLValue isLValue, IsFctConst isFctConst)
{
  if (isLValue)
  {
    if (!isFctConst)
    {
      return delegateMulticast.bindObject(testStruct).memFn<&TestStruct::fctParamOverloaded>();
    }
    else
    {
      return delegateMulticast.bindObject(const_cast<const TestStruct&>(testStruct)).memFn<&TestStruct::fctParamOverloaded>();
    }
  }
  else
  {
    if (isFctConst)
    {
      return delegateMulticast.bindObject(const_cast<const TestStruct&&>(std::move(testStruct))).memFn<&TestStruct::fctParamOverloaded>();
    }
    else
    {
      return delegateMulticast.bindObject(std::move(testStruct)).memFn<&TestStruct::fctParamOverloaded>();
    }
  }
}

std::string getTestNamePart(IsLValue isLValue, IsFctConst isFctConst, CreateDelegateMulticast* factory)
{
  static constexpr auto delegateTypeTuples = std::to_array<std::tuple<CreateDelegateMulticast*, const char*>>({
  /*{&createEmpty, "Empty"},*/
      {                  &bindFreeFunction,                         "FreeFct"},
      {                       &bindFunctor,                         "Functor"},
      {                     &bindMemberFct,                      "BindMemFct"},
      {               &bindObjectMemberFct,                "BindObjectMemFct"},
      {&bindObjectMemberFctConstOverloaded, "BindObjectMemFctConstOverloaded"},
      {&bindObjectMemberFctParamOverloaded, "BindObjectMemFctParamOverloaded"}
  });

  auto isRequestedFactory = [&factory](const std::tuple<CreateDelegateMulticast*, const char*>& element) { return factory == std::get<0>(element); };

  auto result = std::find_if(delegateTypeTuples.begin(), delegateTypeTuples.end(), isRequestedFactory);
  auto isLValueStr = isLValue ? "LValue" : "RValue";
  auto isConstStr = isFctConst ? "Const" : "";
  auto delegateTypeStr = (result != delegateTypeTuples.end()) ? std::get<1>(*result) : "Unknown";
  return std::format("{}{}{}", isLValueStr, delegateTypeStr, isConstStr);
}

using ParamSetType = std::tuple<IsLValue, IsFctConst, CreateDelegateMulticast*>;
class UnaryOpF : public testing::TestWithParam<ParamSetType>
{
public:
  static constexpr auto makeTestName = [](const testing::TestParamInfo<ParamType>& info)
  {
    auto [isLValueA, isFctConstA, delegateBinderA] = info.param;

    auto delegateTypeAName = getTestNamePart(isLValueA, isFctConstA, delegateBinderA);

    return delegateTypeAName;
  };

protected:
private:
};

class BinaryOpF : public testing::TestWithParam<std::tuple<ParamSetType, ParamSetType>>
{
public:
  static constexpr auto makeTestName = [](const testing::TestParamInfo<ParamType>& info)
  {
    auto [paramA, paramB] = info.param;
    auto [isLValueA, isFctConstA, delegateBinderA] = paramA;
    auto [isLValueB, isFctConstB, delegateBinderB] = paramB;

    auto delegateTypeAName = getTestNamePart(isLValueA, isFctConstA, delegateBinderA);
    auto delegateTypeBName = getTestNamePart(isLValueB, isFctConstB, delegateBinderB);

    return std::format("{}{}", delegateTypeAName, delegateTypeBName);
  };

protected:
private:
};

auto makeDelegateTypeParamSet()
{
  static constexpr std::array fctConstness = {true, false};
  static constexpr std::array valueKinds = {true, false};
  static constexpr std::array memberFctDelegateTypes = {&bindFunctor, &bindMemberFct, &bindObjectMemberFct, &bindObjectMemberFctConstOverloaded, &bindObjectMemberFctParamOverloaded};
  static constexpr auto memFctParamSetPartSize = valueKinds.size() * fctConstness.size() * memberFctDelegateTypes.size();
  auto memFctParamSet = std::vector<ParamSetType>();
  memFctParamSet.reserve(memFctParamSetPartSize);
  std::ranges::for_each(memberFctDelegateTypes,
                        [&memFctParamSet](CreateDelegateMulticast* createDelegateMulticast)
                        {
                          memFctParamSet.emplace_back(IsLValue(true), IsFctConst(true), createDelegateMulticast);
                          memFctParamSet.emplace_back(IsLValue(false), IsFctConst(false), createDelegateMulticast);
                          memFctParamSet.emplace_back(IsLValue(true), IsFctConst(false), createDelegateMulticast);
                          memFctParamSet.emplace_back(IsLValue(false), IsFctConst(true), createDelegateMulticast);
                        });

  static constexpr std::array fctDelegateTypes = {&bindFreeFunction};
  static constexpr auto fctParamSetPartSize = fctDelegateTypes.size();
  auto fctParamSet = std::vector<ParamSetType>();
  fctParamSet.reserve(fctParamSetPartSize);
  std::ranges::for_each(fctDelegateTypes,
                        [&fctParamSet](CreateDelegateMulticast* createDelegateMulticast) { fctParamSet.emplace_back(IsLValue(true), IsFctConst(false), createDelegateMulticast); });

  static constexpr auto paramSetPartSize = memFctParamSetPartSize + fctParamSetPartSize;
  auto paramSetPart = std::vector<ParamSetType>();
  paramSetPart.reserve(paramSetPartSize);
  std::copy(fctParamSet.begin(), fctParamSet.end(), std::back_inserter(paramSetPart));
  std::copy(memFctParamSet.begin(), memFctParamSet.end(), std::back_inserter(paramSetPart));

  // auto delegateTypesParamSet = std::vector<std::tuple<ParamSetPartType>>(paramSetPartSize*paramSetPartSize);

  return paramSetPart;
}

// auto memberFctParamSet = ::testing::Combine(::testing::ValuesIn(valueKinds), ::testing::ValuesIn(fctConstness), ::testing::ValuesIn(memberFctDelegateTypes));
// auto fctParamSet = ::testing::Combine(::testing::Values(true), ::testing::Values(false), ::testing::ValuesIn(fctDelegateTypes));
// auto delegateTypesParamSetPart = ::testing::Values(fctParamSet, memberFctParamSet);

auto emptyDelegateTypeParamSet = ::testing::Values(std::make_tuple(IsLValue(true), IsFctConst(false), &bindEmpty));
auto delegateTypesParamSet = ::testing::ValuesIn(makeDelegateTypeParamSet());

INSTANTIATE_TEST_SUITE_P(UnaryOpDelegateMulticast, UnaryOpF, delegateTypesParamSet, UnaryOpF::makeTestName);
INSTANTIATE_TEST_SUITE_P(BinaryOpDelegateMulticast, BinaryOpF, ::testing::Combine(delegateTypesParamSet, delegateTypesParamSet), BinaryOpF::makeTestName);

TEST_P(UnaryOpF, ConstructBroadcastDestroy)
{
  auto [isLValue, isFctConst, delegateBinder] = GetParam();

  TestStruct testStruct;
  DelegateMulticast_T delegateMulticast;
  ASSERT_TRUE((bool)delegateMulticast.isEmpty());

  delegateBinder(delegateMulticast, testStruct, isLValue, isFctConst);
  ASSERT_FALSE((bool)delegateMulticast.isEmpty());

  int value{};
  delegateMulticast.broadcast(value);
  if (isFctConst)
  {
    ASSERT_EQ(value, TestStruct::m_staticValueConst);
  }
  else
  {
    ASSERT_EQ(value, TestStruct::m_staticValue);
  }

  delegateMulticast.unbindAll();
  ASSERT_TRUE((bool)delegateMulticast.isEmpty());

  delegateBinder(delegateMulticast, testStruct, isLValue, isFctConst);
  ASSERT_FALSE((bool)delegateMulticast.isEmpty());

  delegateMulticast.~DelegateMulticast();
  ASSERT_TRUE((bool)delegateMulticast.isEmpty());
}

TEST_P(UnaryOpF, MoveCopyConstructBroadcast)
{
  auto [isLValue, isFctConst, delegateBinder] = GetParam();

  TestStruct testStruct;
  DelegateMulticast_T delegateMulticast;
  ASSERT_TRUE((bool)delegateMulticast.isEmpty());

  delegateBinder(delegateMulticast, testStruct, isLValue, isFctConst);
  ASSERT_FALSE((bool)delegateMulticast.isEmpty());

  DelegateMulticast_T delegateMulticastMove{std::move(delegateMulticast)};
  ASSERT_FALSE((bool)delegateMulticastMove.isEmpty());
  ASSERT_TRUE((bool)delegateMulticast.isEmpty());

  int value{};
  delegateMulticastMove.broadcast(value);
  if (isFctConst)
  {
    ASSERT_EQ(value, TestStruct::m_staticValueConst);
  }
  else
  {
    ASSERT_EQ(value, TestStruct::m_staticValue);
  }

  DelegateMulticast_T delegateMulticastCopy{delegateMulticastMove};
  ASSERT_FALSE((bool)delegateMulticastCopy.isEmpty());

  // auto test = delegateMulticastMove == delegateMulticastCopy;
  // ASSERT_EQ(delegateMulticastMove, delegateMulticastCopy);

  value = {};
  delegateMulticastMove.broadcast(value);
  if (isFctConst)
  {
    ASSERT_EQ(value, TestStruct::m_staticValueConst);
  }
  else
  {
    ASSERT_EQ(value, TestStruct::m_staticValue);
  }

  value = {};
  delegateMulticastCopy.broadcast(value);
  if (isFctConst)
  {
    ASSERT_EQ(value, TestStruct::m_staticValueConst);
  }
  else
  {
    ASSERT_EQ(value, TestStruct::m_staticValue);
  }
}

TEST_P(UnaryOpF, MoveCopyAssignBroadcast)
{
  auto [isLValue, isFctConst, delegateBinder] = GetParam();

  TestStruct testStruct;
  DelegateMulticast_T delegateMulticast;
  ASSERT_TRUE((bool)delegateMulticast.isEmpty());

  delegateBinder(delegateMulticast, testStruct, isLValue, isFctConst);
  ASSERT_FALSE((bool)delegateMulticast.isEmpty());

  DelegateMulticast_T delegateMulticastMove;
  delegateMulticastMove = std::move(delegateMulticast);
  ASSERT_FALSE((bool)delegateMulticastMove.isEmpty());
  ASSERT_TRUE((bool)delegateMulticast.isEmpty());

  int value{};
  delegateMulticastMove.broadcast(value);
  if (isFctConst)
  {
    ASSERT_EQ(value, TestStruct::m_staticValueConst);
  }
  else
  {
    ASSERT_EQ(value, TestStruct::m_staticValue);
  }

  delegateBinder(delegateMulticast, testStruct, isLValue, isFctConst);
  ASSERT_FALSE((bool)delegateMulticast.isEmpty());

  DelegateMulticast_T delegateMulticastCopy;
  delegateMulticastCopy = delegateMulticastMove;
  ASSERT_FALSE((bool)delegateMulticastCopy.isEmpty());

  value = {};
  delegateMulticastMove.broadcast(value);
  if (isFctConst)
  {
    ASSERT_EQ(value, TestStruct::m_staticValueConst);
  }
  else
  {
    ASSERT_EQ(value, TestStruct::m_staticValue);
  }

  value = {};
  delegateMulticastCopy.broadcast(value);
  if (isFctConst)
  {
    ASSERT_EQ(value, TestStruct::m_staticValueConst);
  }
  else
  {
    ASSERT_EQ(value, TestStruct::m_staticValue);
  }
}

TEST_P(BinaryOpF, SpecialFcts)
{
  auto [paramA, paramB] = GetParam();
  auto [isLValueA, isFctConstA, delegateBinderA] = paramA;
  auto [isLValueB, isFctConstB, delegateBinderB] = paramB;

  TestStruct testStructA;
  DelegateMulticast_T delegateMulticastA;
  ASSERT_TRUE((bool)delegateMulticastA.isEmpty());

  delegateBinderA(delegateMulticastA, testStructA, isLValueA, isFctConstA);
  ASSERT_FALSE((bool)delegateMulticastA.isEmpty());

  TestStruct testStructB;
  DelegateMulticast_T delegateMulticastB;
  ASSERT_TRUE((bool)delegateMulticastB.isEmpty());

  delegateBinderB(delegateMulticastB, testStructB, isLValueB, isFctConstB);
  ASSERT_FALSE((bool)delegateMulticastB.isEmpty());
}

TEST(DelegateMulticast, LValueFunctor) {}

TEST(DelegateMulticast, DefaultConstructorDestructor)
{
  DelegateMulticast_T delegateMulticast;
  ASSERT_TRUE((bool)delegateMulticast.isEmpty());

  delegateMulticast.~delegateMulticast();
  ASSERT_TRUE((bool)delegateMulticast.isEmpty());
}

// TEST(DelegateMulticast, CopyConstructor)
//{
//   TestStruct::resetStaticCounters();
//   {
//     DelegateMulticast_T delegateMulticast;
//     delegateMulticast.Bind<&freeFunction>();
//     ASSERT_TRUE((bool)delegateMulticast);
//
//     DelegateMulticast_T delegateMulticastCopy{delegateMulticast};
//     ASSERT_TRUE((bool)delegateMulticast);
//     ASSERT_TRUE((bool)delegateMulticastCopy);
//
//     auto value = 1;
//     delegateMulticastCopy.broadcast(value);
//     ASSERT_EQ(value, TestStruct::m_staticValue);
//   }
//   const uint8_t expectedNbCallDefaultConstructor = 0;
//   ASSERT_EQ(expectedNbCallDefaultConstructor, TestStruct::m_counters.m_nbCallDefaultConstructor);
//   const uint8_t expectedNbCallDestructor = 0;
//   ASSERT_EQ(expectedNbCallDestructor, TestStruct::m_counters.m_nbCallDestructor);
//   const uint8_t expectedNbCallCopyConstructor = 0;
//   ASSERT_EQ(expectedNbCallCopyConstructor, TestStruct::m_counters.m_nbCallCopyConstructor);
//   const uint8_t expectedNbCallMoveConstructor = 0;
//   ASSERT_EQ(expectedNbCallMoveConstructor, TestStruct::m_counters.m_nbCallMoveConstructor);
//   const uint8_t nbConstructorCall = TestStruct::m_counters.m_nbCallDefaultConstructor + TestStruct::m_counters.m_nbCallCopyConstructor + TestStruct::m_counters.m_nbCallMoveConstructor;
//   ASSERT_EQ(nbConstructorCall, TestStruct::m_counters.m_nbCallDestructor);
//
//   SpecialFunctionCallCounter expectedCounter;
//   expectedCounter.m_nbCallDefaultConstructor = 0;
//   expectedCounter.m_nbCallDestructor = 0;
//   expectedCounter.m_nbCallCopyConstructor = 0;
//   expectedCounter.m_nbCallMoveConstructor = 0;
//   assertSpecialFunctionCallCounter(expectedCounter);
// }

} // namespace Delegate