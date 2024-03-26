#pragma once

#include "TestUtilities/Config.h"

#define TEST_FIXTURE_FRIEND(TestFixtureType) \
  template<typename T>                       \
  friend class TestFixtureType

#define TEST_FIXTURE_FORWARD_DECLARE_NS(TestNamespace, TestFixtureType) \
  namespace TestNamespace                                               \
  {                                                                     \
  template<typename T>                                                  \
  class TestFixtureType;                                                \
  }

#define TEST_FIXTURE_FRIEND_NS(TestNamespace, TestFixtureType) \
  template<typename T>                                         \
  friend class TestNamespace::TestFixtureType

#define TEST_FRIEND_FORWARD_DECLARE_NS(TestNamespace, TestFixtureType, TestName) \
  namespace TestNamespace                                                        \
  {                                                                              \
  class TestFixtureType##_##TestName##_Test;                                     \
  }

#define TEST_FRIEND_NS(TestNamespace, TestFixtureType, TestName) friend class TestNamespace::TestFixtureType##_##TestName##_Test

namespace TestUtilities
{

class TestFriend
{
public:
  template<auto MemberPtr>
  requires isTestBuild
  static consteval const auto& get(const auto& obj)
  {
    return obj.*MemberPtr;
  }
};
} // namespace TestUtilities
