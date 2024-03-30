
#include <memory>

#include "Core/Pimpl.h"
#include "gtest/gtest.h"

namespace CoreUnitTest
{

struct ImplTest
{
  ImplTest() = default;
  virtual ~ImplTest() = default;

  ImplTest(const ImplTest&) = default;
  ImplTest& operator=(const ImplTest&) = default;

  ImplTest(int a)
  : m_a(a)
  {
  }

  virtual int getValue() { return m_a; }

  int m_a = 11;
};

struct ImplTestB : public ImplTest
{
  ImplTestB(int u)
  : m_u(u)
  {
  }

  int getValue() override { return m_u; }

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
  auto ptr = implTestToRelease.get();
  auto releasedPtr = implTestToRelease.release();
  EXPECT_EQ(releasedPtr, ptr);
  EXPECT_EQ(implTestToRelease.get(), nullptr);
  delete releasedPtr;

  EXPECT_TRUE((bool)implTestB);
  EXPECT_FALSE((bool)Core::ImplPtr<ImplTest>());

  auto hasher = std::hash<Core::ImplPtr<ImplTest>>();
  auto hash = hasher(implTestA);
  EXPECT_EQ(hash, std::hash<typename decltype(implTestA)::const_pointer>()(implTestA.get()));
}
} // namespace CoreUnitTest