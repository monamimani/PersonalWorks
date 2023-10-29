#pragma once

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
