#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>

#include <stacktrace>
#include <source_location>
#include <print>

template<>
struct std::formatter<std::source_location>: std::formatter<std::string_view>
{
  auto format(const std::source_location& location, format_context& ctx) const
  {
    return std::format_to(ctx.out(), "{}({},{}), function `{}`\n", location.file_name(), location.line(), location.column(), location.function_name());
  }
};

//template<>
//struct std::formatter<std::stacktrace_entry>: std::formatter<std::string_view>
//{
//  auto format(const std::stacktrace_entry& entry, format_context& ctx) const
//  {
//    return std::format_to(ctx.out(), "{}({}): {}\n", entry.source_file(), entry.source_line(), entry.description());
//  }
//};

struct ContractViolationDesc
{
  std::source_location  m_sourceLocation;
  std::stacktrace m_stacktrace;
};

void contract_violation_handler(std::source_location sourceLocation = std::source_location::current(), std::stacktrace stacktrace = std::stacktrace::current())
{
  std::print("Source Location: {}", sourceLocation);
  std::print("Stackstrace: {}", stacktrace);
}

void foo()
{
  auto preconditionA = [&]() -> bool {
    return false;
  };

  if (!preconditionA())
  {
    contract_violation_handler();
  }

}


TEST_CASE("Contracts", "[Contracts]")
{
  foo();

  SECTION("Success Path")
  {
    REQUIRE(true);
  }
}
