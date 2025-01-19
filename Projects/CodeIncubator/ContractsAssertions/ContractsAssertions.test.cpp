#include <cstdint>
#include <format>
#include <print>
#include <source_location>
#include <stacktrace>
#include <string_view>

#include "catch2/benchmark/catch_benchmark.hpp"
#include "catch2/catch_test_macros.hpp"

// The standard (C++26) doesn't provide a formatter for std::source_location for the moment, so we need to provide one.
// NOLINTBEGIN(cert-dcl58-cpp, readability-convert-member-functions-to-static]
template<>
struct std::formatter<std::source_location>: std::formatter<std::string_view>
{
  auto format(const std::source_location& location, format_context& ctx) const
  {
    return std::format_to(ctx.out(), "{}({},{}), function `{}`\n", location.file_name(), location.line(), location.column(), location.function_name());
  }
};
// NOLINTEND(cert-dcl58-cpp, readability-convert-member-functions-to-static]

#define CONTRACT_ASSERTION_ENABLE

// #define CONTRACT_ASSERTION_BEHAVIOR_TERMINATE
#define CONTRACT_ASSERTION_BEHAVIOR_LOG
//   #define CONTRACT_ASSERTION_BEHAVIOR_NO_OP

#ifdef CONTRACT_ASSERTION_ENABLE
  #if defined(CONTRACT_ASSERTION_BEHAVIOR_TERMINATE) + defined(CONTRACT_ASSERTION_BEHAVIOR_LOG) + defined(CONTRACT_ASSERTION_BEHAVIOR_NO_OP) > 1
    #error "Only one CONTRACT_ASSERTION_BEHAVIOR should be defined at a time."
  #elif !defined(CONTRACT_ASSERTION_BEHAVIOR_TERMINATE) && !defined(CONTRACT_ASSERTION_BEHAVIOR_LOG) && !defined(CONTRACT_ASSERTION_BEHAVIOR_NO_OP)
    #error "At least one CONTRACT_ASSERTION_BEHAVIOR must be defined."
  #endif
#else
  #if defined(CONTRACT_ASSERTION_BEHAVIOR_TERMINATE) + defined(CONTRACT_ASSERTION_BEHAVIOR_LOG) + defined(CONTRACT_ASSERTION_BEHAVIOR_NO_OP) > 0
    #error "No CONTRACT_ASSERTION_BEHAVIOR should be defined when CONTRACT_ASSERTION is not enabled."
  #endif
#endif // CONTRACT_ASSERTION_ENABLE

namespace ContractsAssertions
{
enum class Behavior : uint8_t
{
  NoOp,
  Terminate,
  Log,
};

#ifdef CONTRACT_ASSERTION_ENABLE
constexpr auto ContractsAssertionsEnabled = true;

  #ifdef CONTRACT_ASSERTION_BEHAVIOR_TERMINATE
constexpr auto Behavior = Behavior::Terminate;
  #endif // CONTRACT_ASSERTION_BEHAVIOR_TERMINATE

  #ifdef CONTRACT_ASSERTION_BEHAVIOR_LOG
constexpr auto Behavior = Behavior::Log;
  #endif // CONTRACT_ASSERTION_BEHAVIOR_LOG

  #ifdef CONTRACT_ASSERTION_BEHAVIOR_NO_OP
constexpr auto Behavior = Behavior::NoOp;
  #endif // CONTRACT_ASSERTION_BEHAVIOR_NO_OP

#else
constexpr auto ContractsAssertionsEnabled = false;

constexpr auto Behavior = Behavior::NoOp;

#endif // CONTRAC_ASSERTION_ENABLED

void contract_violation_handler([[maybe_unused]] std::string_view predicateBody,
                                [[maybe_unused]] const std::source_location& sourceLocation,
                                [[maybe_unused]] const std::stacktrace& stacktrace)
{
  // std::println("Contract body: {}", predicateBody);
  // std::println("Source Location: {}", sourceLocation);
  // std::println("Stackstrace: {}", stacktrace);

  if constexpr (Behavior == Behavior::Terminate)
  {
    std::terminate();
  }

  if constexpr (Behavior == Behavior::Log)
  {
    std::println("Contract failed!");
  }

  if constexpr (Behavior == Behavior::NoOp)
  {
  }
}

using ContractViolationHandlerPtr = decltype(&contract_violation_handler);
ContractViolationHandlerPtr contract_violation_handler_ptr = &contract_violation_handler;

void set_contract_violation_handler(ContractViolationHandlerPtr handler)
{
  contract_violation_handler_ptr = handler;
}

} // namespace ContractsAssertions

inline void check_contract(bool predicateResult,
                           std::string_view predicateBody = "",
                           const std::source_location& sourceLocation = std::source_location::current(),
                           const std::stacktrace& stacktrace = std::stacktrace::current())
{
  if (!predicateResult) [[unlikely]]
  {
    ContractsAssertions::contract_violation_handler_ptr(predicateBody, sourceLocation, stacktrace);
  }
}

#define ContractAssertion(predicateBody)                           \
  do                                                               \
  {                                                                \
    if constexpr (ContractsAssertions::ContractsAssertionsEnabled) \
    {                                                              \
      check_contract(                                              \
          [&] {                                                    \
            predicateBody                                          \
          }(),                                                     \
          #predicateBody);                                         \
    }                                                              \
  }                                                                \
  while (false)

#define Precondition(predicateBody) ContractAssertion(predicateBody)

#define Postcondition(postConditionName, predicateBody)                         \
  [[maybe_unused]] auto postConditionName = [&](auto&& returnValue) -> auto&& { \
    ContractAssertion(predicateBody);                                           \
    return std::forward<decltype(returnValue)>(returnValue);                    \
  }

#define PostconditionReturn(postConditionName, returnValue)        \
  do                                                               \
  {                                                                \
    if constexpr (ContractsAssertions::ContractsAssertionsEnabled) \
    {                                                              \
      return postConditionName(returnValue);                       \
    }                                                              \
    else                                                           \
    {                                                              \
      return returnValue;                                          \
    }                                                              \
  }                                                                \
  while (false)

void unitTestContractViolationHandler([[maybe_unused]] std::string_view predicateBody,
                                      [[maybe_unused]] const std::source_location& sourceLocation,
                                      [[maybe_unused]] const std::stacktrace& stacktrace)
{
  // std::println("Contract body: {}", predicateBody);
  // std::println("Source Location: {}", sourceLocation);
  // std::println("Stackstrace: {}", stacktrace);

  // FAIL(predicateBody);
}

int foo(int m_value)
{
  Postcondition(postA, return returnValue > 0;);

  Precondition({ return m_value == 42; });

  ContractAssertion(return m_value == 42;);

  check_contract([&] {
    return m_value > 0;
  }());

  PostconditionReturn(postA, m_value - 100);
}

TEST_CASE("Contracts", "[Contracts]")
{
  ContractsAssertions::set_contract_violation_handler(&unitTestContractViolationHandler);

  foo(42);

  SECTION("Success Path")
  {
    REQUIRE(true);
  }

  BENCHMARK("Contracts", i)
  {
    //  Precondition({ return i >= 0; });
    return i;
  };
}
