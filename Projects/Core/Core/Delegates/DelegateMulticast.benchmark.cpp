#include <codeanalysis\warnings.h>


#include "TestUtilities/GoogleBenchmark.h"

#include "TestUtilities/TestStruct.benchmark.h"
using namespace Test;

import DelegateMulticast;

namespace Delegate
{

using FctSig = void(int&);
using DelegateMulticastCandidate = DelegateMulticast<void(int&)>;

//void globalFunction_DelegateCandidate(benchmark::State& state)
//{
//  auto delegate = DelegateCandidate();
//  delegate.Bind<&freeFunction>();
//  benchmark::DoNotOptimize(delegate);
//
//  int value = 0;
//  benchmark::DoNotOptimize(value);
//  // Code inside this loop is measured repeatedly
//  for (auto _ : state)
//  {
//    delegate(value);
//    benchmark::DoNotOptimize(value);
//    // benchmark::ClobberMemory();
//  }
//}
//BENCHMARK(globalFunctionOutOfLine_DelegateCandidate);

}