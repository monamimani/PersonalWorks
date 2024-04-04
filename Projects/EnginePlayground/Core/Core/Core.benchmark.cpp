#ifdef CORE_PLATFORM_WINDOWS
  #include <codeanalysis\warnings.h>
  #pragma warning(push)
  #pragma warning(disable : ALL_CODE_ANALYSIS_WARNINGS)
#endif
#include "benchmark/benchmark.h"
#ifdef CORE_PLATFORM_WINDOWS
  #pragma warning(pop)
#endif

#include <mutex>

void stdMutex(benchmark::State& state) // cppcheck-suppress constParameterCallback
{
  static std::mutex lock;
  benchmark::DoNotOptimize(lock);

  for (auto _ : state)
  {
    auto autoLock = std::scoped_lock{lock};
  }
}

BENCHMARK(stdMutex)->ThreadRange(1, 16);
