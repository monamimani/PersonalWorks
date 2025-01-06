
#include "BuildConfig.hunit.h"

#include <print>

#include "TestUtilities/GoogleTest.h"

namespace BuildConfigTest
{

TEST(BuildConfig, IsDebugBuild)
{

  if constexpr (BuildConfig::isDebugBuild)
  {
    ASSERT_TRUE(BuildConfig::isDebugBuild);
  }
  else
  {
    ASSERT_TRUE(BuildConfig::isOptimisedBuild);
  }

#ifdef BUILDCONFIG_LIB_EXPORT
  std::println("---------------------------BUILDCONFIG_LIB_EXPORT");
#endif

#if BUILDCONFIG_MSVC == 1
  std::println("---------------------------BUILDCONFIG_MSVC= {}", BUILDCONFIG_MSVC);
#endif
}

TEST(BuildConfig, Compiler)
{

  if constexpr (BuildConfig::compiler == BuildConfig::Compiler::MSVC)
  {
    ASSERT_TRUE(BuildConfig::compiler == BuildConfig::Compiler::MSVC);
  }
  else if constexpr (BuildConfig::compiler == BuildConfig::Compiler::GCC)
  {
    ASSERT_TRUE(BuildConfig::compiler == BuildConfig::Compiler::GCC);
  }
  else if constexpr (BuildConfig::compiler == BuildConfig::Compiler::Clang)
  {
    ASSERT_TRUE(BuildConfig::compiler == BuildConfig::Compiler::Clang);
  }
}

} // namespace BuildConfigTest
