#include <gtest/gtest.h>
#include <utility>

// NOLINTBEGIN(*-magic-numbers,readability-identifier-length)

namespace TestUtilitiesTests
{
struct Sample
{
  [[nodiscard]] explicit operator bool() const
  {
    return m_value != 0;
  }

  void operator()() const
  {}

  auto operator<=>(const Sample&) const = default;

  int m_value = 0;

  friend void swap(Sample& lhs, Sample& rhs) noexcept
  {
    using std::swap;
    swap(lhs.m_value, rhs.m_value);
  }
};

TEST(SampleTest, DefaultConstructor)
{
  Sample s;
  EXPECT_EQ(s.m_value, 0);
  EXPECT_FALSE((bool)s);
}

TEST(SampleTest, ValueConstructor)
{
  Sample s{42};
  EXPECT_EQ(s.m_value, 42);
  EXPECT_TRUE((bool)s);
}

TEST(SampleTest, CopyConstructor)
{
  Sample s1{42};
  Sample s2{s1};
  EXPECT_EQ(s2.m_value, 42);
  EXPECT_EQ(s1.m_value, 42);
  EXPECT_EQ(s1, s2);
}

TEST(SampleTest, MoveConstructor)
{
  Sample s1{42};
  Sample s2{std::move(s1)};
  EXPECT_EQ(s2.m_value, 42);
  EXPECT_EQ(s1, s2); // Sample is just an int, move doesn't clear it unless we explicitly do so
}

TEST(SampleTest, CopyAssignment)
{
  Sample s1{42};
  Sample s2;
  s2 = s1;
  EXPECT_EQ(s2.m_value, 42);
  EXPECT_EQ(s1.m_value, 42);
  EXPECT_EQ(s1, s2);
}

TEST(SampleTest, MoveAssignment)
{
  Sample s1{42};
  Sample s2;
  s2 = std::move(s1);
  EXPECT_EQ(s2.m_value, 42);
  EXPECT_EQ(s1, s2);
}

TEST(SampleTest, Comparison)
{
  Sample s1{42};
  Sample s2{42};
  Sample s3{151};

  EXPECT_EQ(s1, s2);
  EXPECT_NE(s1, s3);
  EXPECT_LT(s1, s3);
  EXPECT_GT(s3, s1);
}

TEST(SampleTest, Swap)
{
  Sample s1{42};
  Sample s2{151};

  using std::swap;
  swap(s1, s2);

  EXPECT_EQ(s1.m_value, 151);
  EXPECT_EQ(s2.m_value, 42);
}

TEST(SampleTest, OperatorBool)
{
  Sample s0{0};
  Sample s1{42};

  EXPECT_FALSE((bool)s0);
  EXPECT_TRUE((bool)s1);
}

TEST(SampleTest, OperatorCall)
{
  Sample s;
  s(); // Just verify it compiles and runs
}

} // namespace TestUtilitiesTests

// NOLINTEND(*-magic-numbers,readability-identifier-length)
