
#include "BuildConfig.hunit.h"
//import BuildConfig;

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
  std::cout << "---------------------------BUILDCONFIG_LIB_EXPORT\n";
#endif

#if BUILDCONFIG_MSVC == 1
  std::cout << "---------------------------BUILDCONFIG_MSVC\n" << BUILDCONFIG_MSVC << "\n";
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