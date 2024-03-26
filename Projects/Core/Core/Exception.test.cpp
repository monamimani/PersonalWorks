
#include <string>

#include "fmt/format.h"

#include "Core/Exception.h"

#include "TestUtilities/GoogleTest.h"

namespace CoreTests
{
  struct Foo
  {
    uint32_t m_value{42};
  };


TEST(ExceptionType, DataException)
{
  using FooError = Core::Exception<Foo>;

  auto foo = Foo{};
  const auto msg = "Error";
  auto error = FooError{foo, msg};

  ASSERT_STRCASEEQ(error.what().c_str(), msg);

  ASSERT_EQ(error.data().m_value, 42);

  error.data().m_value++;
  ASSERT_EQ(error.data().m_value, 43);

  const auto suffix = " Bar";
  error.what() += suffix;

  const auto errorMsg = fmt::format("{}{}", msg, suffix);
  ASSERT_STRCASEEQ(error.what().c_str(), errorMsg.c_str());
}

TEST(ExceptionType, VoidException)
{
  using VoidError = Core::Exception;

  const auto msg = "Error";
  auto error = VoidError{msg};

  ASSERT_STRCASEEQ(error.what().c_str(), msg);

  const auto suffix = " Bar";
  error.what() += suffix;

  const auto errorMsg = fmt::format("{}{}", msg, suffix);
  ASSERT_STRCASEEQ(error.what().c_str(), errorMsg.c_str());
}
}
