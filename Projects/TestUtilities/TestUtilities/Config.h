#pragma once

#include "TestUtilities/ConfigGenerated.h"

namespace TestUtilities
{
#ifdef CORE_TESTS_ENABLED
constexpr auto areTestsEnabled = true;
#else
constexpr auto areTestsEnabled = false;
#endif // CORE_TESTS_ENABLED

#ifdef TEST_BUILD
constexpr auto isTestBuild = true;
#else
constexpr auto isTestBuild = false;
#endif

}
