module;

#include <concepts>
#include <functional>
#include <memory>
#include <optional>
#include <type_traits>
#include <utility>

export module Delegate;

import CoreConcepts;
import ErasedStorage;
import StaticFunction;

namespace Delegate
{

export template<typename Signature>
class Connection;

export template<typename Ret, typename... Args>
class [[nodiscard]] Connection<Ret(Args...)> final
{
  using Function_Sig = Ret(Args...);
  using StaticFunction_T = Core::StaticFunction<Function_Sig>;

public:
  Connection() noexcept = default;

  explicit Connection(std::shared_ptr<StaticFunction_T>&& staticFunction) noexcept
  : m_staticFunction{std::move(staticFunction)}
  {}

  ~Connection()
  {
    unbind();
  }

  Connection(const Connection& other) = delete;

  Connection(Connection&& other) noexcept
  {
    m_staticFunction = std::move(other.m_staticFunction);
    other.m_staticFunction.reset();
  }

  Connection& operator=(const Connection&) = delete;

  Connection& operator=(Connection&& other) noexcept
  {
    m_staticFunction = std::move(other.m_staticFunction);
    other.m_staticFunction.reset();
    return *this;
  }

  void unbind() noexcept
  {
    m_staticFunction.reset();
  }

private:
  std::shared_ptr<StaticFunction_T> m_staticFunction;
};

template<typename Ret, typename... Args>
Connection(std::shared_ptr<Core::StaticFunction<Ret(Args...)>>&&) -> Connection<Ret(Args...)>;

export template<typename Ret, typename... Args>
class DelegateBase
{
private:

protected:
  using Function_Sig = Ret(Args...);
  using StaticFunction_T = Core::StaticFunction<Function_Sig>;

  DelegateBase() = default;

public:
  using Connection = Connection<Function_Sig>;

  template<typename Instance_T>
  using MemberFunctionPtr = Ret (Instance_T::*)(Args...);
  template<typename Instance_T>
  using MemberFunctionConstPtr = Ret (Instance_T::*)(Args...) const;

  template<typename Instance_T>
  static consteval decltype(auto) asFnPtr(MemberFunctionPtr<Instance_T> fct)
  {
    return fct;
  }

  template<typename Instance_T>
  static consteval decltype(auto) asFnConstPtr(MemberFunctionConstPtr<Instance_T> fct)
  {
    return fct;
  }

private:
};

export template<typename Signature>
class Delegate;

/**
 * @brief
 *
 * lvalues are stored as pointer to the target object and their lifetime is thus not managed by the Delegate class
 * rvalues up to the size of the storage are allowed and are copied in the aligned storage. The delegate class therefore manages thier lifetime.
 * rvalues that are bigger than the size of the aligned storage I could allocate memory and store a pointer to it in the aligned storage.
 * @tparam Ret
 * @tparam ...Args
 */
export template<typename Ret, typename... Args>
class Delegate<Ret(Args...)> final: public DelegateBase<Ret, Args...>
{
private:
  using DelegateBase<Ret, Args...>::StaticFunction_T;

public:
  template<Core::InvocableAndReturn<Ret, Args...> auto function>
  [[nodiscard]] constexpr auto bind()
  {
    auto staticFunction = std::make_shared<StaticFunction_T>();
    staticFunction->bind<function>();
    m_staticFunction = std::weak_ptr<StaticFunction_T>{staticFunction};
    return Connection{std::move(staticFunction)};
  }

  template<Core::FunctorAndReturn<Ret, Args...> Instance_T>
  [[nodiscard]] constexpr auto bind(Instance_T&& functor)
  {
    auto staticFunction = std::make_shared<StaticFunction_T>();
    staticFunction->bind(std::forward<Instance_T>(functor));
    m_staticFunction = std::weak_ptr<StaticFunction_T>{staticFunction};
    return Connection{std::move(staticFunction)};
  }

  template<auto function, typename Instance_T>
  requires Core::InvocableAndReturnNTTP<function, Ret, Instance_T, Args...>
  [[nodiscard]] constexpr auto bind(Instance_T&& instance)
  {
    auto staticFunction = std::make_shared<StaticFunction_T>();
    staticFunction->bind<function>(std::forward<Instance_T>(instance));
    m_staticFunction = std::weak_ptr<StaticFunction_T>{staticFunction};
    return Connection{std::move(staticFunction)};
  }

  [[nodiscard]] constexpr bool isBound() const
  {
    return !m_staticFunction.expired();
  }

  constexpr void unbind() noexcept
  {
    m_staticFunction.reset();
  }

  constexpr void swap(Delegate& other)
  {
    using std::swap;

    swap(m_staticFunction, other.m_staticFunction);
  }

  [[nodiscard]] constexpr explicit operator bool() const noexcept
  {
    return isBound();
  }

  constexpr decltype(auto) operator()(Args&&... args)
  {
    return invoke(std::forward<Args>(args)...);
  }

  constexpr decltype(auto) invoke(Args&&... args)
  {
    if (auto staticFunction = m_staticFunction.lock())
    {
      return staticFunction->invoke(std::forward<Args>(args)...);
    }
  }

  constexpr decltype(auto) invokeSafe(Args&&... args)
  {
    if (auto staticFunction = m_staticFunction.lock())
    {
      return staticFunction->invokeSafe(std::forward<Args>(args)...);
    }

    return std::optional<Ret>{};
  }

  friend bool operator==(const Delegate& lhs, const Delegate& rhs)
  {
    return (lhs.m_staticFunction.expired() && rhs.m_staticFunction.expired()) || ((!lhs.m_staticFunction.expired() && !rhs.m_staticFunction.expired()) && (*lhs.m_staticFunction.lock() == *rhs.m_staticFunction.lock()));
  }

private:
  std::weak_ptr<StaticFunction_T> m_staticFunction;
};

export template<typename Signature>
inline void swap(Delegate<Signature>& d1, Delegate<Signature>& d2)
{
  d1.swap(d2);
}

} // namespace Delegate

module :private;
