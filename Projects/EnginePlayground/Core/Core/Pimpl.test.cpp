
#include "Core/Pimpl.h"

#include <memory>

#include "gtest/gtest.h"

// NOLINTBEGIN(*-magic-numbers,readability-identifier-length)

namespace CoreUnitTest
{

struct ImplTest
{
  ImplTest() = default;
  virtual ~ImplTest() = default;

  ImplTest(const ImplTest&) = default;
  ImplTest& operator=(const ImplTest&) = default;

  ImplTest(ImplTest&&) = delete;
  ImplTest& operator=(ImplTest&&) = delete;

  explicit ImplTest(int value)
  : m_a(value)
  {}

  virtual int getValue()
  {
    return m_a;
  }

  friend constexpr auto operator<=>(const ImplTest& lhs, const ImplTest& rhs) = default;

  friend constexpr bool operator==(const ImplTest& lhs, const ImplTest& rhs) = default;

  int m_a = 11;
};

struct ImplTestB: public ImplTest
{
  explicit ImplTestB(int u)
  : m_u(u)
  {}

  int getValue() override
  {
    return m_u;
  }

  int m_u = 0;
};

TEST(CoreImplPtr, ImplPtr_ConstructorsAssignments)
{
  EXPECT_EQ(Core::ImplPtr<ImplTest>().get(), nullptr);
  EXPECT_EQ(Core::ImplPtr<ImplTest>(nullptr).get(), nullptr);

  EXPECT_EQ(Core::makeImplPtr<ImplTest>()->m_a, 11);

  {
    auto implTest = Core::ImplPtr<ImplTest>(new ImplTest(22));
    EXPECT_EQ(implTest->m_a, 22);
  }

  {
    auto implTest = Core::makeImplPtr<ImplTest>(22);
    auto implTmp = Core::ImplPtr<ImplTest>(implTest);
    EXPECT_EQ(implTmp->m_a, 22);
  }

  {
    auto implTestA = Core::ImplPtr<ImplTest>();
    auto implTestB = Core::makeImplPtr<ImplTest>(22);
    implTestA = implTestB;
    EXPECT_EQ(implTestA->m_a, 22);
  }

  {
    auto implTestA = Core::makeImplPtr<ImplTest>(42);
    auto implTestB = Core::makeImplPtr<ImplTest>(22);
    implTestA = implTestB;
    EXPECT_EQ(implTestA->m_a, 22);
  }

  {
    auto implTest = Core::makeImplPtr<ImplTest>(22);
    auto implTmp = Core::ImplPtr<ImplTest>(std::move(implTest));
    EXPECT_EQ(implTmp->m_a, 22);
  }

  {
    auto implTestA = Core::ImplPtr<ImplTest>();
    auto implTestB = Core::makeImplPtr<ImplTest>(22);
    implTestA = std::move(implTestB);
    EXPECT_EQ(implTestA->m_a, 22);
  }

  {
    auto implTestB = Core::ImplPtr<ImplTest>(new ImplTestB(22));
    EXPECT_EQ(implTestB->getValue(), 22);
  }
}

TEST(CoreImplPtr, ImplPtr_Accessor)
{
  auto implTest = Core::makeImplPtr<ImplTest>(42);
  const auto implTestConst = Core::makeImplPtr<ImplTest>(42);

  EXPECT_EQ((*implTest).m_a, 42);
  EXPECT_EQ((*implTestConst).m_a, 42);

  EXPECT_EQ(implTest->m_a, 42);
  EXPECT_EQ(implTestConst->m_a, 42);

  EXPECT_EQ(implTest.get()->m_a, 42);
  EXPECT_EQ(implTestConst.get()->m_a, 42);
}

TEST(CoreImplPtr, ImplPtr_Utility)
{
  auto implTestA = Core::makeImplPtr<ImplTest>(42);
  auto implTestB = Core::makeImplPtr<ImplTest>(121);

  implTestA.swap(implTestB);
  EXPECT_EQ(implTestA->m_a, 121);
  EXPECT_EQ(implTestB->m_a, 42);

  Core::swap(implTestA, implTestB);
  EXPECT_EQ(implTestA->m_a, 42);
  EXPECT_EQ(implTestB->m_a, 121);

  auto implTestToRelease = Core::makeImplPtr<ImplTest>(42);
  auto* ptr = implTestToRelease.get();
  auto* releasedPtr = implTestToRelease.release();
  EXPECT_EQ(releasedPtr, ptr);
  EXPECT_EQ(implTestToRelease.get(), nullptr);
  delete releasedPtr; // NOLINT(cppcoreguidelines-owning-memory)

  EXPECT_TRUE((bool)implTestB);
  EXPECT_FALSE((bool)Core::ImplPtr<ImplTest>());

  auto hasher = std::hash<Core::ImplPtr<ImplTest>>();
  auto hash = hasher(implTestA);
  EXPECT_EQ(hash, std::hash<typename decltype(implTestA)::const_pointer>()(implTestA.get()));
}
} // namespace CoreUnitTest

// NOLINTEND(*-magic-numbers,readability-identifier-length)

//#include <print>
//#include <type_traits>
//
//#include "catch2/catch_session.hpp"
//#include "catch2/benchmark/catch_benchmark.hpp"
//#include "catch2/catch_test_macros.hpp"
//
//#include "Pimpl/Pimpl.h"
//
//#if defined(_MSC_VER)
//
//#define NO_UNIQUE_ADDRESS [[msvc::no_unique_address]]
//
//#else
//
//#define NO_UNIQUE_ADDRESS [[no_unique_address]]
//
//#endif
//
//struct Empty {};
//
//namespace details {
//
//struct NonMovable {
//   //protected:
//    NonMovable() = default;
//    ~NonMovable() = default;
//
//   public:
//    NonMovable(const NonMovable&) = default;
//    NonMovable& operator=(const NonMovable&) = default;
//
//    // Don't delete the move operation, they won't be implicitly declared because the copy ctr is user declared
//    // The distinction is important for the concept std::copy_constructible
//    // not declared != deleted 
//    // NonMovable(NonMovable&&) = delete;
//    // NonMovable& operator=(NonMovable&&) = delete;
//};
//
//struct NonCopyable {
//   protected:
//    NonCopyable() = default;
//    ~NonCopyable() = default;
//
//   public:
//    NonCopyable(NonCopyable&&) = default;
//    NonCopyable& operator=(NonCopyable&&) = default;
//
//    NonCopyable(const NonCopyable&) = delete;
//    NonCopyable& operator=(const NonCopyable&) = delete;
//};
//
//struct NonCopyMoveable {
//   protected:
//    NonCopyMoveable() = default;
//    ~NonCopyMoveable() = default;
//
//   public:
//    NonCopyMoveable(NonCopyMoveable&&) = delete;
//    NonCopyMoveable& operator=(NonCopyMoveable&&) = delete;
//
//    NonCopyMoveable(const NonCopyMoveable&) = delete;
//    NonCopyMoveable& operator=(const NonCopyMoveable&) = delete;
//};
//
//struct Empty {};
//
//template <typename T = Empty>
//struct TestType : private T {
//    TestType() = default; 
//    explicit TestType(int value) : m_a(value) {}
//
//    TestType(const TestType&) = default;
//    TestType& operator=(const TestType&) = default;
//
//    TestType(TestType&&) = delete;
//    TestType& operator=(TestType&&) = delete;
//
//    int getValue() { return m_a; }
//
//    friend constexpr auto operator<=>(const TestType& lhs, const TestType& rhs)
//    {
//        return lhs.m_a <=> rhs.m_a;
//    }
//    friend constexpr bool operator==(const TestType& lhs, const TestType& rhs)
//    {
//        return lhs.m_a == rhs.m_a;
//    }
//
//    int m_a = 11;
//};
//}  // namespace details
//
//using TestTypeNonCopyable = details::TestType<details::NonCopyable>;
//using TestTypeNonMovable = details::TestType<details::NonMovable>;
//using TestTypeNonCopyMovable = details::TestType<details::NonCopyMoveable>;
//using TestType = details::TestType<>;
//
//TEST_CASE( "An example of an integration test case", "[.][IntegrationTest]" ) 
//{
//}
//
//// TEST_CASE( "TestType various type used for test utilities", "[UnitTest]" ) 
//// {
////     SECTION("TestTypeNonCopyable can't be copied")
////     {
////         auto testTypeA = TestTypeNonCopyable{};
////         auto testTypeB = TestTypeNonCopyable{testTypeA};
////         testTypeB = testTypeA;
////     }
//// }
//
//TEST_CASE( "ImplPtr for PIMPL but using a smart ptr", "[UnitTest]" ) 
//{
//    SECTION("Constructed by default or nullptr will be nullptr")
//    {
//        REQUIRE(Core::ImplPtr<TestType>{}.get() == nullptr);
//        REQUIRE(Core::ImplPtr<TestType>{nullptr}.get() == nullptr);
//    }
//
//    SECTION("Constructed from a ptr to the impl type will be valid")
//    {
//        auto implPtr = Core::ImplPtr<TestType>{new TestType(22)};
//        REQUIRE(implPtr.get() != nullptr);
//        REQUIRE(implPtr->m_a == 22);
//    }
//
//    SECTION("Constructed from makeImplPtr will be valid")
//    {
//        auto implPtr = Core::makeImplPtr<TestType>();
//        REQUIRE(implPtr.get() != nullptr);
//        REQUIRE(implPtr->m_a == 11);
//    }
//
//    SECTION("Constructed from makeImplPtr with a parameter will be valid")
//    {
//        auto implPtr = Core::makeImplPtr<TestType>(22);
//        REQUIRE(implPtr.get() != nullptr);
//        REQUIRE(implPtr->m_a == 22);
//    }
//
//    SECTION("Copy constructor operator")
//    {
//        auto implPtrA = Core::makeImplPtr<TestType>(42);
//        auto implPtrB = Core::ImplPtr<TestType>(implPtrA);
//        REQUIRE(implPtrA.get() != nullptr);
//        REQUIRE(implPtrB.get() != nullptr);
//        REQUIRE(implPtrA->m_a == 42);
//        REQUIRE(implPtrB->m_a == 42);
//    }
//
//    SECTION("Copy constructor operator with NonMovable (Copyable only) Impl type")
//    {
//        auto implPtrA = Core::makeImplPtr<TestTypeNonMovable>(42);
//        auto implPtrB = Core::ImplPtr<TestTypeNonMovable>(implPtrA);
//        REQUIRE(implPtrA.get() != nullptr);
//        REQUIRE(implPtrB.get() != nullptr);
//        REQUIRE(implPtrA->m_a == 42);
//        REQUIRE(implPtrB->m_a == 42);
//    }
//
//    SECTION("Copy assigment operator")
//    {
//        auto implPtrA = Core::makeImplPtr<TestType>(42);
//        auto implPtrB = Core::makeImplPtr<TestType>(22);
//        implPtrA = implPtrB;
//        REQUIRE(implPtrA.get() != nullptr);
//        REQUIRE(implPtrB.get() != nullptr);
//        REQUIRE(implPtrA->m_a == 22);
//        REQUIRE(implPtrB->m_a == 22);
//    }
//
//    SECTION("Copy assigment operator with a NonMovable (Copyable only) Impl type")
//    {
//        auto implPtrA = Core::makeImplPtr<TestTypeNonMovable>(42);
//        auto implPtrB = Core::makeImplPtr<TestTypeNonMovable>(22);
//        implPtrA = implPtrB;
//        REQUIRE(implPtrA.get() != nullptr);
//        REQUIRE(implPtrB.get() != nullptr);
//        REQUIRE(implPtrA->m_a == 22);
//        REQUIRE(implPtrB->m_a == 22);
//    }
//
//    SECTION("Move Constructor")
//    {
//        auto implPtrA = Core::makeImplPtr<TestType>(22);
//        auto implPtrB = Core::ImplPtr<TestType>{std::move(implPtrA)};
//        REQUIRE(implPtrA.get() == nullptr);
//        REQUIRE(implPtrB.get() != nullptr);
//        REQUIRE(implPtrB->m_a == 22);
//    }
//
//    SECTION("Move Constructor with a NonCopyable (Move only) type")
//    {
//        auto implPtrA = Core::makeImplPtr<TestTypeNonCopyable>(22);
//        auto implPtrB = Core::ImplPtr<TestTypeNonCopyable>{std::move(implPtrA)};
//        REQUIRE(implPtrA.get() == nullptr);
//        REQUIRE(implPtrB.get() != nullptr);
//        REQUIRE(implPtrB->m_a == 22);
//    }
//
//    SECTION("Move Constructor with a NonMovable (Copy only) type")
//    {
//        auto implPtrA = Core::makeImplPtr<TestTypeNonMovable>(22);
//        auto implPtrB = Core::ImplPtr<TestTypeNonMovable>{std::move(implPtrA)};
//        REQUIRE(implPtrA.get() == nullptr);
//        REQUIRE(implPtrB.get() != nullptr);
//        REQUIRE(implPtrB->m_a == 22);
//    }
//
//    SECTION("Move assigment operator")
//    {
//        auto implPtrA = Core::makeImplPtr<TestType>(42);
//        auto implPtrB = Core::makeImplPtr<TestType>(22);
//        implPtrA = std::move(implPtrB);
//        REQUIRE(implPtrA.get() != nullptr);
//        REQUIRE(implPtrB.get() == nullptr);
//        REQUIRE(implPtrA->m_a == 22);
//    }
//
//    SECTION("Move assigment operator with a NonCopyable (Move only) Impl type")
//    {
//        auto implPtrA = Core::makeImplPtr<TestTypeNonCopyable>(42);
//        auto implPtrB = Core::makeImplPtr<TestTypeNonCopyable>(22);
//        implPtrA = std::move(implPtrB);
//        REQUIRE(implPtrA.get() != nullptr);
//        REQUIRE(implPtrB.get() == nullptr);
//        REQUIRE(implPtrA->m_a == 22);
//    }
//
//    SECTION("Move assigment operator with a NonMoveable (Copyable only) Impl type")
//    {
//        auto implPtrA = Core::makeImplPtr<TestTypeNonMovable>(42);
//        auto implPtrB = Core::makeImplPtr<TestTypeNonMovable>(22);
//        implPtrA = std::move(implPtrB);
//        REQUIRE(implPtrA.get() != nullptr);
//        REQUIRE(implPtrB.get() == nullptr);
//        REQUIRE(implPtrA->m_a == 22);
//    }
//
//    auto implTest = Core::makeImplPtr<TestType>(42);
//    const auto implTestConst = Core::makeImplPtr<TestType>(42);
//
//    SECTION("Can be dereferenced by operator*")
//    {
//        REQUIRE((*implTest).m_a == 42);
//        REQUIRE((*implTestConst).m_a == 42);
//    }
//
//    SECTION("Can be dereferenced by operator->")
//    {
//        REQUIRE(implTest->m_a == 42);
//        REQUIRE(implTestConst->m_a == 42);
//    }
//
//    SECTION("Accessor get return the correct pointer to obj")
//    {
//        REQUIRE(implTest.get()->m_a == 42);
//        REQUIRE(implTestConst.get()->m_a == 42);
//    }
//
//    SECTION("Member swap fct swap objects")
//    {
//        auto implTestA = Core::makeImplPtr<TestType>(42);
//        auto implTestB = Core::makeImplPtr<TestType>(121);
//        implTestA.swap(implTestB);
//        REQUIRE(implTestA->m_a == 121);
//        REQUIRE(implTestB->m_a == 42);
//    }
//
//    SECTION("free fct swap swap objects")
//    {
//        auto implTestA = Core::makeImplPtr<TestType>(42);
//        auto implTestB = Core::makeImplPtr<TestType>(121);
//        Core::swap(implTestA, implTestB);
//        REQUIRE(implTestA->m_a == 121);
//        REQUIRE(implTestB->m_a == 42);
//    }
//
//    SECTION("release function release the ownership of the obj")
//    {
//        auto implTestToRelease = Core::makeImplPtr<TestType>(42);
//        auto* ptr = implTestToRelease.get();
//        auto* releasedPtr = implTestToRelease.release();
//        REQUIRE(releasedPtr == ptr);
//        REQUIRE(implTestToRelease.get() == nullptr);
//        delete releasedPtr; // NOLINT(cppcoreguidelines-owning-memory)
//    }
//
//    SECTION("Explicit Conversion to bool")
//    {
//        auto implTest = Core::makeImplPtr<TestType>(42);
//        REQUIRE((bool)implTest == true);
//        REQUIRE((bool)Core::ImplPtr<TestType>() == false);
//    }
//
//    SECTION("Can be hashed")
//    {
//        auto implTest = Core::makeImplPtr<TestType>(42);
//        auto hasher = std::hash<Core::ImplPtr<TestType>>();
//        auto hash = hasher(implTest);
//        REQUIRE(hash == std::hash<typename decltype(implTest)::const_pointer>()(implTest.get()));
//        REQUIRE(hash == std::hash<decltype(implTest)>()(implTest));
//    }
//
//    SECTION("Can be equal compared")
//    {
//        auto implTestA = Core::makeImplPtr<TestType>(42);
//        auto implTestB = Core::makeImplPtr<TestType>(42);
//        REQUIRE(implTestA == implTestA);
//        REQUIRE(implTestA == implTestB);
//    }
//
//    SECTION("Can be non equale compared")
//    {
//        auto implTestA = Core::makeImplPtr<TestType>(42);
//        auto implTestB = Core::makeImplPtr<TestType>(22);
//        REQUIRE(implTestA != implTestB);
//    }
//
//    SECTION("Can be less compared")
//    {
//        auto implTestA = Core::makeImplPtr<TestType>(22);
//        auto implTestB = Core::makeImplPtr<TestType>(42);
//        REQUIRE(implTestA < implTestB);
//    }
//
//    SECTION("Can be less equal compared")
//    {
//        auto implTestA = Core::makeImplPtr<TestType>(22);
//        auto implTestB = Core::makeImplPtr<TestType>(22);
//        REQUIRE(implTestA <= implTestB);
//    }
//
//    SECTION("Can be greater compared")
//    {
//        auto implTestA = Core::makeImplPtr<TestType>(22);
//        auto implTestB = Core::makeImplPtr<TestType>(42);
//        REQUIRE(implTestB > implTestA);
//    }
//
//    SECTION("Can be greater equal compared")
//    {
//        auto implTestA = Core::makeImplPtr<TestType>(22);
//        auto implTestB = Core::makeImplPtr<TestType>(22);
//        REQUIRE(implTestB >= implTestA);
//    }
//}
//
//int main( int argc, char* argv[] ) {
//  Catch::Session session; // There must be exactly one instance
//
//  // writing to session.configData() here sets defaults
//  // this is the preferred way to set them
//
//  int returnCode = session.applyCommandLine( argc, argv );
//  if( returnCode != 0 ) // Indicates a command line error
//        return returnCode;
//
//  // writing to session.configData() or session.Config() here
//  // overrides command line args
//  // only do this if you know you need to
//
//  int numFailed = session.run();
//
//  // numFailed is clamped to 255 as some unices only use the lower 8 bits.
//  // This clamping has already been applied, so just return it here
//  // You can also do any post run clean-up here
//  return numFailed;
//}
