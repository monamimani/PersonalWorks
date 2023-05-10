#pragma once

#include <algorithm>
#include <concepts>
#include <functional>
#include <source_location>
#include <span>
#include <string_view>
#include <type_traits>
#include <utility>

#include "fmt/format.h"
#include "TestUtilities/GoogleTest.h"
#include "TestUtilities/TestStruct.test.h"

namespace TestUtilities
{
enum class BasicTestsObjOps : std::uint8_t
{
  DefaultCtor,
  Dtor,
  CopyCtor,
  MoveCtor,
  CopyAssign,
  MoveAssign,
  MovedFrom,
  Functor,
  ConvertibleBool,
  Equal,
  LessEqual,
  LessThan,
  GreaterEqual,
  GreaterThan,
  Swap
};

template<typename TestFixtureType, typename TestedType, BasicTestsObjOps objOps>
concept DefaultObjTestable = requires(const TestFixtureType& fixtureConst, TestFixtureType& fixture, const TestedType& arg) {
  {
    fixtureConst.template testDefaultObj<objOps>(arg)
  } -> std::same_as<testing::AssertionResult>;
  {
    fixture.getDefaultObj()
  } -> std::same_as<TestedType>;
};

// template<typename TestFixtureType, typename TestedType>
// concept DefaultObjDtorStateTestable = requires(const TestFixtureType& fixture, const TestedType& arg) {
//   {
//     fixture.template testDefaultObj<BasicTestsObjOps::Dtor>(std::as_bytes(std::span{std::addressof(arg), 1}))
//   } -> std::same_as<testing::AssertionResult>;
// };

template<typename TestFixtureType, typename TestedType, BasicTestsObjOps objOps>
concept ObjATestable = requires(const TestFixtureType& fixtureConst, TestFixtureType& fixture, const TestedType& arg) {
  {
    fixtureConst.template testObjA<objOps>(arg)
  } -> std::same_as<testing::AssertionResult>;
  {
    fixture.getObjA()
  } -> std::same_as<TestedType>;
};

template<typename TestFixtureType, typename TestedType, BasicTestsObjOps objOps>
concept ObjBTestable = requires(const TestFixtureType& fixtureConst, TestFixtureType& fixture, const TestedType& arg) {
  {
    fixtureConst.template testObjB<objOps>(arg)
  } -> std::same_as<testing::AssertionResult>;
  {
    fixture.getObjB()
  } -> std::same_as<TestedType>;
};

template<typename TestFixtureType, typename TestedType>
class DefaultCtorTest: public TestFixtureType
{
public:
  explicit DefaultCtorTest() = default;

  template<typename ParamT>
  explicit DefaultCtorTest(ParamT&& param)
  : TestFixtureType(std::forward<ParamT>(param))
  {}

  static consteval auto getTestName()
  {
    return "DefaultCtor";
  }

  static consteval auto getSourceLocation()
  {
    return std::source_location::current();
  }

  using TestFixtureType::testDefaultObj;

  void TestBody() override
  {
    static_assert(DefaultObjTestable<TestFixtureType, TestedType, BasicTestsObjOps::DefaultCtor>);

    TestedType obj = TestFixtureType::getDefaultObj();
    EXPECT_TRUE(testDefaultObj<BasicTestsObjOps::DefaultCtor>(obj));

    // if constexpr (DefaultObjDtorStateTestable<TestFixtureType, TestedType>)
    //{
    obj.~TestedType();
    EXPECT_TRUE(testDefaultObj<BasicTestsObjOps::Dtor>(obj)); // TODO: Maybe it would be best to have a function dedicated to test obj after the destructor and
                                                              // have a concept to detect if the Fixture type has the function
    //}
  }

private:
};

template<typename TestFixtureType, typename TestedType>
class CopyCtorTest: public TestFixtureType
{
public:
  explicit CopyCtorTest() = default;

  template<typename ParamT>
  explicit CopyCtorTest(ParamT&& param)
  : TestFixtureType(std::forward<ParamT>(param))
  {}

  static consteval auto getTestName()
  {
    return "CopyCtor";
  }

  static consteval auto getSourceLocation()
  {
    return std::source_location::current();
  }

  using TestFixtureType::testDefaultObj;
  using TestFixtureType::testObjA;

  void TestBody() override
  {
    static_assert(DefaultObjTestable<TestFixtureType, TestedType, BasicTestsObjOps::CopyCtor>);

    {
      TestedType obj = TestFixtureType::getDefaultObj();
      auto objCopy{obj};
      EXPECT_TRUE(testDefaultObj<BasicTestsObjOps::CopyCtor>(objCopy));

      // if constexpr (DefaultObjDtorStateTestable<TestFixtureType, TestedType>)
      //{
      objCopy.~TestedType();
      EXPECT_TRUE(testDefaultObj<BasicTestsObjOps::Dtor>(objCopy));
      //}
    }

    if constexpr (ObjATestable<TestFixtureType, TestedType, BasicTestsObjOps::CopyCtor>)
    {

      auto objCopy{TestFixtureType::getObjA()};
      EXPECT_TRUE(testObjA<BasicTestsObjOps::CopyCtor>(objCopy));

      // if constexpr (DefaultObjDtorStateTestable<TestFixtureType, TestedType>)
      //{
      objCopy.~TestedType();
      EXPECT_TRUE(testObjA<BasicTestsObjOps::Dtor>(objCopy));
      //}
    }
  }

private:
};

template<typename TestFixtureType, typename TestedType>
class MoveCtorTest: public TestFixtureType
{
public:
  explicit MoveCtorTest() = default;

  template<typename ParamT>
  explicit MoveCtorTest(ParamT&& param)
  : TestFixtureType(std::forward<ParamT>(param))
  {}

  static consteval auto getTestName()
  {
    return "MoveCtor";
  }

  static consteval auto getSourceLocation()
  {
    return std::source_location::current();
  }

  using TestFixtureType::testDefaultObj;
  using TestFixtureType::testObjA;

  void TestBody() override
  {
    static_assert(DefaultObjTestable<TestFixtureType, TestedType, BasicTestsObjOps::MoveCtor>);

    {
      TestedType obj = TestFixtureType::getDefaultObj();
      auto objMove{std::move(obj)};
      EXPECT_TRUE(testDefaultObj<BasicTestsObjOps::MoveCtor>(objMove));
      EXPECT_TRUE(testDefaultObj<BasicTestsObjOps::MovedFrom>(obj));

      // if constexpr (DefaultObjDtorStateTestable<TestFixtureType, TestedType>)
      //{
      objMove.~TestedType();
      EXPECT_TRUE(testDefaultObj<BasicTestsObjOps::Dtor>(objMove));
      //}
    }

    if constexpr (ObjATestable<TestFixtureType, TestedType, BasicTestsObjOps::MoveCtor>)
    {
      auto objA = TestFixtureType::getObjA();
      auto objMove{std::move(objA)};
      EXPECT_TRUE(testObjA<BasicTestsObjOps::MoveCtor>(objMove));
      EXPECT_TRUE(testObjA<BasicTestsObjOps::MovedFrom>(objA));

      // if constexpr (DefaultObjDtorStateTestable<TestFixtureType, TestedType>)
      //{
      objMove.~TestedType();
      EXPECT_TRUE(testObjA<BasicTestsObjOps::Dtor>(objMove));
      //}
    }
  }

private:
};

template<typename TestFixtureType, typename TestedType>
class CopyAssignTest: public TestFixtureType
{
public:
  explicit CopyAssignTest() = default;

  template<typename ParamT>
  explicit CopyAssignTest(ParamT&& param)
  : TestFixtureType(std::forward<ParamT>(param))
  {}

  static consteval auto getTestName()
  {
    return "CopyAssign";
  }

  static consteval auto getSourceLocation()
  {
    return std::source_location::current();
  }

  using TestFixtureType::testDefaultObj;
  using TestFixtureType::testObjA;

  void TestBody() override
  {
    static_assert(DefaultObjTestable<TestFixtureType, TestedType, BasicTestsObjOps::CopyAssign>);

    {
      TestedType obj = TestFixtureType::getDefaultObj();
      TestedType objCopy;
      objCopy = obj;
      EXPECT_TRUE(testDefaultObj<BasicTestsObjOps::CopyAssign>(objCopy));

      // if constexpr (DefaultObjDtorStateTestable<TestFixtureType, TestedType>)
      //{
      objCopy.~TestedType();
      EXPECT_TRUE(testDefaultObj<BasicTestsObjOps::Dtor>(objCopy));
      //}
    }

    if constexpr (ObjATestable<TestFixtureType, TestedType, BasicTestsObjOps::CopyAssign>)
    {
      auto objA = TestFixtureType::getObjA();
      TestedType objCopy;
      objCopy = objA;
      EXPECT_TRUE(testObjA<BasicTestsObjOps::CopyAssign>(objCopy));

      // if constexpr (DefaultObjDtorStateTestable<TestFixtureType, TestedType>)
      //{
      objCopy.~TestedType();
      EXPECT_TRUE(testObjA<BasicTestsObjOps::Dtor>(objCopy));
      //}
    }
  }

private:
};

template<typename TestFixtureType, typename TestedType>
class MoveAssignTest: public TestFixtureType
{
public:
  explicit MoveAssignTest() = default;

  template<typename ParamT>
  explicit MoveAssignTest(ParamT&& param)
  : TestFixtureType(std::forward<ParamT>(param))
  {}

  static consteval auto getTestName()
  {
    return "MoveAssign";
  }

  static consteval auto getSourceLocation()
  {
    return std::source_location::current();
  }

  using TestFixtureType::testDefaultObj;
  using TestFixtureType::testObjA;

  void TestBody() override
  {
    static_assert(DefaultObjTestable<TestFixtureType, TestedType, BasicTestsObjOps::MoveAssign>);

    {
      TestedType obj = TestFixtureType::getDefaultObj();
      TestedType objMove;
      objMove = std::move(obj);
      EXPECT_TRUE(testDefaultObj<BasicTestsObjOps::MoveAssign>(objMove));
      EXPECT_TRUE(testDefaultObj<BasicTestsObjOps::MovedFrom>(obj));

      // if constexpr (DefaultObjDtorStateTestable<TestFixtureType, TestedType>)
      //{
      objMove.~TestedType();
      EXPECT_TRUE(testDefaultObj<BasicTestsObjOps::Dtor>(objMove));
      //}
    }

    if constexpr (ObjATestable<TestFixtureType, TestedType, BasicTestsObjOps::CopyAssign>)
    {
      auto objA = TestFixtureType::getObjA();
      TestedType objMove;
      objMove = std::move(objA);
      EXPECT_TRUE(testObjA<BasicTestsObjOps::MoveAssign>(objMove));
      EXPECT_TRUE(testObjA<BasicTestsObjOps::MovedFrom>(objA));

      // if constexpr (DefaultObjDtorStateTestable<TestFixtureType, TestedType>)
      //{
      objMove.~TestedType();
      EXPECT_TRUE(testObjA<BasicTestsObjOps::Dtor>(objMove));
      //}
    }
  }

private:
};

// template<typename TestFixtureType, typename TestedType>
// class FunctorTest: public TestFixtureType
//{
// public:
//   explicit FunctorTest() = default;
//
//   template<typename ParamT>
//   explicit FunctorTest(ParamT&& param)
//   : TestFixtureType(std::forward<ParamT>(param))
//   {}
//
//   static consteval auto getTestName()
//   {
//     return "FunctorTest";
//   }
//
//   static consteval auto getSourceLocation()
//   {
//     return std::source_location::current();
//   }
//
//   using TestFixtureType::testDefaultObj;
//
//   void TestBody() override
//   {
//     static_assert(DefaultObjTestable<TestFixtureType, TestedType, BasicTestsObjOps::FunctorTest>);
//
//     GTEST_SKIP_("Not sure how to implement BasicTestsObjOps::FunctorTest");
//   }
//
// private:
// };

template<typename TestFixtureType, typename TestedType>
class ConvertibleBoolTest: public TestFixtureType
{
public:
  explicit ConvertibleBoolTest() = default;

  template<typename ParamT>
  explicit ConvertibleBoolTest(ParamT&& param)
  : TestFixtureType(std::forward<ParamT>(param))
  {}

  static consteval auto getTestName()
  {
    return "ConvertibleBool";
  }

  static consteval auto getSourceLocation()
  {
    return std::source_location::current();
  }

  using TestFixtureType::testDefaultObj;
  using TestFixtureType::testObjA;

  void TestBody() override
  {
    static_assert(requires(const TestFixtureType& fixtureConst, TestFixtureType& fixture, const TestedType& arg) {
      {
        testDefaultObj<BasicTestsObjOps::ConvertibleBool>(arg)
      } -> std::same_as<testing::AssertionResult>;
      {
        TestFixtureType::getObjA()
      } -> std::same_as<TestedType>;
    });

    EXPECT_TRUE((bool)TestFixtureType::getObjA());
  }

private:
};

template<typename TestFixtureType, typename TestedType>
class EqualCmpTest: public TestFixtureType
{
public:
  explicit EqualCmpTest() = default;

  template<typename ParamT>
  explicit EqualCmpTest(ParamT&& param)
  : TestFixtureType(std::forward<ParamT>(param))
  {}

  static consteval auto getTestName()
  {
    return "EqualCmp";
  }

  static consteval auto getSourceLocation()
  {
    return std::source_location::current();
  }

  using TestFixtureType::testDefaultObj;
  using TestFixtureType::testObjA;
  using TestFixtureType::testObjB;

  void TestBody() override
  {
    static_assert(ObjATestable<TestFixtureType, TestedType, BasicTestsObjOps::Equal>);
    static_assert(ObjBTestable<TestFixtureType, TestedType, BasicTestsObjOps::Equal>);

    TestedType objA = TestFixtureType::getObjA();
    EXPECT_TRUE(testObjA<BasicTestsObjOps::Equal>(objA));
    ASSERT_EQ(objA, objA);

    TestedType objB = TestFixtureType::getObjB();
    EXPECT_TRUE(testObjA<BasicTestsObjOps::Equal>(objA));
    EXPECT_TRUE(testObjB<BasicTestsObjOps::Equal>(objB));
    ASSERT_NE(objA, objB);

    objB.~TestedType();
    EXPECT_TRUE(testObjB<BasicTestsObjOps::Dtor>(objB));
  }

private:
};

template<typename TestFixtureType, typename TestedType>
class OrderingCmpTest: public TestFixtureType
{
public:
  explicit OrderingCmpTest() = default;

  template<typename ParamT>
  explicit OrderingCmpTest(ParamT&& param)
  : TestFixtureType(std::forward<ParamT>(param))
  {}

  static consteval auto getTestName()
  {
    return "OrderingCmp";
  }

  static consteval auto getSourceLocation()
  {
    return std::source_location::current();
  }

  using TestFixtureType::testDefaultObj;
  using TestFixtureType::testObjA;
  using TestFixtureType::testObjB;

  void TestBody() override
  {

    static constexpr bool isLessEqualThanCmp = requires(const TestedType& objA, const TestedType& objB) { objA <= objB; };
    static constexpr bool isLessThanCmp = requires(const TestedType& objA, const TestedType& objB) { objA < objB; };
    static constexpr bool isGreaterEqualThanCmp = requires(const TestedType& objA, const TestedType& objB) { objA >= objB; };
    static constexpr bool isGreaterThanCmp = requires(const TestedType& objA, const TestedType& objB) { objA > objB; };

    TestedType objA = TestFixtureType::getObjA();
    TestedType objB = TestFixtureType::getObjB();

    if constexpr (isLessEqualThanCmp)
    {
      static_assert(ObjATestable<TestFixtureType, TestedType, BasicTestsObjOps::LessEqual>);

      EXPECT_TRUE(testObjA<BasicTestsObjOps::LessEqual>(objA));

      ASSERT_LE(objA, objA);
      EXPECT_TRUE(testObjA<BasicTestsObjOps::LessEqual>(objA));
    }

    if constexpr (isLessThanCmp)
    {
      static_assert(ObjATestable<TestFixtureType, TestedType, BasicTestsObjOps::LessThan>);
      static_assert(ObjBTestable<TestFixtureType, TestedType, BasicTestsObjOps::LessThan>);

      EXPECT_TRUE(testObjA<BasicTestsObjOps::LessThan>(objA));
      EXPECT_TRUE(testObjB<BasicTestsObjOps::LessThan>(objB));

      ASSERT_LT(objA, objB);
      EXPECT_TRUE(testObjA<BasicTestsObjOps::LessThan>(objA));
      EXPECT_TRUE(testObjB<BasicTestsObjOps::LessThan>(objB));
    }

    if constexpr (isGreaterEqualThanCmp)
    {
      static_assert(ObjATestable<TestFixtureType, TestedType, BasicTestsObjOps::GreaterEqual>);

      EXPECT_TRUE(testObjA<BasicTestsObjOps::GreaterEqual>(objA));

      ASSERT_GE(objA, objA);
      EXPECT_TRUE(testObjA<BasicTestsObjOps::GreaterEqual>(objA));
    }

    if constexpr (isGreaterThanCmp)
    {
      static_assert(ObjATestable<TestFixtureType, TestedType, BasicTestsObjOps::GreaterThan>);
      static_assert(ObjBTestable<TestFixtureType, TestedType, BasicTestsObjOps::GreaterThan>);

      EXPECT_TRUE(testObjA<BasicTestsObjOps::GreaterThan>(objA));
      EXPECT_TRUE(testObjB<BasicTestsObjOps::GreaterThan>(objB));

      ASSERT_GT(objB, objA);
      EXPECT_TRUE(testObjA<BasicTestsObjOps::GreaterThan>(objA));
      EXPECT_TRUE(testObjB<BasicTestsObjOps::GreaterThan>(objB));
    }
  }

private:
};

template<typename TestFixtureType, typename TestedType>
class SwapTest: public TestFixtureType
{
public:
  explicit SwapTest() = default;

  template<typename ParamT>
  explicit SwapTest(ParamT&& param)
  : TestFixtureType(std::forward<ParamT>(param))
  {}

  static consteval auto getTestName()
  {
    return "Swap";
  }

  static consteval auto getSourceLocation()
  {
    return std::source_location::current();
  }

  using TestFixtureType::testDefaultObj;
  using TestFixtureType::testObjA;
  using TestFixtureType::testObjB;

  void TestBody() override
  {
    static_assert(ObjATestable<TestFixtureType, TestedType, BasicTestsObjOps::Swap>);
    static_assert(ObjBTestable<TestFixtureType, TestedType, BasicTestsObjOps::Swap>);

    TestedType objA = TestFixtureType::getObjA();
    TestedType objB = TestFixtureType::getObjB();

    EXPECT_TRUE(testObjA<BasicTestsObjOps::Swap>(objA));
    EXPECT_TRUE(testObjB<BasicTestsObjOps::Swap>(objB));

    using std::swap;
    swap(objA, objB);

    EXPECT_TRUE(testObjA<BasicTestsObjOps::Swap>(objB));
    EXPECT_TRUE(testObjB<BasicTestsObjOps::Swap>(objA));

    ASSERT_EQ(objA, TestFixtureType::getObjB());
    ASSERT_EQ(objB, TestFixtureType::getObjA());
  }

private:
};

inline auto objAsBytes(const auto& obj)
{
  return std::as_bytes(std::span{std::addressof(obj), 1});
}

template<typename TestedType_>
struct TypedTestDesc
{
  using TestedType = TestedType_;
  const char* m_typeName;
};

template<bool enableTest, typename TestFixture, typename TestType>
void registerTest(const std::string& typeName)
{
  if constexpr (enableTest)
  {
    constexpr auto sourceLocation = TestType::getSourceLocation();

    testing::RegisterTest(("SpecialMemberFctTests/" + typeName).c_str(),
                          TestType::getTestName(),
                          typeName.c_str(),
                          nullptr,
                          sourceLocation.file_name(),
                          sourceLocation.line(),
                          // Important to use the fixture type as the return type here.
                          []() -> TestFixture* {
                            return new TestType{};
                          });
  }
}

template<bool enableTest, typename TestFixture, typename TestType>
void registerTest(const std::string& typeName, const auto& testParamGenerator, const auto& getTestParamName)
{
  if constexpr (enableTest)
  {
    using IteratorType = typename std::remove_reference_t<decltype(testParamGenerator)>::iterator;
    using ParamType = typename IteratorType::value_type;
    constexpr auto sourceLocation = TestType::getSourceLocation();
    auto it = testParamGenerator.begin();
    std::size_t i = 0;
    for (; it != testParamGenerator.end(); it++, i++)
    {
      std::string paramTestName = getTestParamName(testing::TestParamInfo<ParamType>(*it, i));
      auto testName = fmt::format("{}/{}", std::string{TestType::getTestName()}, paramTestName);
      testing::RegisterTest(("SpecialMemberFctTestsParametric/" + typeName).c_str(),
                            testName.c_str(),
                            typeName.c_str(),
                            nullptr,
                            sourceLocation.file_name(),
                            sourceLocation.line(),
                            // Important to use the fixture type as the return type here.
                            [param = *it]() -> TestFixture* {
                              return new TestType{param};
                            });
    }
  }
}

template<template<typename TestedType> typename TestFixtureType, auto TypedTestDesc>
void registerTests()
{
  using TestedType = decltype(TypedTestDesc)::TestedType;
  using FixtureType = TestFixtureType<TestedType>;

  static_assert(std::derived_from<TestFixtureType<TestedType>, testing::Test>, "Fixture type need to derive from gTest testing::Test");

  static constexpr bool isConvertibleToBool = requires(const TestedType& obj) { (bool)obj; };
  static constexpr bool isLessThanCmp = requires(const TestedType& objA, const TestedType& objB) { objA < objB; };
  static constexpr bool isLessEqualThanCmp = requires(const TestedType& objA, const TestedType& objB) { objA <= objB; };
  static constexpr bool isGreaterThanCmp = requires(const TestedType& objA, const TestedType& objB) { objA > objB; };
  static constexpr bool isGreaterEqualThanCmp = requires(const TestedType& objA, const TestedType& objB) { objA >= objB; };
  static constexpr bool isOrdered = isLessThanCmp || isLessEqualThanCmp || isGreaterThanCmp || isGreaterEqualThanCmp;

  std::string typeNameNoSpace = TypedTestDesc.m_typeName;
  std::erase(typeNameNoSpace, ' ');

  registerTest<std::is_default_constructible_v<TestedType>, FixtureType, DefaultCtorTest<FixtureType, TestedType>>(typeNameNoSpace);
  registerTest<std::is_copy_constructible_v<TestedType>, FixtureType, CopyCtorTest<FixtureType, TestedType>>(typeNameNoSpace);
  registerTest<std::is_move_constructible_v<TestedType>, FixtureType, MoveCtorTest<FixtureType, TestedType>>(typeNameNoSpace);
  registerTest<std::is_copy_assignable_v<TestedType>, FixtureType, CopyAssignTest<FixtureType, TestedType>>(typeNameNoSpace);
  registerTest<std::is_move_assignable_v<TestedType>, FixtureType, MoveAssignTest<FixtureType, TestedType>>(typeNameNoSpace);
  // registerTest<std::invocable<TestedType>, FixtureType, FunctorTest<FixtureType, TestedType>>(typeNameNoSpace);
  registerTest<isConvertibleToBool, FixtureType, ConvertibleBoolTest<FixtureType, TestedType>>(typeNameNoSpace);
  registerTest<std::equality_comparable<TestedType>, FixtureType, EqualCmpTest<FixtureType, TestedType>>(typeNameNoSpace);
  registerTest<isOrdered, FixtureType, OrderingCmpTest<FixtureType, TestedType>>(typeNameNoSpace);
  registerTest<std::swappable<TestedType>, FixtureType, SwapTest<FixtureType, TestedType>>(typeNameNoSpace);
}

template<template<typename TestedTypeT> typename TestFixtureType, auto TypedTestDesc>
void registerTests(const auto& testParams, const auto& getTestParamName)
{
  using TestedType = decltype(TypedTestDesc)::TestedType;
  using FixtureType = TestFixtureType<TestedType>;

  static_assert(std::derived_from<FixtureType, testing::Test>, "Fixture type need to derive from gTest testing::Test");
  static_assert(requires() { typename FixtureType::ParamType; });
  static_assert(std::derived_from<FixtureType, testing::TestWithParam<typename FixtureType::ParamType>>,
                "Fixture type need to derive from gTest testing::TestWithParam");

  static constexpr bool isConvertibleToBool = requires(const TestedType& obj) { (bool)obj; };
  static constexpr bool isLessThanCmp = requires(const TestedType& objA, const TestedType& objB) { objA < objB; };
  static constexpr bool isLessEqualThanCmp = requires(const TestedType& objA, const TestedType& objB) { objA <= objB; };
  static constexpr bool isGreaterThanCmp = requires(const TestedType& objA, const TestedType& objB) { objA > objB; };
  static constexpr bool isGreaterEqualThanCmp = requires(const TestedType& objA, const TestedType& objB) { objA >= objB; };
  static constexpr bool isOrdered = isLessThanCmp || isLessEqualThanCmp || isGreaterThanCmp || isGreaterEqualThanCmp;

  std::string typeNameNoSpace = TypedTestDesc.m_typeName;
  std::erase(typeNameNoSpace, ' ');

  const ::testing::internal::ParamGenerator<typename FixtureType::ParamType>& testParamGenerator = testParams;

  registerTest<std::is_default_constructible_v<TestedType>, FixtureType, DefaultCtorTest<FixtureType, TestedType>>(
      typeNameNoSpace, testParamGenerator, getTestParamName);
  registerTest<std::is_copy_constructible_v<TestedType>, FixtureType, CopyCtorTest<FixtureType, TestedType>>(
      typeNameNoSpace, testParamGenerator, getTestParamName);
  registerTest<std::is_move_constructible_v<TestedType>, FixtureType, MoveCtorTest<FixtureType, TestedType>>(
      typeNameNoSpace, testParamGenerator, getTestParamName);
  registerTest<std::is_copy_assignable_v<TestedType>, FixtureType, CopyAssignTest<FixtureType, TestedType>>(
      typeNameNoSpace, testParamGenerator, getTestParamName);
  registerTest<std::is_move_assignable_v<TestedType>, FixtureType, MoveAssignTest<FixtureType, TestedType>>(
      typeNameNoSpace, testParamGenerator, getTestParamName);
  // registerTest<std::invocable<TestedType>, FixtureType, FunctorTest<FixtureType, TestedType>>(typeNameNoSpacetestParamGenerator,  getTestParamName);
  registerTest<isConvertibleToBool, FixtureType, ConvertibleBoolTest<FixtureType, TestedType>>(typeNameNoSpace, testParamGenerator, getTestParamName);
  registerTest<std::equality_comparable<TestedType>, FixtureType, EqualCmpTest<FixtureType, TestedType>>(typeNameNoSpace, testParamGenerator, getTestParamName);
  registerTest<isOrdered, FixtureType, OrderingCmpTest<FixtureType, TestedType>>(typeNameNoSpace, testParamGenerator, getTestParamName);
  registerTest<std::swappable<TestedType>, FixtureType, SwapTest<FixtureType, TestedType>>(typeNameNoSpace, testParamGenerator, getTestParamName);
}

struct Empty
{};

template<template<typename TestedType> typename TestFixtureType, auto... TypedTestDescList>
struct RegistratorCommonTests
{
  RegistratorCommonTests()
  {
    (registerTests<TestFixtureType, TypedTestDescList>(), ...);
  }

  RegistratorCommonTests(const auto& testParamGenerator, const auto& getTestParamName)
  {
    (registerTests<TestFixtureType, TypedTestDescList>(testParamGenerator, getTestParamName), ...);
  }
};

#define TEST_TYPE(TestedType)              \
  TestUtilities::TypedTestDesc<TestedType> \
  {                                        \
    #TestedType                            \
  }

} // namespace TestUtilities
