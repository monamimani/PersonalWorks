
#include <string>
#include "TestUtilities/GoogleTest.h"


// Google test sanitizer integration.
extern "C" {
void __ubsan_on_report() {
  FAIL() << "Encountered an undefined behavior sanitizer error";
}
void __asan_on_error() {
  FAIL() << "Encountered an address sanitizer error";
}
void __tsan_on_report() {
  FAIL() << "Encountered a thread sanitizer error";
}
}  // extern "C"


int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
  std::cout << "--- Google Test ---\n";

  auto cmdLine = std::vector<char*>(argv, argv + argc);
  
#ifdef _DEBUG
  std::string breakOnFaillure = "--gtest_break_on_failure";
  cmdLine.push_back(&breakOnFaillure[0]);
  
  std::string notCatchException = "--gtest_catch_exceptions=0";
  cmdLine.push_back(notCatchException.data());
#endif // _DEBUG
  cmdLine.emplace_back();

  int argCount = static_cast<int>(cmdLine.size() - 1);
  auto argsPtr = cmdLine.data();
  ::testing::InitGoogleTest(&argCount, argsPtr);
  if (RUN_ALL_TESTS() == EXIT_FAILURE)
  {
    return EXIT_FAILURE;
  }

  //std::cout << "\n--- Google Benchmark ---\n";
  //::benchmark::Initialize(&argc, argv);
  //if (::benchmark::ReportUnrecognizedArguments(argc, argv))
  //{
  //  return EXIT_FAILURE;
  //}
  //::benchmark::RunSpecifiedBenchmarks();
  //::benchmark::Shutdown();

  return EXIT_SUCCESS;
}