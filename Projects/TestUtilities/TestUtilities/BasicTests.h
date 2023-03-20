#pragma once

#include <algorithm>
#include <concepts>
#include <functional>
#include <source_location>
#include <span>
#include <string_view>
#include <type_traits>

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
  // NotEqual,
  LessEqual,
  LessThan,
  GreaterEqual,
  GreaterThan,
  Swap
};

class CommonTests: public testing::Test
{
public:
  // All of these optional, just like in regular macro usage.
  static void SetUpTestSuite()
  {}

  static void TearDownTestSuite()
  {}

  void SetUp() override
  {}

  void TearDown() override
  {}
};

template<typename TestFixtureT, typename TestedType>
class DefaultCtor: TestFixtureT, public CommonTests
{
public:
  explicit DefaultCtor()
  {}

  template<typename ParamT>
  explicit DefaultCtor(ParamT&& param)
  : TestFixtureT(std::forward<ParamT>(param))
  {}

  static consteval auto getTestName()
  {
    return "DefaultCtor";
  }

  static consteval auto getSourceLocation()
  {
    return std::source_location::current();
  }

  using TestFixtureT::testObj;

  void TestBody() override
  {
    static_assert(requires(const TestFixtureT& fixture, const TestedType& arg) {
      {
        testObj<BasicTestsObjOps::DefaultCtor>(arg)
      } -> std::same_as<testing::AssertionResult>;
    });

    TestedType obj;
    ASSERT_TRUE(testObj<BasicTestsObjOps::DefaultCtor>(obj));

    if constexpr (requires(const TestFixtureT& fixture, const TestedType& arg) {
                    {
                      testObj<BasicTestsObjOps::Dtor>(std::as_bytes(std::span{std::addressof(arg), 1}))
                    } -> std::same_as<testing::AssertionResult>;
                  })
    {
      obj.~TestedType();
      ASSERT_TRUE(testObj<BasicTestsObjOps::Dtor>(std::as_bytes(std::span{std::addressof(obj), 1})));
    }
  }

private:
};

template<typename TestFixtureT, typename TestedType>
class CopyCtor: TestFixtureT, public CommonTests
{
public:
  explicit CopyCtor()
  {}

  template<typename ParamT>
  explicit CopyCtor(ParamT&& param)
  : TestFixtureT(std::forward<ParamT>(param))
  {}

  static consteval auto getTestName()
  {
    return "CopyCtor";
  }

  static consteval auto getSourceLocation()
  {
    return std::source_location::current();
  }

  using TestFixtureT::testObj;

  void TestBody() override
  {
    static_assert(requires(const TestFixtureT& fixture, const TestedType& arg) {
      {
        testObj<BasicTestsObjOps::CopyCtor>(arg)
      } -> std::same_as<testing::AssertionResult>;
    });

    TestedType obj;
    auto objCopy{obj};
    ASSERT_TRUE(testObj<BasicTestsObjOps::DefaultCtor>(objCopy));

    if constexpr (requires(const TestFixtureT& fixture, const TestedType& arg) {
                    {
                      testObj<BasicTestsObjOps::Dtor>(std::as_bytes(std::span{std::addressof(arg), 1}))
                    } -> std::same_as<testing::AssertionResult>;
                  })
    {
      objCopy.~TestedType();
      ASSERT_TRUE(testObj<BasicTestsObjOps::Dtor>(std::as_bytes(std::span{std::addressof(objCopy), 1})));
    }
  }

private:
};

template<typename TestFixtureT, typename TestedType>
class MoveCtor: TestFixtureT, public CommonTests
{
public:
  explicit MoveCtor()
  {}

  template<typename ParamT>
  explicit MoveCtor(ParamT&& param)
  : TestFixtureT(std::forward<ParamT>(param))
  {}

  static consteval auto getTestName()
  {
    return "MoveCtor";
  }

  static consteval auto getSourceLocation()
  {
    return std::source_location::current();
  }

  using TestFixtureT::testObj;

  void TestBody() override
  {
    static_assert(requires(const TestFixtureT& fixture, const TestedType& arg) {
      {
        testObj<BasicTestsObjOps::MoveCtor>(arg)
      } -> std::same_as<testing::AssertionResult>;
    });

    TestedType obj;
    auto objMove{std::move(obj)};
    ASSERT_TRUE(testObj<BasicTestsObjOps::MoveCtor>(objMove));
    ASSERT_TRUE(testObj<BasicTestsObjOps::MovedFrom>(obj));

    if constexpr (requires(const TestFixtureT& fixture, const TestedType& arg) {
                    {
                      testObj<BasicTestsObjOps::Dtor>(std::as_bytes(std::span{std::addressof(arg), 1}))
                    } -> std::same_as<testing::AssertionResult>;
                  })
    {
      objMove.~TestedType();
      ASSERT_TRUE(testObj<BasicTestsObjOps::Dtor>(std::as_bytes(std::span{std::addressof(objMove), 1})));
    }
  }

private:
};

template<typename TestFixtureT, typename TestedType>
class CopyAssign: TestFixtureT, public CommonTests
{
public:
  explicit CopyAssign()
  {}

  template<typename ParamT>
  explicit CopyAssign(ParamT&& param)
  : TestFixtureT(std::forward<ParamT>(param))
  {}

  static consteval auto getTestName()
  {
    return "CopyAssign";
  }

  static consteval auto getSourceLocation()
  {
    return std::source_location::current();
  }

  using TestFixtureT::testObj;

  void TestBody() override
  {
    static_assert(requires(const TestFixtureT& fixture, const TestedType& arg) {
      {
        testObj<BasicTestsObjOps::CopyAssign>(arg)
      } -> std::same_as<testing::AssertionResult>;
    });

    TestedType obj;
    TestedType objCopy;
    objCopy = obj;
    ASSERT_TRUE(testObj<BasicTestsObjOps::CopyAssign>(objCopy));

    if constexpr (requires(const TestFixtureT& fixture, const TestedType& arg) {
                    {
                      testObj<BasicTestsObjOps::Dtor>(std::as_bytes(std::span{std::addressof(arg), 1}))
                    } -> std::same_as<testing::AssertionResult>;
                  })
    {
      objCopy.~TestedType();
      ASSERT_TRUE(testObj<BasicTestsObjOps::Dtor>(std::as_bytes(std::span{std::addressof(objCopy), 1})));
    }
  }

private:
};

template<typename TestFixtureT, typename TestedType>
class MoveAssign: TestFixtureT, public CommonTests
{
public:
  explicit MoveAssign()
  {}

  template<typename ParamT>
  explicit MoveAssign(ParamT&& param)
  : TestFixtureT(std::forward<ParamT>(param))
  {}

  static consteval auto getTestName()
  {
    return "MoveAssign";
  }

  static consteval auto getSourceLocation()
  {
    return std::source_location::current();
  }

  using TestFixtureT::testObj;

  void TestBody() override
  {
    static_assert(requires(const TestFixtureT& fixture, const TestedType& arg) {
      {
        testObj<BasicTestsObjOps::MoveAssign>(arg)
      } -> std::same_as<testing::AssertionResult>;
    });

    TestedType obj;
    TestedType objMove;
    objMove = std::move(obj);
    ASSERT_TRUE(testObj<BasicTestsObjOps::MoveAssign>(objMove));
    ASSERT_TRUE(testObj<BasicTestsObjOps::MovedFrom>(obj));

    if constexpr (requires(const TestFixtureT& fixture, const TestedType& arg) {
                    {
                      testObj<BasicTestsObjOps::Dtor>(std::as_bytes(std::span{std::addressof(arg), 1}))
                    } -> std::same_as<testing::AssertionResult>;
                  })
    {
      objMove.~TestedType();
      ASSERT_TRUE(testObj<BasicTestsObjOps::Dtor>(std::as_bytes(std::span{std::addressof(objMove), 1})));
    }
  }

private:
};

template<typename TestFixtureT, typename TestedType>
class Functor: TestFixtureT, public CommonTests
{
public:
  explicit Functor()
  {}

  template<typename ParamT>
  explicit Functor(ParamT&& param)
  : TestFixtureT(std::forward<ParamT>(param))
  {}

  static consteval auto getTestName()
  {
    return "Functor";
  }

  static consteval auto getSourceLocation()
  {
    return std::source_location::current();
  }

  using TestFixtureT::testObj;

  void TestBody() override
  {
    static_assert(requires(const TestFixtureT& fixture, const TestedType& arg) {
      {
        testObj<BasicTestsObjOps::Functor>(arg)
      } -> std::same_as<testing::AssertionResult>;
    });

    GTEST_SKIP_("Not sure how to implement BasicTestsObjOps::Functor");
  }

private:
};

template<typename TestFixtureT, typename TestedType>
class ConvertibleBool: TestFixtureT, public CommonTests
{
public:
  explicit ConvertibleBool()
  {}

  template<typename ParamT>
  explicit ConvertibleBool(ParamT&& param)
  : TestFixtureT(std::forward<ParamT>(param))
  {}

  static consteval auto getTestName()
  {
    return "ConvertibleBool";
  }

  static consteval auto getSourceLocation()
  {
    return std::source_location::current();
  }

  using TestFixtureT::testObj;

  void TestBody() override
  {
    static_assert(requires(const TestFixtureT& fixture, const TestedType& arg) {
      {
        testObj<BasicTestsObjOps::ConvertibleBool>(arg)
      } -> std::same_as<testing::AssertionResult>;
    });

    GTEST_SKIP_("Not sure how to implement BasicTestsObjOps::ConvertibleBool");
    // TestedType obj;
    // ASSERT_TRUE((bool)obj);
  }

private:
};

template<typename TestFixtureT, typename TestedType>
class EqualCmp: TestFixtureT, public CommonTests
{
public:
  explicit EqualCmp()
  {}

  template<typename ParamT>
  explicit EqualCmp(ParamT&& param)
  : TestFixtureT(std::forward<ParamT>(param))
  {}

  static consteval auto getTestName()
  {
    return "EqualCmp";
  }

  static consteval auto getSourceLocation()
  {
    return std::source_location::current();
  }

  using TestFixtureT::testObj;

  void TestBody() override
  {
    static_assert(requires(const TestFixtureT& fixture, const TestedType& arg) {
      {
        testObj<BasicTestsObjOps::Equal>(arg)
      } -> std::same_as<testing::AssertionResult>;
    });

    TestedType objA;
    TestedType objB;
    ASSERT_EQ(objA, objB);
  }

private:
};

template<typename Type>
struct TypedTestDesc
{
  using TestTypeT = Type;
  const char* m_typeName;
};

template<typename Type, typename TestParamT>
struct TypedTestParamDesc: TypedTestDesc<Type>
{
  TestParamT m_testParam;
};

template<bool enableTest, typename TestFixture>
void registerTest(const std::string& typeName)
{
  if constexpr (enableTest)
  {
    constexpr auto sourceLocation = TestFixture::getSourceLocation();
    testing::RegisterTest(("SpecialMemberFctTests/" + typeName).c_str(),
                          TestFixture::getTestName(),
                          typeName.c_str(),
                          nullptr,
                          sourceLocation.file_name(),
                          sourceLocation.line(),
                          // Important to use the fixture type as the return type here.
                          []() -> CommonTests* {
                            return new TestFixture{};
                          });
  }
}

template<typename TestFixtureT, auto TypedTestDesc>
void registerTests()
{
  using TestTypeT = decltype(TypedTestDesc)::TestTypeT;
  std::string typeNameNoSpace = TypedTestDesc.m_typeName;
  std::erase(typeNameNoSpace, ' ');

  static constexpr bool isConvertibleToBool = requires(const TestTypeT& obj) { (bool)obj; };

  registerTest<std::is_default_constructible_v<TestTypeT>, DefaultCtor<TestFixtureT, TestTypeT>>(typeNameNoSpace);
  registerTest<std::is_copy_constructible_v<TestTypeT>, CopyCtor<TestFixtureT, TestTypeT>>(typeNameNoSpace);
  registerTest<std::is_move_constructible_v<TestTypeT>, MoveCtor<TestFixtureT, TestTypeT>>(typeNameNoSpace);
  registerTest<std::is_copy_assignable_v<TestTypeT>, CopyAssign<TestFixtureT, TestTypeT>>(typeNameNoSpace);
  registerTest<std::is_move_assignable_v<TestTypeT>, MoveAssign<TestFixtureT, TestTypeT>>(typeNameNoSpace);
  registerTest<std::invocable<TestTypeT>, Functor<TestFixtureT, TestTypeT>>(typeNameNoSpace);
  registerTest<isConvertibleToBool, ConvertibleBool<TestFixtureT, TestTypeT>>(typeNameNoSpace);
  registerTest<std::equality_comparable<TestTypeT>, EqualCmp<TestFixtureT, TestTypeT>>(typeNameNoSpace);
}

template<auto... TypedTestDescList>
auto RegisterCommonTests()
{
  (registerTests<TypedTestDescList>(), ...);
  return 1;
}

template<typename TestFixtureT, auto... TypedTestDescList>
struct RegistratorCommonTests
{
  RegistratorCommonTests()
  {
    (registerTests<TestFixtureT, TypedTestDescList>(), ...);
  }
};

#define TEST_TYPE(Type)              \
  TestUtilities::TypedTestDesc<Type> \
  {                                  \
    #Type                            \
  }

} // namespace TestUtilities
