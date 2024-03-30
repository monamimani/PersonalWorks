
#include <iostream>

#include "TestUtilities/GoogleBenchmark.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{

  std::cout << "\n--- Google Benchmark ---\n";
  ::benchmark::Initialize(&argc, argv);
  if (::benchmark::ReportUnrecognizedArguments(argc, argv))
  {
    return EXIT_FAILURE;
  }
  ::benchmark::RunSpecifiedBenchmarks();
  ::benchmark::Shutdown();

  return EXIT_SUCCESS;
}