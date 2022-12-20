module;
#include <memory>
#include <vector>

export module DelegateMulticast;

import CoreConcepts;
import StaticFunction;

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
    m_staticFunction = std::move(m_staticFunction);
    other.m_staticFunction.reset();
  }

  DelegateMulticastRAII& operator=(const DelegateMulticastRAII&) = delete;
  DelegateMulticastRAII& operator=(DelegateMulticastRAII&& other) noexcept
  {
    m_staticFunction = std::move(m_staticFunction);
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

  // template <typename Instance_T, typename DelegateMulticast_T>
  // class ObjectMemFnBinder;

public:
  using Function_Sig = Ret(Args...);
  using StaticFunction_T = Core::StaticFunction<Function_Sig>;

  using DelegateRAII = DelegateMulticastRAII<StaticFunction_T>;
  // struct DelegateRAII;
  // friend struct DelegateRAII;

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

    // auto& delegate = m_delegateList.emplace_back();
    // auto handleRAII = delegate.bind<function>();
    // return DelegateRAII{std::move(handleRAII)};
  }

  template <Core::FunctorAndReturn<Ret, Args...> Instance_T>
  inline constexpr [[nodiscard]] auto bind(Instance_T&& functor)
  {
    auto staticFunction = std::make_shared<StaticFunction_T>();
    staticFunction->bind(std::forward<Instance_T>(functor));

    m_staticFunctionList.emplace_back(staticFunction);
    return DelegateMulticastRAII{std::move(staticFunction)};

    // auto& delegate = m_delegateList.emplace_back();
    // auto handleRAII = delegate.bind(std::forward<Instance_T>(functor));
    // return DelegateRAII{std::move(handleRAII)};
  }

  template <auto function, typename Instance_T>
  requires Core::InvocableAndReturn<decltype(function), Ret, Instance_T, Args...>
  inline constexpr [[nodiscard]] auto bind(Instance_T&& instance)
  {
    auto staticFunction = std::make_shared<StaticFunction_T>();
    staticFunction->bind<function>(std::forward<Instance_T>(instance));

    m_staticFunctionList.emplace_back(staticFunction);
    return DelegateMulticastRAII{std::move(staticFunction)};

    // auto& delegate = m_delegateList.emplace_back();
    // auto handleRAII = delegate.bind<function>(std::forward<Instance_T>(instance));
    // return DelegateRAII{std::move(handleRAII)};
  }

  // template <typename Instance_T>
  // inline constexpr [[nodiscard]] decltype(auto) bindObject(Instance_T&& instance)
  //{
  //   auto& delegate = m_delegateList.emplace_back();
  //   delegate.constructStorage(std::forward<Instance_T>(instance));
  //   return ObjectMemFnBinder<Instance_T&&, DelegateMulticast<Ret(Args...)>>{&m_delegateList.back()};
  // }

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
  // std::vector<Delegate_T> m_delegateList;
};

// template <typename Ret, typename... Args>
// struct [[nodiscard]] DelegateMulticast<Ret(Args...)>::DelegateRAII
//{
//   using DelegateMulticast_T = DelegateMulticast<Ret(Args...)>;
//   using DelegateHandleRAII_T = DelegateMulticast_T::Delegate_T::DelegateRAII;
//
//   DelegateHandleRAII_T m_delegateHandle;
//
// public:
//
//   DelegateRAII() = default;
//
//   explicit DelegateRAII(DelegateHandleRAII_T&& delegateHandle)
//   : m_delegateHandle{std::move(delegateHandle)}
//   {
//   }
//
//   void unbind() noexcept
//   {
//     m_delegateHandle->unbind();
//   }
// };

// template <typename Ret, typename... Args>
// template <typename Instance_T, typename DelegateMulticast_T>
// class DelegateMulticast<Ret(Args...)>::ObjectMemFnBinder
//{
//   using Delegate_T = DelegateMulticast_T::Delegate_T;
//   using MemFct_Ptr = Delegate_T::template MemberFunction_Ptr<Instance_T>;
//   using MemFctConst_Ptr = Delegate_T::template MemberFunctionConst_Ptr<Instance_T>;
//   using DelegateMulticastHandleRAII_T = DelegateMulticast_T::DelegateRAII;
//
//   friend typename DelegateMulticast_T;
//
//   /// @brief Constructor is private so that only Delegate_T can construct it.
//   ObjectMemFnBinder(Delegate_T* delegate)
//   : m_delegate{delegate}
//   {
//   }
//
// public:
//   template <MemFct_Ptr function>
//   [[nodiscard]] decltype(auto) memFn() &&
//   {
//     auto handleRAII = m_delegate->template setTrampolineFct<Instance_T&&, function>();
//
//     return DelegateMulticastHandleRAII_T{std::move(handleRAII)};
//   }
//
//   template <MemFctConst_Ptr function>
//   [[nodiscard]] decltype(auto) memFnConst() &&
//   {
//     auto handleRAII = m_delegate->template setTrampolineFct<Instance_T&&, function>();
//
//     return DelegateMulticastHandleRAII_T{std::move(handleRAII)};
//   }
//
//   Delegate_T* m_delegate = {};
// };

} // namespace Delegate