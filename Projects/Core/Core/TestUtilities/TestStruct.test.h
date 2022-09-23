#pragma once

#include "Core/GoogleTest.h"

namespace Test
{

struct SpecialFunctionCallCounter
{
  volatile uint8_t m_nbCallDefaultConstructor = 0;
  volatile uint8_t m_nbCallCopyConstructor = 0;
  volatile uint8_t m_nbCallMoveConstructor = 0;
  volatile uint8_t m_nbCallDestructor = 0;
};

struct TestStruct
{
  constexpr static const uint8_t m_staticValue = 42;
  constexpr static const uint8_t m_staticValueConst = m_staticValue + 1;
  constexpr static const double m_staticValueDouble = m_staticValue + 0.5;
  constexpr static const double m_staticValueDoubleConst = m_staticValueDouble + 0.5 + 1;
  uint8_t m_value = m_staticValue;

  inline static SpecialFunctionCallCounter m_counters;

  static void assertSpecialFunctionCallCounter(const SpecialFunctionCallCounter& expectedCounter)
  {
    EXPECT_EQ(expectedCounter.m_nbCallDefaultConstructor, m_counters.m_nbCallDefaultConstructor);
    EXPECT_EQ(expectedCounter.m_nbCallDestructor, m_counters.m_nbCallDestructor);
    EXPECT_EQ(expectedCounter.m_nbCallCopyConstructor, m_counters.m_nbCallCopyConstructor);
    EXPECT_EQ(expectedCounter.m_nbCallMoveConstructor, m_counters.m_nbCallMoveConstructor);

    const uint8_t nbConstructorCall = m_counters.m_nbCallDefaultConstructor + m_counters.m_nbCallCopyConstructor + m_counters.m_nbCallMoveConstructor;
    EXPECT_EQ(nbConstructorCall, m_counters.m_nbCallDestructor);
  }

  TestStruct(uint8_t value = m_staticValue) noexcept
  : m_value{value}
  {
    TestStruct::m_counters.m_nbCallDefaultConstructor++;
  }

  ~TestStruct() noexcept
  {
    TestStruct::m_counters.m_nbCallDestructor++;
  }

  TestStruct(const TestStruct& obj) noexcept
  : m_value{obj.m_value}
  {
    TestStruct::m_counters.m_nbCallCopyConstructor++;
  }

  TestStruct(TestStruct&& obj) noexcept
  : m_value{std::move(obj.m_value)}
  {
    TestStruct::m_counters.m_nbCallMoveConstructor++;
  }

  bool operator==(const TestStruct&) const = default;

  static void staticFunction(int& value)
  {
    value = m_staticValue;
  }

  int fctReturn(int& value)
  {
    value = m_value;
    return value;
  }

  testing::AssertionResult fctAssertionresult(int& value)
  {
    value = m_value;
    return testing::AssertionSuccess();
    ;
  }

  void fct(int& value)
  {
    value = m_value;
  }

  void fctConst(int& value) const
  {
    value = m_value + 1;
  }

  void fctTemplate(auto& value)
  {
    value = m_value;
  }

  void fctTemplate(auto& value) const
  {
    value = m_value + 1;
  }

  void fctParamOverloaded(int& value)
  {
    value = m_value;
  }

  void fctParamOverloaded(int& value) const
  {
    value = m_value + 1;
  }

  void fctParamOverloaded(double& value)
  {
    value = m_value + 0.5;
  }

  void fctParamOverloaded(double& value) const
  {
    value = (m_value + 0.5) + 1;
  }

  void fctConstOverloaded(int& value)
  {
    value = m_value;
  }

  void fctConstOverloaded(int& value) const
  {
    value = m_value + 1;
  }

  void operator()(int& value)
  {
    value = m_value;
  }

  void operator()(int& value) const
  {
    value = m_value + 1;
  }

  static void resetStaticCounters()
  {
    TestStruct::m_counters = {};
  }
};

static inline void freeFunction(int& value)
{
  value = TestStruct::m_staticValue;
}

#define ExpectSpecialFunctionCallCounter(expectedCounter)                                                   \
  EXPECT_EQ(expectedCounter.m_nbCallDefaultConstructor, TestStruct::m_counters.m_nbCallDefaultConstructor); \
  EXPECT_EQ(expectedCounter.m_nbCallDestructor, TestStruct::m_counters.m_nbCallDestructor);                 \
  EXPECT_EQ(expectedCounter.m_nbCallCopyConstructor, TestStruct::m_counters.m_nbCallCopyConstructor);       \
  EXPECT_EQ(expectedCounter.m_nbCallMoveConstructor, TestStruct::m_counters.m_nbCallMoveConstructor);

#define ExpectConstructorsAndDestructorsCount(expectedCounter)                                                                                                                           \
  const uint8_t nbConstructorCall = TestStruct::m_counters.m_nbCallDefaultConstructor + TestStruct::m_counters.m_nbCallCopyConstructor + TestStruct::m_counters.m_nbCallMoveConstructor; \
  EXPECT_EQ(nbConstructorCall, expectedCounter.m_nbCallDestructor)

} // namespace Test