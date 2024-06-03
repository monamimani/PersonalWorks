#include <coroutine>
#include <cstdint>
#include <exception>
#include <format>
#include <iostream>
#include <optional>
#include <utility>

#include "catch2/benchmark/catch_benchmark.hpp"
#include "catch2/catch_test_macros.hpp"

/// @brief Code sample from blog posts titled: C++ Coroutines Part 1: co_yield, co_return and a Prime Sieve and Part 2: co_await and Fizz Buzz written by
/// Nigeltao https://nigeltao.github.io/blog/2023/cpp-coro-part-1-yield-return-prime-sieve.html
/// https://nigeltao.github.io/blog/2023/cpp-coro-part-2-await-fizz-buzz.html

class Generator
{
public:
  class promise_type
  {
  public:
    Generator get_return_object()
    {
      return Generator(std::coroutine_handle<promise_type>::from_promise(*this));
    }

    static std::suspend_always initial_suspend()
    {
      return {};
    }

    static std::suspend_always final_suspend() noexcept
    {
      return {};
    }

    std::suspend_always yield_value(int value)
    {
      m_value = value;
      return {};
    }

    static void return_void()
    {}

    void unhandled_exception()
    {
      m_exception = std::current_exception();
    }

    int m_value;
    std::exception_ptr m_exception;
  };

  // This class is move-only. See
  // https://google.github.io/styleguide/cppguide.html#Copyable_Movable_Types
  Generator(Generator&& other)
  : m_cohandle{other.release_handle()}
  {}

  Generator& operator=(Generator&& other)
  {
    if (this != &other)
    {
      if (m_cohandle)
      {
        m_cohandle.destroy();
      }
      m_cohandle = other.release_handle();
    }
    return *this;
  }

  ~Generator()
  {
    if (m_cohandle)
    {
      m_cohandle.destroy();
    }
  }

  [[nodiscard]] std::optional<int> next()
  {
    if (!m_cohandle || m_cohandle.done())
    {
      return std::nullopt;
    }
    m_cohandle.resume();
    if (m_cohandle.done())
    {
      return std::nullopt;
    }
    return m_cohandle.promise().m_value;
  }

private:
  explicit Generator(const std::coroutine_handle<promise_type> cohandle)
  : m_cohandle{cohandle}
  {}

  std::coroutine_handle<promise_type> release_handle()
  {
    return std::exchange(m_cohandle, nullptr);
  }

  std::coroutine_handle<promise_type> m_cohandle;
};

Generator source(int end)
{
  for (int x = 2; x < end; x++)
  {
    co_yield x;
  }
}

Generator filter(Generator g, int prime)
{
  while (std::optional<int> optional_x = g.next())
  {
    int x = optional_x.value();
    if ((x % prime) != 0)
    {
      co_yield x;
    }
  }
}

TEST_CASE("CoroutineNigeltao", "[Coroutine]")
{
  Generator g = source(40);

  while (std::optional<int> optional_prime = g.next())
  {
    int prime = optional_prime.value();

    std::cout << std::format("{}\n", prime);
    g = filter(std::move(g), prime);
  }
}
