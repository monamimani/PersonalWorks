#include "TestUtilities/GoogleBenchmark.h"
#include "TestUtilities/TestStruct.benchmark.h"
using namespace Test;

//NOLINTBEGIN(readability-identifier-naming)
namespace
{

  void StaticFunctionBaseline_FunctionCall(benchmark::State& state)
  {
    int value = 0;
    benchmark::DoNotOptimize(value);
    // Code inside this loop is measured repeatedly
    for (auto _ : state) //NOLINT(clang-analyzer-deadcode.DeadStores)
    {
      freeFunction(value);
      benchmark::DoNotOptimize(value);
      // benchmark::ClobberMemory();
    }
  }

  void StaticFunctionBaseline_FunctionCallOutOfLine(benchmark::State& state)
  {
    int value = 0;
    benchmark::DoNotOptimize(value);
    // Code inside this loop is measured repeatedly
    for (auto _ : state) //NOLINT(clang-analyzer-deadcode.DeadStores)
    {
      freeFunctionOutOfLine(value);
      benchmark::DoNotOptimize(value);
      // benchmark::ClobberMemory();
    }
  }

  void StaticFunctionBaseline_FunctionPointerCall(benchmark::State& state)
  {
    void (*fct)(int&) = &freeFunction;
    benchmark::DoNotOptimize(fct);

    int value = 0;
    benchmark::DoNotOptimize(value);
    // Code inside this loop is measured repeatedly
    for (auto _ : state) //NOLINT(clang-analyzer-deadcode.DeadStores)
    {
      benchmark::DoNotOptimize(fct);
      (*fct)(value);
      benchmark::DoNotOptimize(value);
      // benchmark::ClobberMemory();
    }
  }

  void StaticFunctionBaseline_MemberFunctionCall(benchmark::State& state)
  {
    BMStruct bmStruct;
    benchmark::DoNotOptimize(bmStruct);

    int value = 0;
    benchmark::DoNotOptimize(value);
    // Code inside this loop is measured repeatedly
    for (auto _ : state) //NOLINT(clang-analyzer-deadcode.DeadStores)
    {
      bmStruct.fctConstOverloaded(value);
      benchmark::DoNotOptimize(value);
      // benchmark::ClobberMemory();
    }
  }

  void StaticFunctionBaseline_MemberFunctionOutOfLineCall(benchmark::State& state)
  {
    BMStructOutOfLine bmStructOutOfLine;
    benchmark::DoNotOptimize(bmStructOutOfLine);

    int value = 0;
    benchmark::DoNotOptimize(value);
    // Code inside this loop is measured repeatedly
    for (auto _ : state) //NOLINT(clang-analyzer-deadcode.DeadStores)
    {
      bmStructOutOfLine.fctConstOverloaded(value);
      benchmark::DoNotOptimize(value);
      // benchmark::ClobberMemory();
    }
  }

  void StaticFunctionBaseline_LambdaCall(benchmark::State& state)
  {
    auto lambda = lambdaGlobal;
    benchmark::DoNotOptimize(lambda);

    int value = 0;
    benchmark::DoNotOptimize(value);
    // Code inside this loop is measured repeatedly
    for (auto _ : state) //NOLINT(clang-analyzer-deadcode.DeadStores)
    {
      lambda(value);
      benchmark::DoNotOptimize(value);
      // benchmark::ClobberMemory();
    }
  }

  void StaticFunctionBaseline_StatefullLambdaCall(benchmark::State& state)
  {
    auto lambda = lambdaStatefullGlobal;
    benchmark::DoNotOptimize(lambda);

    int value = 0;
    benchmark::DoNotOptimize(value);
    // Code inside this loop is measured repeatedly
    for (auto _ : state) //NOLINT(clang-analyzer-deadcode.DeadStores)
    {
      lambda(value);
      benchmark::DoNotOptimize(value);
      // benchmark::ClobberMemory();
    }
  }

  BENCHMARK(StaticFunctionBaseline_FunctionCall);
  BENCHMARK(StaticFunctionBaseline_FunctionCallOutOfLine);
  BENCHMARK(StaticFunctionBaseline_FunctionPointerCall);
  BENCHMARK(StaticFunctionBaseline_MemberFunctionCall);
  BENCHMARK(StaticFunctionBaseline_MemberFunctionOutOfLineCall);
  BENCHMARK(StaticFunctionBaseline_LambdaCall);
  BENCHMARK(StaticFunctionBaseline_StatefullLambdaCall);
  }

  //NOLINTEND(readability-identifier-naming)
