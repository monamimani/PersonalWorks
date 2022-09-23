
#include <utility>

#include "Core/Delegates/DelegateStdFunction.h"
#include "GoogleBenchmark.h"
#include "TestUtilities/TestStruct.benchmark.h"
using namespace Test;

import Delegate;

namespace Delegate
{

using FctSig = void(int&);

template <class Delegate_T>
void DelegateCompare_DefaultCtor(benchmark::State& state)
{
  for (auto _ : state)
  {
    auto delegate = Delegate_T{};
    benchmark::DoNotOptimize(delegate);
    // benchmark::ClobberMemory();
  }
}
BENCHMARK(DelegateCompare_DefaultCtor<DelegateStdFunction<FctSig>>);
BENCHMARK(DelegateCompare_DefaultCtor<Delegate<FctSig>>);

template <class Delegate_T>
void DelegateCompare_CopyCtorEmpty(benchmark::State& state)
{
  auto delegate = Delegate_T{};
  benchmark::DoNotOptimize(delegate);
  for (auto _ : state)
  {
    Delegate_T delegateCopy{delegate};
    benchmark::DoNotOptimize(delegateCopy);
  }
}
BENCHMARK(DelegateCompare_CopyCtorEmpty<DelegateStdFunction<FctSig>>);
BENCHMARK(DelegateCompare_CopyCtorEmpty<Delegate<FctSig>>);

template <class Delegate_T>
void DelegateCompare_CopyCtorLValue(benchmark::State& state)
{
  BMStruct bmStruct;
  benchmark::DoNotOptimize(bmStruct);

  auto delegate = Delegate_T{};
  benchmark::DoNotOptimize(delegate);
  delegate.bind(bmStruct);

  for (auto _ : state)
  {
    Delegate_T delegateCopy{delegate};
    benchmark::DoNotOptimize(delegateCopy);
  }
}
BENCHMARK(DelegateCompare_CopyCtorLValue<DelegateStdFunction<FctSig>>);
BENCHMARK(DelegateCompare_CopyCtorLValue<Delegate<FctSig>>);

template <class Delegate_T>
void DelegateCompare_CopyCtorRValue(benchmark::State& state)
{
  auto delegate = Delegate_T{};
  benchmark::DoNotOptimize(delegate);
  delegate.bind(BMStruct{});

  for (auto _ : state)
  {
    Delegate_T delegateCopy{delegate};
    benchmark::DoNotOptimize(delegateCopy);
  }
}
BENCHMARK(DelegateCompare_CopyCtorRValue<DelegateStdFunction<FctSig>>);
BENCHMARK(DelegateCompare_CopyCtorRValue<Delegate<FctSig>>);

template <class Delegate_T>
void DelegateCompare_MoveCtorEmpty(benchmark::State& state)
{
  auto delegate = Delegate_T{};
  benchmark::DoNotOptimize(delegate);
  for (auto _ : state)
  {
    Delegate_T delegateMove{std::move(delegate)};
    benchmark::DoNotOptimize(delegateMove);
  }
}
BENCHMARK(DelegateCompare_MoveCtorEmpty<DelegateStdFunction<FctSig>>);
BENCHMARK(DelegateCompare_MoveCtorEmpty<Delegate<FctSig>>);

template <class Delegate_T>
void DelegateCompare_MoveCtorLValue(benchmark::State& state)
{
  BMStruct bmStruct;
  benchmark::DoNotOptimize(bmStruct);

  auto delegate = Delegate_T{};
  benchmark::DoNotOptimize(delegate);
  delegate.bind(bmStruct);

  for (auto _ : state)
  {
    Delegate_T delegateMove{std::move(delegate)};
    benchmark::DoNotOptimize(delegateMove);
  }
}
BENCHMARK(DelegateCompare_MoveCtorLValue<DelegateStdFunction<FctSig>>);
BENCHMARK(DelegateCompare_MoveCtorLValue<Delegate<FctSig>>);

template <class Delegate_T>
void DelegateCompare_MoveCtorRValue(benchmark::State& state)
{
  auto delegate = Delegate_T{};
  benchmark::DoNotOptimize(delegate);
  delegate.bind(BMStruct{});

  for (auto _ : state)
  {
    Delegate_T delegateMove{std::move(delegate)};
    benchmark::DoNotOptimize(delegateMove);
  }
}
BENCHMARK(DelegateCompare_MoveCtorRValue<DelegateStdFunction<FctSig>>);
BENCHMARK(DelegateCompare_MoveCtorRValue<Delegate<FctSig>>);

template <class Delegate_T>
void DelegateCompare_CopyAssignEmpty(benchmark::State& state)
{
  auto delegate = Delegate_T{};
  benchmark::DoNotOptimize(delegate);
  for (auto _ : state)
  {
    auto delegateCopy = delegate;
    benchmark::DoNotOptimize(delegateCopy);
  }
}
BENCHMARK(DelegateCompare_CopyAssignEmpty<DelegateStdFunction<FctSig>>);
BENCHMARK(DelegateCompare_CopyAssignEmpty<Delegate<FctSig>>);

template <class Delegate_T>
void DelegateCompare_CopyAssignLValue(benchmark::State& state)
{
  BMStruct bmStruct;
  benchmark::DoNotOptimize(bmStruct);

  auto delegate = Delegate_T{};
  benchmark::DoNotOptimize(delegate);
  delegate.bind(bmStruct);

  for (auto _ : state)
  {
    auto delegateCopy = delegate;
    benchmark::DoNotOptimize(delegateCopy);
  }
}
BENCHMARK(DelegateCompare_CopyAssignLValue<DelegateStdFunction<FctSig>>);
BENCHMARK(DelegateCompare_CopyAssignLValue<Delegate<FctSig>>);

template <class Delegate_T>
void DelegateCompare_CopyAssignRValue(benchmark::State& state)
{
  auto delegate = Delegate_T{};
  benchmark::DoNotOptimize(delegate);
  delegate.bind(BMStruct{});

  for (auto _ : state)
  {
    auto delegateCopy = delegate;
    benchmark::DoNotOptimize(delegateCopy);
  }
}
BENCHMARK(DelegateCompare_CopyAssignRValue<DelegateStdFunction<FctSig>>);
BENCHMARK(DelegateCompare_CopyAssignRValue<Delegate<FctSig>>);

template <class Delegate_T>
void DelegateCompare_MoveAssignEmpty(benchmark::State& state)
{
  auto delegate = Delegate_T{};
  benchmark::DoNotOptimize(delegate);
  for (auto _ : state)
  {
    auto delegateMove = std::move(delegate);
    benchmark::DoNotOptimize(delegateMove);
  }
}
BENCHMARK(DelegateCompare_MoveAssignEmpty<DelegateStdFunction<FctSig>>);
BENCHMARK(DelegateCompare_MoveAssignEmpty<Delegate<FctSig>>);

template <class Delegate_T>
void DelegateCompare_MoveAssignLValue(benchmark::State& state)
{
  BMStruct bmStruct;
  benchmark::DoNotOptimize(bmStruct);

  auto delegate = Delegate_T{};
  benchmark::DoNotOptimize(delegate);
  delegate.bind(bmStruct);

  for (auto _ : state)
  {
    auto delegateMove = std::move(delegate);
    benchmark::DoNotOptimize(delegateMove);
  }
}
BENCHMARK(DelegateCompare_MoveAssignLValue<DelegateStdFunction<FctSig>>);
BENCHMARK(DelegateCompare_MoveAssignLValue<Delegate<FctSig>>);

template <class Delegate_T>
void DelegateCompare_MoveAssignRValue(benchmark::State& state)
{
  auto delegate = Delegate_T{};
  benchmark::DoNotOptimize(delegate);
  delegate.bind(BMStruct{});

  for (auto _ : state)
  {
    auto delegateMove = std::move(delegate);
    benchmark::DoNotOptimize(delegateMove);
  }
}
BENCHMARK(DelegateCompare_MoveAssignRValue<DelegateStdFunction<FctSig>>);
BENCHMARK(DelegateCompare_MoveAssignRValue<Delegate<FctSig>>);

template <class Delegate_T>
void DelegateCompare_SwapEmpty(benchmark::State& state)
{
  auto delegateA = Delegate_T{};
  benchmark::DoNotOptimize(delegateA);

  auto delegateB = Delegate_T{};
  benchmark::DoNotOptimize(delegateB);

  for (auto _ : state)
  {
    using std::swap;
    swap(delegateA, delegateB);

    benchmark::DoNotOptimize(delegateA);
    benchmark::DoNotOptimize(delegateB);

    // benchmark::ClobberMemory();
  }
}
BENCHMARK(DelegateCompare_SwapEmpty<DelegateStdFunction<FctSig>>);
BENCHMARK(DelegateCompare_SwapEmpty<Delegate<FctSig>>);

template <class Delegate_T>
void DelegateCompare_SwapLValue(benchmark::State& state)
{
  BMStruct structA;
  benchmark::DoNotOptimize(structA);

  BMStruct structB;
  benchmark::DoNotOptimize(structB);

  auto delegateA = Delegate_T{};
  delegateA.bind(structA);
  benchmark::DoNotOptimize(delegateA);

  auto delegateB = Delegate_T{};
  delegateA.bind(structB);
  benchmark::DoNotOptimize(delegateB);

  for (auto _ : state)
  {
    using std::swap;
    swap(delegateA, delegateB);

    benchmark::DoNotOptimize(delegateA);
    benchmark::DoNotOptimize(delegateB);

    // benchmark::ClobberMemory();
  }
}
BENCHMARK(DelegateCompare_SwapLValue<DelegateStdFunction<FctSig>>);
BENCHMARK(DelegateCompare_SwapLValue<Delegate<FctSig>>);

template <class Delegate_T>
void DelegateCompare_SwapRValue(benchmark::State& state)
{
  auto delegateA = Delegate_T{};
  delegateA.bind(BMStruct{});
  benchmark::DoNotOptimize(delegateA);

  auto delegateB = Delegate_T{};
  delegateA.bind(BMStruct{});
  benchmark::DoNotOptimize(delegateB);

  for (auto _ : state)
  {
    using std::swap;
    swap(delegateA, delegateB);

    benchmark::DoNotOptimize(delegateA);
    benchmark::DoNotOptimize(delegateB);

    // benchmark::ClobberMemory();
  }
}
BENCHMARK(DelegateCompare_SwapRValue<DelegateStdFunction<FctSig>>);
BENCHMARK(DelegateCompare_SwapRValue<Delegate<FctSig>>);

template <class Delegate_T>
void DelegateCompare_GlobalFunction(benchmark::State& state)
{
  auto delegate = Delegate_T();
  auto delegateRAII = delegate.bind<&freeFunction>();
  benchmark::DoNotOptimize(delegate);

  int value = 0;
  benchmark::DoNotOptimize(value);
  for (auto _ : state)
  {
    delegate(value);
    benchmark::DoNotOptimize(value);
    // benchmark::ClobberMemory();
  }
}
BENCHMARK(DelegateCompare_GlobalFunction<DelegateStdFunction<FctSig>>);
BENCHMARK(DelegateCompare_GlobalFunction<Delegate<FctSig>>);

template <class Delegate_T>
void DelegateCompare_GlobalFunctionOOL(benchmark::State& state)
{
  auto delegate = Delegate_T();
  auto delegateRAII = delegate.bind<&freeFunctionOutOfLine>();
  benchmark::DoNotOptimize(delegate);

  int value = 0;
  benchmark::DoNotOptimize(value);
  for (auto _ : state)
  {
    delegate(value);
    benchmark::DoNotOptimize(value);
    // benchmark::ClobberMemory();
  }
}
BENCHMARK(DelegateCompare_GlobalFunctionOOL<DelegateStdFunction<FctSig>>);
BENCHMARK(DelegateCompare_GlobalFunctionOOL<Delegate<FctSig>>);

template <class Delegate_T>
void DelegateCompare_Bind_Lambda(benchmark::State& state)
{
  auto lambda = lambdaGlobal;
  benchmark::DoNotOptimize(lambda);

  auto delegate = Delegate_T{};
  auto delegateRAII = delegate.bind(lambda);
  benchmark::DoNotOptimize(delegate);

  int value = 0;
  benchmark::DoNotOptimize(value);
  for (auto _ : state)
  {
    delegate(value);
    benchmark::DoNotOptimize(value);
    // benchmark::ClobberMemory();
  }
}
BENCHMARK(DelegateCompare_Bind_Lambda<DelegateStdFunction<FctSig>>);
BENCHMARK(DelegateCompare_Bind_Lambda<Delegate<FctSig>>);

template <class Delegate_T>
void DelegateCompare_Bind_LambdaRValue(benchmark::State& state)
{
  auto lambda = lambdaGlobal;
  benchmark::DoNotOptimize(lambda);

  auto delegate = Delegate_T{};
  auto delegateRAII = delegate.bind(std::move(lambda));
  benchmark::DoNotOptimize(delegate);

  int value = 0;
  benchmark::DoNotOptimize(value);
  for (auto _ : state)
  {
    delegate(value);
    benchmark::DoNotOptimize(value);
    // benchmark::ClobberMemory();
  }
}
BENCHMARK(DelegateCompare_Bind_LambdaRValue<DelegateStdFunction<FctSig>>);
BENCHMARK(DelegateCompare_Bind_LambdaRValue<Delegate<FctSig>>);

template <class Delegate_T>
void DelegateCompare_Bind_LambdaStatefull(benchmark::State& state)
{
  auto lambda = lambdaStatefullGlobal;
  benchmark::DoNotOptimize(lambda);

  auto delegate = Delegate_T{};
  auto delegateRAII = delegate.bind(lambda);
  benchmark::DoNotOptimize(delegate);

  int value = 0;
  benchmark::DoNotOptimize(value);
  for (auto _ : state)
  {
    delegate(value);
    benchmark::DoNotOptimize(value);
    // benchmark::ClobberMemory();
  }
}
BENCHMARK(DelegateCompare_Bind_LambdaStatefull<DelegateStdFunction<FctSig>>);
BENCHMARK(DelegateCompare_Bind_LambdaStatefull<Delegate<FctSig>>);

template <class Delegate_T>
void DelegateCompare_Bind_LambdaStatefullRValue(benchmark::State& state)
{
  auto lambda = lambdaStatefullGlobal;
  benchmark::DoNotOptimize(lambda);

  auto delegate = Delegate_T{};
  auto delegateRAII = delegate.bind(std::move(lambda));
  benchmark::DoNotOptimize(delegate);

  int value = 0;
  benchmark::DoNotOptimize(value);
  for (auto _ : state)
  {
    delegate(value);
    benchmark::DoNotOptimize(value);
    // benchmark::ClobberMemory();
  }
}
BENCHMARK(DelegateCompare_Bind_LambdaStatefullRValue<DelegateStdFunction<FctSig>>);
BENCHMARK(DelegateCompare_Bind_LambdaStatefullRValue<Delegate<FctSig>>);

template <class Delegate_T>
void DelegateCompare_Bind_Functor(benchmark::State& state)
{
  BMStruct bmStruct;
  benchmark::DoNotOptimize(bmStruct);

  auto delegate = Delegate_T{};
  auto delegateRAII = delegate.bind(bmStruct);
  benchmark::DoNotOptimize(delegate);

  int value = 0;
  benchmark::DoNotOptimize(value);
  for (auto _ : state)
  {
    delegate(value);
    benchmark::DoNotOptimize(value);
    // benchmark::ClobberMemory();
  }
}
BENCHMARK(DelegateCompare_Bind_Functor<DelegateStdFunction<FctSig>>);
BENCHMARK(DelegateCompare_Bind_Functor<Delegate<FctSig>>);

template <class Delegate_T>
void DelegateCompare_Bind_FunctorOOL(benchmark::State& state)
{
  BMStructOutOfLine bmStructOutOfLine;
  benchmark::DoNotOptimize(bmStructOutOfLine);

  auto delegate = Delegate_T{};
  auto delegateRAII = delegate.bind(bmStructOutOfLine);
  benchmark::DoNotOptimize(delegate);

  int value = 0;
  benchmark::DoNotOptimize(value);
  for (auto _ : state)
  {
    delegate(value);
    benchmark::DoNotOptimize(value);
    // benchmark::ClobberMemory();
  }
}
BENCHMARK(DelegateCompare_Bind_FunctorOOL<DelegateStdFunction<FctSig>>);
BENCHMARK(DelegateCompare_Bind_FunctorOOL<Delegate<FctSig>>);

template <class Delegate_T>
void DelegateCompare_Bind_FunctorRValue(benchmark::State& state)
{
  auto delegate = Delegate_T{};
  auto delegateRAII = delegate.bind(BMStruct{});
  benchmark::DoNotOptimize(delegate);

  int value = 0;
  benchmark::DoNotOptimize(value);
  for (auto _ : state)
  {
    delegate(value);
    benchmark::DoNotOptimize(value);
    // benchmark::ClobberMemory();
  }
}
BENCHMARK(DelegateCompare_Bind_FunctorRValue<DelegateStdFunction<FctSig>>);
BENCHMARK(DelegateCompare_Bind_FunctorRValue<Delegate<FctSig>>);

template <class Delegate_T>
void DelegateCompare_Bind_FunctorRValueOOL(benchmark::State& state)
{
  auto delegate = Delegate_T{};
  auto delegateRAII = delegate.bind(BMStructOutOfLine{});
  benchmark::DoNotOptimize(delegate);

  int value = 0;
  benchmark::DoNotOptimize(value);
  for (auto _ : state)
  {
    delegate(value);
    benchmark::DoNotOptimize(value);
    // benchmark::ClobberMemory();
  }
}
BENCHMARK(DelegateCompare_Bind_FunctorRValueOOL<DelegateStdFunction<FctSig>>);
BENCHMARK(DelegateCompare_Bind_FunctorRValueOOL<Delegate<FctSig>>);

template <class Delegate_T>
void DelegateCompare_BindNTTP_MemFn(benchmark::State& state)
{
  BMStruct bmStruct;
  benchmark::DoNotOptimize(bmStruct);

  auto delegate = Delegate_T{};
  auto delegateRAII = delegate.bind<&BMStruct::fct>(bmStruct);
  benchmark::DoNotOptimize(delegate);

  int value = 0;
  benchmark::DoNotOptimize(value);
  for (auto _ : state)
  {
    delegate(value);
    benchmark::DoNotOptimize(value);
    // benchmark::ClobberMemory();
  }
}
BENCHMARK(DelegateCompare_BindNTTP_MemFn<DelegateStdFunction<FctSig>>);
BENCHMARK(DelegateCompare_BindNTTP_MemFn<Delegate<FctSig>>);

template <class Delegate_T>
void DelegateCompare_BindNTTP_MemFnOOL(benchmark::State& state)
{
  BMStructOutOfLine bmStructOutOfLine;
  benchmark::DoNotOptimize(bmStructOutOfLine);

  auto delegate = Delegate_T{};
  auto delegateRAII = delegate.bind<&BMStructOutOfLine::fct>(bmStructOutOfLine);
  benchmark::DoNotOptimize(delegate);

  int value = 0;
  benchmark::DoNotOptimize(value);
  for (auto _ : state)
  {
    delegate(value);
    benchmark::DoNotOptimize(value);
    // benchmark::ClobberMemory();
  }
}
BENCHMARK(DelegateCompare_BindNTTP_MemFnOOL<DelegateStdFunction<FctSig>>);
BENCHMARK(DelegateCompare_BindNTTP_MemFnOOL<Delegate<FctSig>>);

template <class Delegate_T>
void DelegateCompare_BindNTTP_MemFnConst(benchmark::State& state)
{
  BMStruct bmStruct;
  benchmark::DoNotOptimize(bmStruct);

  auto delegate = Delegate_T{};
  auto delegateRAII = delegate.bind<&BMStruct::fctConst>(bmStruct);
  benchmark::DoNotOptimize(delegate);

  int value = 0;
  benchmark::DoNotOptimize(value);
  for (auto _ : state)
  {
    delegate(value);
    benchmark::DoNotOptimize(value);
    // benchmark::ClobberMemory();
  }
}
BENCHMARK(DelegateCompare_BindNTTP_MemFnConst<DelegateStdFunction<FctSig>>);
BENCHMARK(DelegateCompare_BindNTTP_MemFnConst<Delegate<FctSig>>);

template <class Delegate_T>
void DelegateCompare_BindNTTP_MemFnConstOOL(benchmark::State& state)
{
  BMStructOutOfLine bmStructOutOfLine;
  benchmark::DoNotOptimize(bmStructOutOfLine);

  auto delegate = Delegate_T{};
  auto delegateRAII = delegate.bind<&BMStructOutOfLine::fctConst>(bmStructOutOfLine);
  benchmark::DoNotOptimize(delegate);

  int value = 0;
  benchmark::DoNotOptimize(value);
  for (auto _ : state)
  {
    delegate(value);
    benchmark::DoNotOptimize(value);
    // benchmark::ClobberMemory();
  }
}
BENCHMARK(DelegateCompare_BindNTTP_MemFnConstOOL<DelegateStdFunction<FctSig>>);
BENCHMARK(DelegateCompare_BindNTTP_MemFnConstOOL<Delegate<FctSig>>);

template <class Delegate_T>
void DelegateCompare_BindNTTP_CastMemFnConstOverloaded(benchmark::State& state)
{
  BMStruct bmStruct;
  benchmark::DoNotOptimize(bmStruct);

  auto delegate = Delegate_T{};
  auto delegateRAII = delegate.bind<Delegate_T::template asMemFnPtr<BMStruct>(&BMStruct::fctConstOverloaded)>(bmStruct);
  benchmark::DoNotOptimize(delegate);

  int value = 0;
  benchmark::DoNotOptimize(value);
  for (auto _ : state)
  {
    delegate(value);
    benchmark::DoNotOptimize(value);
    // benchmark::ClobberMemory();
  }
}
BENCHMARK(DelegateCompare_BindNTTP_CastMemFnConstOverloaded<DelegateStdFunction<FctSig>>);
BENCHMARK(DelegateCompare_BindNTTP_CastMemFnConstOverloaded<Delegate<FctSig>>);

template <class Delegate_T>
void DelegateCompare_BindNTTP_CastMemFnConstOverloadedOOL(benchmark::State& state)
{
  BMStructOutOfLine bmStructOutOfLine;
  benchmark::DoNotOptimize(bmStructOutOfLine);

  auto delegate = Delegate_T{};
  auto delegateRAII = delegate.bind<Delegate_T::template asMemFnPtr<BMStructOutOfLine>(&BMStructOutOfLine::fctConstOverloaded)>(bmStructOutOfLine);
  benchmark::DoNotOptimize(delegate);

  int value = 0;
  benchmark::DoNotOptimize(value);
  // Code inside this loop is measured repeatedly
  for (auto _ : state)
  {
    delegate(value);
    benchmark::DoNotOptimize(value);
    // benchmark::ClobberMemory();
  }
}
BENCHMARK(DelegateCompare_BindNTTP_CastMemFnConstOverloadedOOL<DelegateStdFunction<FctSig>>);
BENCHMARK(DelegateCompare_BindNTTP_CastMemFnConstOverloadedOOL<Delegate<FctSig>>);

template <class Delegate_T>
void DelegateCompare_BindNTTP_CastMemFnConstOverloadedConst(benchmark::State& state)
{
  BMStruct bmStruct;
  benchmark::DoNotOptimize(bmStruct);

  auto delegate = Delegate_T{};
  auto delegateRAII = delegate.bind<Delegate_T::template asMemFnConstPtr<BMStruct>(&BMStruct::fctConstOverloaded)>(bmStruct);
  benchmark::DoNotOptimize(delegate);

  int value = 0;
  benchmark::DoNotOptimize(value);
  for (auto _ : state)
  {
    delegate(value);
    benchmark::DoNotOptimize(value);
    // benchmark::ClobberMemory();
  }
}
BENCHMARK(DelegateCompare_BindNTTP_CastMemFnConstOverloadedConst<DelegateStdFunction<FctSig>>);
BENCHMARK(DelegateCompare_BindNTTP_CastMemFnConstOverloadedConst<Delegate<FctSig>>);

template <class Delegate_T>
void DelegateCompare_BindNTTP_CastMemFnConstOverloadedConstOOL(benchmark::State& state)
{
  BMStructOutOfLine bmStructOutOfLine;
  benchmark::DoNotOptimize(bmStructOutOfLine);

  auto delegate = Delegate_T{};
  auto delegateRAII = delegate.bind<Delegate_T::template asMemFnConstPtr<BMStructOutOfLine>(&BMStructOutOfLine::fctConstOverloaded)>(bmStructOutOfLine);
  benchmark::DoNotOptimize(delegate);

  int value = 0;
  benchmark::DoNotOptimize(value);
  for (auto _ : state)
  {
    delegate(value);
    benchmark::DoNotOptimize(value);
    // benchmark::ClobberMemory();
  }
}
BENCHMARK(DelegateCompare_BindNTTP_CastMemFnConstOverloadedConstOOL<DelegateStdFunction<FctSig>>);
BENCHMARK(DelegateCompare_BindNTTP_CastMemFnConstOverloadedConstOOL<Delegate<FctSig>>);

template <class Delegate_T>
void DelegateCompare_BindNTTP_CastMemFnConstOverloadedRValue(benchmark::State& state)
{
  auto delegate = Delegate_T{};
  auto delegateRAII = delegate.bind<Delegate_T::template asMemFnPtr<BMStruct>(&BMStruct::fctConstOverloaded)>(BMStruct{});
  benchmark::DoNotOptimize(delegate);

  int value = 0;
  benchmark::DoNotOptimize(value);
  for (auto _ : state)
  {
    delegate(value);
    benchmark::DoNotOptimize(value);
    // benchmark::ClobberMemory();
  }
}
BENCHMARK(DelegateCompare_BindNTTP_CastMemFnConstOverloadedRValue<DelegateStdFunction<FctSig>>);
BENCHMARK(DelegateCompare_BindNTTP_CastMemFnConstOverloadedRValue<Delegate<FctSig>>);

template <class Delegate_T>
void DelegateCompare_BindNTTP_CastMemFnConstOverloadedRValueOOL(benchmark::State& state)
{
  auto delegate = Delegate_T{};
  auto delegateRAII = delegate.bind<Delegate_T::template asMemFnPtr<BMStructOutOfLine>(&BMStructOutOfLine::fctConstOverloaded)>(BMStructOutOfLine{});
  benchmark::DoNotOptimize(delegate);

  int value = 0;
  benchmark::DoNotOptimize(value);
  for (auto _ : state)
  {
    delegate(value);
    benchmark::DoNotOptimize(value);
    // benchmark::ClobberMemory();
  }
}
BENCHMARK(DelegateCompare_BindNTTP_CastMemFnConstOverloadedRValue<DelegateStdFunction<FctSig>>);
BENCHMARK(DelegateCompare_BindNTTP_CastMemFnConstOverloadedRValue<Delegate<FctSig>>);

template <class Delegate_T>
void DelegateCompare_BindNTTP_CastMemFnConstOverloadedConstRValue(benchmark::State& state)
{
  auto delegate = Delegate_T{};
  auto delegateRAII = delegate.bind<Delegate_T::template asMemFnConstPtr<BMStruct>(&BMStruct::fctConstOverloaded)>(BMStruct{});
  benchmark::DoNotOptimize(delegate);

  int value = 0;
  benchmark::DoNotOptimize(value);
  for (auto _ : state)
  {
    delegate(value);
    benchmark::DoNotOptimize(value);
    // benchmark::ClobberMemory();
  }
}
BENCHMARK(DelegateCompare_BindNTTP_CastMemFnConstOverloadedConstRValue<DelegateStdFunction<FctSig>>);
BENCHMARK(DelegateCompare_BindNTTP_CastMemFnConstOverloadedConstRValue<Delegate<FctSig>>);

template <class Delegate_T>
void DelegateCompare_BindNTTP_CastMemFnConstOverloadedConstRValueOOL(benchmark::State& state)
{
  auto delegate = Delegate_T{};
  auto delegateRAII = delegate.bind<Delegate_T::template asMemFnConstPtr<BMStructOutOfLine>(&BMStructOutOfLine::fctConstOverloaded)>(BMStructOutOfLine{});
  benchmark::DoNotOptimize(delegate);

  int value = 0;
  benchmark::DoNotOptimize(value);
  for (auto _ : state)
  {
    delegate(value);
    benchmark::DoNotOptimize(value);
    // benchmark::ClobberMemory();
  }
}
BENCHMARK(DelegateCompare_BindNTTP_CastMemFnConstOverloadedConstRValueOOL<DelegateStdFunction<FctSig>>);
BENCHMARK(DelegateCompare_BindNTTP_CastMemFnConstOverloadedConstRValueOOL<Delegate<FctSig>>);

template <class Delegate_T>
void DelegateCompare_BindObject_MemFn(benchmark::State& state)
{
  BMStruct bmStruct;
  benchmark::DoNotOptimize(bmStruct);

  auto delegate = Delegate_T{};
  auto delegateRAII = delegate.bindObject(bmStruct).memFn<&BMStruct::fct>();
  benchmark::DoNotOptimize(delegate);

  int value = 0;
  benchmark::DoNotOptimize(value);
  for (auto _ : state)
  {
    delegate(value);
    benchmark::DoNotOptimize(value);
    // benchmark::ClobberMemory();
  }
}
BENCHMARK(DelegateCompare_BindObject_MemFn<DelegateStdFunction<FctSig>>);
BENCHMARK(DelegateCompare_BindObject_MemFn<Delegate<FctSig>>);

template <class Delegate_T>
void DelegateCompare_BindObject_MemFnOOL(benchmark::State& state)
{
  BMStructOutOfLine bmStructOutOfLine;
  benchmark::DoNotOptimize(bmStructOutOfLine);

  auto delegate = Delegate_T{};
  auto delegateRAII = delegate.bindObject(bmStructOutOfLine).memFn<&BMStructOutOfLine::fct>();
  benchmark::DoNotOptimize(delegate);

  int value = 0;
  benchmark::DoNotOptimize(value);
  for (auto _ : state)
  {
    delegate(value);
    benchmark::DoNotOptimize(value);
    // benchmark::ClobberMemory();
  }
}
BENCHMARK(DelegateCompare_BindObject_MemFnOOL<DelegateStdFunction<FctSig>>);
BENCHMARK(DelegateCompare_BindObject_MemFnOOL<Delegate<FctSig>>);

template <class Delegate_T>
void DelegateCompare_BindObject_MemFnConst(benchmark::State& state)
{
  BMStruct bmStruct;
  benchmark::DoNotOptimize(bmStruct);

  auto delegate = Delegate_T{};
  auto delegateRAII = delegate.bindObject(bmStruct).memFn<&BMStruct::fctConst>();
  benchmark::DoNotOptimize(delegate);

  int value = 0;
  benchmark::DoNotOptimize(value);
  for (auto _ : state)
  {
    delegate(value);
    benchmark::DoNotOptimize(value);
    // benchmark::ClobberMemory();
  }
}
BENCHMARK(DelegateCompare_BindObject_MemFnConst<DelegateStdFunction<void(int&)>>);
BENCHMARK(DelegateCompare_BindObject_MemFnConst<Delegate<void(int&)>>);

template <class Delegate_T>
void DelegateCompare_BindObject_MemFnConstOOL(benchmark::State& state)
{
  BMStructOutOfLine bmStructOutOfLine;
  benchmark::DoNotOptimize(bmStructOutOfLine);

  auto delegate = Delegate_T{};
  auto delegateRAII = delegate.bindObject(bmStructOutOfLine).memFn<&BMStructOutOfLine::fctConst>();
  ;
  benchmark::DoNotOptimize(delegate);

  int value = 0;
  benchmark::DoNotOptimize(value);
  for (auto _ : state)
  {
    delegate(value);
    benchmark::DoNotOptimize(value);
    // benchmark::ClobberMemory();
  }
}
BENCHMARK(DelegateCompare_BindObject_MemFnConstOOL<DelegateStdFunction<FctSig>>);
BENCHMARK(DelegateCompare_BindObject_MemFnConstOOL<Delegate<FctSig>>);

template <class Delegate_T>
void DelegateCompare_BindObject_CastMemFnConstOverloaded(benchmark::State& state)
{
  BMStruct bmStruct;
  benchmark::DoNotOptimize(bmStruct);

  auto delegate = Delegate_T{};
  auto delegateRAII = delegate.bindObject(bmStruct).memFn<&BMStruct::fctConstOverloaded>();
  benchmark::DoNotOptimize(delegate);

  int value = 0;
  benchmark::DoNotOptimize(value);
  for (auto _ : state)
  {
    delegate(value);
    benchmark::DoNotOptimize(value);
    // benchmark::ClobberMemory();
  }
}
BENCHMARK(DelegateCompare_BindObject_CastMemFnConstOverloaded<DelegateStdFunction<FctSig>>);
BENCHMARK(DelegateCompare_BindObject_CastMemFnConstOverloaded<Delegate<FctSig>>);

template <class Delegate_T>
void DelegateCompare_BindObject_CastMemFnConstOverloadedOOL(benchmark::State& state)
{
  BMStructOutOfLine bmStructOutOfLine;
  benchmark::DoNotOptimize(bmStructOutOfLine);

  auto delegate = Delegate_T{};
  auto delegateRAII = delegate.bindObject(bmStructOutOfLine).memFn<&BMStructOutOfLine::fctConstOverloaded>();
  benchmark::DoNotOptimize(delegate);

  int value = 0;
  benchmark::DoNotOptimize(value);
  for (auto _ : state)
  {
    delegate(value);
    benchmark::DoNotOptimize(value);
    // benchmark::ClobberMemory();
  }
}
BENCHMARK(DelegateCompare_BindObject_CastMemFnConstOverloadedOOL<DelegateStdFunction<FctSig>>);
BENCHMARK(DelegateCompare_BindObject_CastMemFnConstOverloadedOOL<Delegate<FctSig>>);

template <class Delegate_T>
void DelegateCompare_BindObject_CastMemFnConstOverloadedConst(benchmark::State& state)
{
  BMStruct bmStruct;
  benchmark::DoNotOptimize(bmStruct);

  auto delegate = Delegate_T{};
  auto delegateRAII = delegate.bindObject(bmStruct).memFnConst<&BMStruct::fctConstOverloaded>();
  benchmark::DoNotOptimize(delegate);

  int value = 0;
  benchmark::DoNotOptimize(value);
  for (auto _ : state)
  {
    delegate(value);
    benchmark::DoNotOptimize(value);
    // benchmark::ClobberMemory();
  }
}
BENCHMARK(DelegateCompare_BindObject_CastMemFnConstOverloadedConst<DelegateStdFunction<FctSig>>);
BENCHMARK(DelegateCompare_BindObject_CastMemFnConstOverloadedConst<Delegate<FctSig>>);

template <class Delegate_T>
void DelegateCompare_BindObject_CastMemFnConstOverloadedConstOOL(benchmark::State& state)
{
  BMStructOutOfLine bmStructOutOfLine;
  benchmark::DoNotOptimize(bmStructOutOfLine);

  auto delegate = Delegate_T{};
  auto delegateRAII = delegate.bindObject(bmStructOutOfLine).memFnConst<&BMStructOutOfLine::fctConstOverloaded>();
  benchmark::DoNotOptimize(delegate);

  int value = 0;
  benchmark::DoNotOptimize(value);
  for (auto _ : state)
  {
    delegate(value);
    benchmark::DoNotOptimize(value);
    // benchmark::ClobberMemory();
  }
}
BENCHMARK(DelegateCompare_BindObject_CastMemFnConstOverloadedConstOOL<DelegateStdFunction<FctSig>>);
BENCHMARK(DelegateCompare_BindObject_CastMemFnConstOverloadedConstOOL<Delegate<FctSig>>);

template <class Delegate_T>
void DelegateCompare_BindObject_CastMemFnConstOverloadedRValue(benchmark::State& state)
{
  auto delegate = Delegate_T{};
  auto delegateRAII = delegate.bindObject(BMStruct{}).memFn<&BMStruct::fctConstOverloaded>();
  benchmark::DoNotOptimize(delegate);

  int value = 0;
  benchmark::DoNotOptimize(value);
  for (auto _ : state)
  {
    delegate(value);
    benchmark::DoNotOptimize(value);
    // benchmark::ClobberMemory();
  }
}
BENCHMARK(DelegateCompare_BindObject_CastMemFnConstOverloadedRValue<DelegateStdFunction<FctSig>>);
BENCHMARK(DelegateCompare_BindObject_CastMemFnConstOverloadedRValue<Delegate<FctSig>>);

template <class Delegate_T>
void DelegateCompare_BindObject_CastMemFnConstOverloadedRValueOOL(benchmark::State& state)
{
  auto delegate = Delegate_T{};
  auto delegateRAII = delegate.bindObject(BMStructOutOfLine{}).memFn<&BMStructOutOfLine::fctConstOverloaded>();
  benchmark::DoNotOptimize(delegate);

  int value = 0;
  benchmark::DoNotOptimize(value);
  for (auto _ : state)
  {
    delegate(value);
    benchmark::DoNotOptimize(value);
    // benchmark::ClobberMemory();
  }
}
BENCHMARK(DelegateCompare_BindObject_CastMemFnConstOverloadedRValueOOL<DelegateStdFunction<FctSig>>);
BENCHMARK(DelegateCompare_BindObject_CastMemFnConstOverloadedRValueOOL<Delegate<FctSig>>);

template <class Delegate_T>
void DelegateCompare_BindObject_CastMemFnConstOverloadedConstRValue(benchmark::State& state)
{
  auto delegate = Delegate_T{};
  auto delegateRAII = delegate.bindObject(BMStruct{}).memFnConst<&BMStruct::fctConstOverloaded>();
  benchmark::DoNotOptimize(delegate);

  int value = 0;
  benchmark::DoNotOptimize(value);
  for (auto _ : state)
  {
    delegate(value);
    benchmark::DoNotOptimize(value);
    // benchmark::ClobberMemory();
  }
}
BENCHMARK(DelegateCompare_BindObject_CastMemFnConstOverloadedConstRValue<DelegateStdFunction<FctSig>>);
BENCHMARK(DelegateCompare_BindObject_CastMemFnConstOverloadedConstRValue<Delegate<FctSig>>);

template <class Delegate_T>
void DelegateCompare_BindObject_CastMemFnConstOverloadedConstRValueOOL(benchmark::State& state)
{
  auto delegate = Delegate_T{};
  auto delegateRAII = delegate.bindObject(BMStructOutOfLine{}).memFnConst<&BMStructOutOfLine::fctConstOverloaded>();
  benchmark::DoNotOptimize(delegate);

  int value = 0;
  benchmark::DoNotOptimize(value);
  for (auto _ : state)
  {
    delegate(value);
    benchmark::DoNotOptimize(value);
    // benchmark::ClobberMemory();
  }
}
BENCHMARK(DelegateCompare_BindObject_CastMemFnConstOverloadedConstRValueOOL<DelegateStdFunction<FctSig>>);
BENCHMARK(DelegateCompare_BindObject_CastMemFnConstOverloadedConstRValueOOL<Delegate<FctSig>>);
} // namespace Delegate