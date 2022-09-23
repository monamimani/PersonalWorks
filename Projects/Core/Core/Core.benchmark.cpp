#include <codeanalysis\warnings.h>
#pragma warning(push)
#pragma warning(disable : ALL_CODE_ANALYSIS_WARNINGS)
#include "benchmark/benchmark.h"
#pragma warning(pop)

#include <mutex>

void stdMutex(benchmark::State& state)
{
  static std::mutex lock;
  benchmark::DoNotOptimize(lock);

  for (auto _ : state)
  {
    auto autoLock = std::scoped_lock{lock};
  }
}
BENCHMARK(stdMutex)->ThreadRange(1, 16);