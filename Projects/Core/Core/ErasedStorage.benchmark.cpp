#include "GoogleBenchmark.h"
#include "TestUtilities/TestStruct.test.h"
using namespace Test;

import ErasedStorage;

namespace Core
{

using ErasedType = TestStruct;
using ErasedStorage_T = ErasedStorage<sizeof(ErasedType), alignof(ErasedType)>;

void erasedStorage_DefaultCtor(benchmark::State& state)
{
  for (auto _ : state)
  {
    ErasedStorage_T storage;
    benchmark::DoNotOptimize(storage);
    const auto* typedStorage = storage.construct<ErasedType>((uint8_t)51);
    benchmark::DoNotOptimize(typedStorage);
    // benchmark::ClobberMemory();
  }
}
BENCHMARK(erasedStorage_DefaultCtor);

void erasedStorage_CopyCtor(benchmark::State& state)
{
  ErasedStorage_T storage;
  benchmark::DoNotOptimize(storage);

  for (auto _ : state)
  {
    auto copyStorage(storage);

    benchmark::DoNotOptimize(copyStorage);
    // benchmark::ClobberMemory();
  }
}
BENCHMARK(erasedStorage_CopyCtor);

void erasedStorage_PtrConstruct(benchmark::State& state)
{
  TestStruct type;
  ErasedStorage<sizeof(TestStruct*), alignof(TestStruct*)> storage;
  benchmark::DoNotOptimize(storage);

  for (auto _ : state)
  {
    const auto* typedStorage = storage.construct<ErasedType*>(&type);
    benchmark::DoNotOptimize(typedStorage);
    // benchmark::ClobberMemory();
  }
}
BENCHMARK(erasedStorage_PtrConstruct);

} // namespace Core