module;
#include <memory>
#include <vector>

export module DelegateMulticast;

import CoreConcepts;
import StaticFunction;
import Delegate;

namespace Delegate
{

export template<typename Ret, typename... Args>
class DelegateMulticastBase
{
public:
protected:
  DelegateMulticastBase() = default;

private:
  // bool m_calling = false;
  // bool m_dirty = false;
};

export template<typename Signature>
class DelegateMulticast;

export template<typename Ret, typename... Args>
class DelegateMulticast<Ret(Args...)> final
{
private:
  static_assert(std::is_same_v<Ret, void>, "Delegate return values other than void are not supported.");

  using FctSig = Ret(Args...);
  using StaticFunction_T = Core::StaticFunction<FctSig>;

public:
  using Connection = Connection<FctSig>;

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

  template<Core::InvocableAndReturn<Ret, Args...> auto function>
  inline constexpr [[nodiscard]] auto bind()
  {
    auto staticFunction = std::make_shared<StaticFunction_T>();
    staticFunction->bind<function>();

    m_staticFunctionList.emplace_back(staticFunction);
    return Connection{std::move(staticFunction)};
  }

  template<Core::FunctorAndReturn<Ret, Args...> Instance_T>
  inline constexpr [[nodiscard]] auto bind(Instance_T&& functor)
  {
    auto staticFunction = std::make_shared<StaticFunction_T>();
    staticFunction->bind(std::forward<Instance_T>(functor));

    m_staticFunctionList.emplace_back(staticFunction);
    return Connection{std::move(staticFunction)};
  }

  template<auto function, typename Instance_T>
  requires Core::InvocableAndReturn<decltype(function), Ret, Instance_T, Args...>
  inline constexpr [[nodiscard]] auto bind(Instance_T&& instance)
  {
    auto staticFunction = std::make_shared<StaticFunction_T>();
    staticFunction->bind<function>(std::forward<Instance_T>(instance));

    m_staticFunctionList.emplace_back(staticFunction);
    return Connection{std::move(staticFunction)};
  }

  void operator()(const Args&&... args)
  {
    return invoke(std::forward<Args>(args)...);
  }

  // function names Broadcast fire execute
  void invoke(const Args&&... args)
  {
    for (auto& staticFunction : m_staticFunctionList)
    {
      if (auto sf = staticFunction.lock())
      {
        sf->invoke(std::forward<Args>(args)...);
      }
    }
  }

  [[nodiscard]] constexpr bool isEmpty() const
  {
    return m_staticFunctionList.empty();
  }

  [[nodiscard]] constexpr explicit operator bool() const
  {
    return !isEmpty();
  }

  constexpr void unbindAll()
  {
    m_staticFunctionList.clear();
  }


  friend bool operator==(const DelegateMulticast& lhs, const DelegateMulticast& rhs)
  {

    if (lhs.m_staticFunctionList.size() != rhs.m_staticFunctionList.size())
    {
      return false;
    }

    bool result = true;
    size_t size = lhs.m_staticFunctionList.size();
    for (size_t i = 0; i < size; i++)
    {
      const auto& wptrLhs = lhs.m_staticFunctionList[i];
      const auto& wptrRhs = rhs.m_staticFunctionList[i];

      result &= (wptrLhs.expired() && wptrRhs.expired()) || ((!wptrLhs.expired() && !wptrRhs.expired()) && (*wptrLhs.lock() == *wptrRhs.lock()));
      if (!result)
      {
        break;
      }
    }

    return result;
  }

private:
  std::vector<std::weak_ptr<StaticFunction_T>> m_staticFunctionList;
};
} // namespace Delegate
