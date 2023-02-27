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
class DelegateMulticastBase: public DelegateBase<Ret, Args...>
{
public:
protected:
  DelegateMulticastBase() = default;

private:
  bool m_calling = false;
  bool m_dirty = false;
};

export template<typename Signature>
class DelegateMulticast;

export template<typename Ret, typename... Args>
class DelegateMulticast<Ret(Args...)> final: public DelegateMulticastBase<Ret, Args...>
{
private:
  static_assert(std::is_same_v<Ret, void>, "Delegate return values other than void are not supported.");

  using DelegateBase<Ret, Args...>::StaticFunction_T;

public:
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
    return broadcast(std::forward<Args>(args)...);
  }

  // function names Broadcast fire execute
  void broadcast(const Args&&... args)
  {
    for (auto& staticFunction : m_staticFunctionList)
    {
      if (auto sf = staticFunction.lock())
      {
        sf->invoke(std::forward<Args>(args)...);
      }
    }
  }

  bool isEmpty() const
  {
    return m_staticFunctionList.empty();
  }

  constexpr explicit operator bool() const
  {
    return !isEmpty();
  }

  void unbindAll()
  {
    m_staticFunctionList.clear();
  }

  bool operator==(const DelegateMulticast&) const = default;

private:
  std::vector<std::weak_ptr<StaticFunction_T>> m_staticFunctionList;
};
} // namespace Delegate
