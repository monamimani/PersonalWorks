#pragma once

#include <concepts>
#include <coroutine>
#include <cstdint>
#include <exception>
#include <expected>
#include <variant>

#include "Core/NonCopyable.h"

template<class ResultT, class... ErrorT>
class CoroResultErrorMgmt: Core::NonCopyable
{
  using ErrorVariantT = std::variant<ErrorT...>;
  using ValueT = std::expected<ResultT, ErrorVariantT>;

public:
  class PromiseType
  {
  public:
    CoroResultErrorMgmt get_return_object()
    {
      return CoroResultErrorMgmt(std::coroutine_handle<PromiseType>::from_promise(*this));
    }

    static std::suspend_never initial_suspend()
    {
      return {};
    }

    static std::suspend_always final_suspend() noexcept
    {
      return {};
    }

    template<std::convertible_to<ErrorVariantT> CurErrorT>
    std::suspend_always yield_value(CurErrorT&& currError)
    {
      m_result = std::unexpected(std::forward<CurErrorT>(currError));
      return {};
    }

    template<std::convertible_to<ValueT> From>
    void return_value(From&& from)
    {
      m_result = std::forward<From>(from);
    }

    void unhandled_exception()
    {
      m_exception = std::current_exception();
    }

  private:
    friend CoroResultErrorMgmt<ResultT, ErrorT...>;
    ValueT m_result;
    std::exception_ptr m_exception;
  };

  using promise_type = PromiseType;

  CoroResultErrorMgmt(CoroResultErrorMgmt&& other)
  : m_cohandle{other.release_handle()}
  {}

  CoroResultErrorMgmt& operator=(CoroResultErrorMgmt&& rhs)
  {
    if (this != &rhs)
    {
      if (m_cohandle)
      {
        m_cohandle.destroy();
      }
      m_cohandle = rhs.release_handle();
    }
    return *this;
  }

  ~CoroResultErrorMgmt()
  {
    if (m_cohandle)
    {
      m_cohandle.destroy();
    }
  }

  [[nodiscard]] bool hasValue() const
  {
    // I think this is not needed because of initial_suspend() ->  std::suspend_never
    // and this is not in a multi-threaded context.
    // m_cohandle && !m_cohandle.done();
    return m_cohandle.promise().m_result.has_value();
  }

  [[nodiscard]] ErrorVariantT& getError() const
  {
    return m_cohandle.promise().m_result.error();
  }

  void visit(auto visitor)
  requires requires { std::visit(visitor, std::declval<ErrorVariantT>()); }
  {
    std::visit(visitor, m_cohandle.promise().m_result.error());
    m_cohandle.resume();
  }

  [[nodiscard]] ResultT getResult() const
  {
    return m_cohandle.promise().m_result.value();
  }

  [[nodiscard]] ResultT getResult(auto visitor)
  requires requires { visit(visitor); }
  {
    while (!hasValue())
    {
      visit(visitor);
    }
    return m_cohandle.promise().m_result.value();
  }

protected:
  explicit CoroResultErrorMgmt(const std::coroutine_handle<promise_type> cohandle)
  : m_cohandle{cohandle}
  {}

  std::coroutine_handle<promise_type> release_handle()
  {
    return std::exchange(m_cohandle, nullptr);
  }

private:
  std::coroutine_handle<promise_type> m_cohandle;
};

// template<typename ResultT, typename... ErrorT>
// using CoroResultErrorMgmt = CoroResult<std::expected<ResultT, std::variant<ErrorT...>>>;
