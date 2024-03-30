#pragma once

#ifdef WIN32
#include <codeanalysis\warnings.h>

#pragma warning(push)
#pragma warning(disable : ALL_CODE_ANALYSIS_WARNINGS)
#include "benchmark/benchmark.h"
#pragma warning(pop)
#else
#include "benchmark/benchmark.h"
#endif