#include "TestUtilities/GoogleBenchmark.h"

#include <mutex>

namespace
{
void stdMutex(benchmark::State& state) // cppcheck-suppress constParameterCallback
{
  static std::mutex lock;
  benchmark::DoNotOptimize(lock);

  for ([[maybe_unused]] auto _ : state)
  {
    auto autoLock = std::scoped_lock{lock};
  }
}
} // namespace

constexpr auto g_minThread = 1;
constexpr auto g_maxThread = 16;
BENCHMARK(stdMutex)->ThreadRange(g_minThread, g_maxThread);
