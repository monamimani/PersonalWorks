#ifdef WIN32
#include <codeanalysis\warnings.h>

#pragma warning(push)
#pragma warning(disable : ALL_CODE_ANALYSIS_WARNINGS)
#include "gtest/gtest.h"
#pragma warning(pop)
#else
#include "gtest/gtest.h"
#endif



TEST(ApplicationTest, DummyTest)
{
  FAIL();
}