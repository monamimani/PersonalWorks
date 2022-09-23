module;
#include <vector>

export module DelegateMulticast;

import Core.Concepts;
import Delegate;

namespace Delegate
{

// Or DelegateHandle
// template <typename ReturnType>
// class DelegateMulticastRAII;
//
// template <typename ReturnType, typename... Args>
// class DelegateMulticastRAII<ReturnType(Args...)>
//{
//  using Delegate_t = Delegate<ReturnType, Args...>;
//
// public:
//  DelegateMulticastRAII() = default;
//  //~DelegateMulticastRAII()
//  //{
//  // This causes issues for temporaries.
//  // if (isValid())
//  //{
//  //  m_delegateList->remove(*this);
//  //}
//  //}
//  bool isValid() const
//  {
//    return !m_delegate.expired();
//  }
//  operator bool() const noexcept
//  {
//    isValid();
//  }
//
// private:
//  DelegateMulticastRAII(MulticastDelegate<ReturnType(Args...)>* delegateList, const std::weak_ptr<Delegate_t>& delegate)
//  : m_delegateList(delegateList)
//  , m_delegate(delegate)
//  {
//  }
//
//  std::weak_ptr<Delegate_t> m_delegate;
//  MulticastDelegate<ReturnType(Args...)>* m_delegateList = nullptr;
//
//  friend class MulticastDelegate<ReturnType(Args...)>;
//};

export template <typename Signature>
class DelegateMulticast;

export template <typename Ret, typename... Args>
class DelegateMulticast<Ret(Args...)> final
{
  static_assert(std::is_same_v<Ret, void>, "Delegate return values other than void are not supported.");

public:
  using Function_Sig = Ret(Args...);
  using Delegate_T = Delegate<Function_Sig>;

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

  template <Core::InvocableAndReturn<Ret, Args...> auto function>
  inline constexpr [[nodiscard]] auto bind()
  {
    auto& delegate = m_delegateList.emplace_back();
    auto handleRAII = delegate.bind<function>();
    return handleRAII;
  }

  template <Core::FunctorAndReturn<Ret, Args...> Instance_T>
  inline constexpr [[nodiscard]] auto bind(Instance_T&& functor)
  {
    auto& delegate = m_delegateList.emplace_back();
    auto handleRAII = delegate.bind(std::forward<Instance_T>(functor));
    return handleRAII;
  }

  template <auto function, typename Instance_T>
  requires Core::InvocableAndReturn<decltype(function), Ret, Instance_T, Args...>
  inline constexpr [[nodiscard]] auto bind(Instance_T&& instance)
  {
    auto& delegate = m_delegateList.emplace_back();
    auto handleRAII = delegate.bind<function>(std::forward<Instance_T>(instance));
    return handleRAII;
  }

  template <typename Instance_T>
  inline constexpr decltype(auto) bindObject(Instance_T&& instance)
  {
    auto& delegate = m_delegateList.emplace_back();
    auto handleRAII = delegate.bindObject(std::forward<Instance_T>(instance));

    return Delegate_T::template ObjectMemFnBinder<Instance_T>(&delegate, std::forward<Instance_T>(instance));
  }

  void operator()(const Args&&... args)
  {
    return broadcast(std::forward<Args>(args)...);
  }

  // function names Broadcast fire execute
  void broadcast(const Args&&... args)
  {
    for (auto& delegate : m_delegateList)
    {
      delegate.invoke(std::forward<Args>(args)...);
    }
  }

  bool isEmpty() const
  {
    return m_delegateList.empty();
  }

  constexpr explicit operator bool() const
  {
    return !isEmpty();
  }

  // or reset
  void unbindAll()
  {
    m_delegateList.clear();
  }

  bool operator==(const DelegateMulticast&) const = default;

private:
  std::vector<Delegate_T> m_delegateList;
};

} // namespace Delegate