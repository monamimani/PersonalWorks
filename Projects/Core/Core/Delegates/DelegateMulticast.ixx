module;
#include <memory>
#include <vector>

export module DelegateMulticast;

import CoreConcepts;
import StaticFunction;

namespace Delegate
{

export template <typename StaticFunction_T>
struct [[nodiscard]] DelegateMulticastRAII
{
public:
  DelegateMulticastRAII() noexcept = default;

  explicit DelegateMulticastRAII(std::shared_ptr<StaticFunction_T>&& staticFunction) noexcept
  : m_staticFunction{std::move(staticFunction)}
  {
  }

  ~DelegateMulticastRAII()
  {
    unbind();
  }

  DelegateMulticastRAII(const DelegateMulticastRAII& other) = delete;
  DelegateMulticastRAII(DelegateMulticastRAII&& other) noexcept
  {
    m_staticFunction = std::move(other.m_staticFunction);
    other.m_staticFunction.reset();
  }

  DelegateMulticastRAII& operator=(const DelegateMulticastRAII&) = delete;
  DelegateMulticastRAII& operator=(DelegateMulticastRAII&& other) noexcept
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

export template <typename Signature>
class DelegateMulticast;

export template <typename Ret, typename... Args>
class DelegateMulticast<Ret(Args...)> final
{
  static_assert(std::is_same_v<Ret, void>, "Delegate return values other than void are not supported.");

public:
  using Function_Sig = Ret(Args...);
  using StaticFunction_T = Core::StaticFunction<Function_Sig>;

  using DelegateRAII = DelegateMulticastRAII<StaticFunction_T>;

  //  MulticastDelegate(const MulticastDelegate& other)
  //{
  //  // This is doing a deep copy
  //  std::lock_guard<std::mutex> lockGuard(other.m_mutex);

  //  for (const auto elem : other.m_delegateList)
  //  {
  //    m_delegateList.push_back(std::make_shared<Delegate>(*elem));
  //  }
  //}

  // MulticastDelegate(MulticastDelegate&& other)
  //{
  //   std::lock_guard<std::mutex> lockGuard(other.m_mutex);
  //   m_delegateList = std::move(other.m_delegateList);
  //   other.m_delegateList.clear();
  // }

  template <typename Instance_T>
  using MemberFunction_Ptr = Ret (Instance_T::*)(Args...);
  template <typename Instance_T>
  using MemberFunctionConst_Ptr = Ret (Instance_T::*)(Args...) const;

  template <typename Instance_T>
  static consteval decltype(auto) asFnPtr(MemberFunction_Ptr<Instance_T> fct)
  {
    return fct;
  }

  template <typename Instance_T>
  static consteval decltype(auto) asFnConstPtr(MemberFunctionConst_Ptr<Instance_T> fct)
  {
    return fct;
  }

  template <Core::InvocableAndReturn<Ret, Args...> auto function>
  inline constexpr [[nodiscard]] auto bind()
  {
    auto staticFunction = std::make_shared<StaticFunction_T>();
    staticFunction->bind<function>();

    m_staticFunctionList.emplace_back(staticFunction);
    return DelegateMulticastRAII{std::move(staticFunction)};
  }

  template <Core::FunctorAndReturn<Ret, Args...> Instance_T>
  inline constexpr [[nodiscard]] auto bind(Instance_T&& functor)
  {
    auto staticFunction = std::make_shared<StaticFunction_T>();
    staticFunction->bind(std::forward<Instance_T>(functor));

    m_staticFunctionList.emplace_back(staticFunction);
    return DelegateMulticastRAII{std::move(staticFunction)};
  }

  template <auto function, typename Instance_T>
  requires Core::InvocableAndReturn<decltype(function), Ret, Instance_T, Args...>
  inline constexpr [[nodiscard]] auto bind(Instance_T&& instance)
  {
    auto staticFunction = std::make_shared<StaticFunction_T>();
    staticFunction->bind<function>(std::forward<Instance_T>(instance));

    m_staticFunctionList.emplace_back(staticFunction);
    return DelegateMulticastRAII{std::move(staticFunction)};
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