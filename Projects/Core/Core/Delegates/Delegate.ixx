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

template <typename StaticFunction_T>
class [[nodiscard]] DelegateRAII
{
public:
  DelegateRAII() noexcept = default;

  explicit DelegateRAII(std::shared_ptr<StaticFunction_T>&& staticFunction) noexcept
  : m_staticFunction{std::move(staticFunction)}
  {
  }

  ~DelegateRAII()
  {
    unbind();
  }

  DelegateRAII(const DelegateRAII& other) = delete;
  DelegateRAII(DelegateRAII&& other) noexcept
  {
    m_staticFunction = std::move(other.m_staticFunction);
    other.m_staticFunction.reset();
  }

  DelegateRAII& operator=(const DelegateRAII&) = delete;
  DelegateRAII& operator=(DelegateRAII&& other) noexcept
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

export template <typename Signature>
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
export template <typename Ret, typename... Args>
class Delegate<Ret(Args...)> final
{
private:
  using Function_Sig = Ret(Args...);
  using StaticFunction_T = Core::StaticFunction<Function_Sig>;

public:
  // template <typename Instance_T>
  // using InstanceType = std::remove_reference_t<Instance_T>;
  //  template <typename Instance_T>
  //  using MemberFunction_Sig = Ret(InstanceType<Instance_T>&, Args...);
  template <typename Instance_T>
  using MemberFunction_Ptr = Ret (Instance_T::*)(Args...);
  template <typename Instance_T>
  using MemberFunctionConst_Ptr = Ret (Instance_T::*)(Args...) const;
  // template <typename Instance_T>
  // using MemberFunctionConstOrNot_Ptr = std::conditional_t<std::is_const_v<Instance_T>, MemberFunctionConst_Ptr<Instance_T>, MemberFunction_Ptr<Instance_T>>;

  using DelegateRAII = DelegateRAII<StaticFunction_T>;

   Delegate() = default;
  ~Delegate()
  {
    m_staticFunction.reset();
  }

  // Delegate(const Delegate&) noexcept = default;
  // Delegate(Delegate&&) noexcept = default;
  // Delegate& operator=(const Delegate&) noexcept = default;
  // Delegate& operator=(Delegate&&) noexcept = default;

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
  [[nodiscard]] constexpr auto bind()
  {
    auto staticFunction = std::make_shared<StaticFunction_T>();
    staticFunction->bind<function>();
    m_staticFunction = std::weak_ptr<StaticFunction_T>{staticFunction};
    return DelegateRAII{std::move(staticFunction)};
  }

  template <Core::FunctorAndReturn<Ret, Args...> Instance_T>
  [[nodiscard]] constexpr auto bind(Instance_T&& functor)
  {
    auto staticFunction = std::make_shared<StaticFunction_T>();
    staticFunction->bind(std::forward<Instance_T>(functor));
    m_staticFunction = std::weak_ptr<StaticFunction_T>{staticFunction};
    return DelegateRAII{std::move(staticFunction)};
  }

  template <auto function, typename Instance_T>
  requires Core::InvocableAndReturnNTTP<function, Ret, Instance_T, Args...>
  [[nodiscard]] constexpr auto bind(Instance_T&& instance)
  {
    auto staticFunction = std::make_shared<StaticFunction_T>();
    staticFunction->bind<function>(std::forward<Instance_T>(instance));
    m_staticFunction = std::weak_ptr<StaticFunction_T>{staticFunction};
    return DelegateRAII{std::move(staticFunction)};
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

export template <typename Signature>
inline void swap(Delegate<Signature>& d1, Delegate<Signature>& d2)
{
  d1.swap(d2);
}

// template <typename Ret, typename... Args>
// template <typename Instance_T, typename ObjectMemFnBinder_T>
// class [[nodiscard]] Delegate<Ret(Args...)>::ObjectMemFnBinder
//{
//   using Type = std::remove_reference_t<Instance_T>;
//   using MemFct_Ptr = StaticFunction_T::template MemberFunction_Ptr<Instance_T>;
//   using MemFctConst_Ptr = StaticFunction_T::template MemberFunctionConst_Ptr<Instance_T>;
//
//   /// @brief Constructor is private so that only StaticFunction_T can construct it.
//   ObjectMemFnBinder(std::weak_ptr<StaticFunction_T> staticFunction, ObjectMemFnBinder_T&& objectMemFnBinder)
//     : m_objectMemFnBinder{ objectMemFnBinder }
//     , m_staticFunction{ staticFunction }
//   {
//   }
//
// public:
//   template <MemFct_Ptr function>
//   [[nodiscard]] decltype(auto) memFn()&&
//   {
//     m_objectMemFnBinder.memFn<function>();
//     return DelegateRAII{ m_staticFunction };
//   }
//
//   template <MemFctConst_Ptr function>
//   [[nodiscard]] decltype(auto) memFnConst()&&
//   {
//     m_objectMemFnBinder.memFnConst<function>();
//     return DelegateRAII{ m_staticFunction };
//   }
//   ObjectMemFnBinder_T& m_objectMemFnBinder;
//   std::weak_ptr<StaticFunction_T> m_staticFunction = {};
// };

// template <typename Ret, typename... Args>
// struct [[nodiscard]] Delegate<Ret(Args...)>::DelegateRAII
//{
//   using Delegate_T = Delegate<Ret(Args...)>;
//
//   Delegate_T* m_delegate = nullptr;
//   std::weak_ptr<bool> m_sentinel;
//
// public:
//   DelegateRAII() noexcept = default;
//
//   explicit DelegateRAII(Delegate_T* const delegate) noexcept
//   : m_delegate{delegate}
//   , m_sentinel{delegate->m_sentinel}
//   {
//   }
//
//   ~DelegateRAII()
//   {
//     unbind();
//   }
//
//   DelegateRAII(const DelegateRAII& other) = delete;
//   DelegateRAII(DelegateRAII&& other) noexcept
//   : m_delegate{std::exchange(other.m_delegate, nullptr)}
//   {
//   }
//
//   DelegateRAII& operator=(const DelegateRAII&) = delete;
//   DelegateRAII& operator=(DelegateRAII&& other) noexcept
//   {
//     m_delegate = std::exchange(other.m_delegate, nullptr);
//     return *this;
//   }
//
//   void unbind() noexcept
//   {
//     if (!m_sentinel.expired() && m_delegate)
//     {
//       m_delegate->unbind();
//     }
//   }
// };

// template <typename Ret, typename... Args>
// template <typename Instance_T, typename Delegate_T>
// class [[nodiscard]] Delegate<Ret(Args...)>::ObjectMemFnBinder
//{
//   using Type = std::remove_reference_t<Instance_T>;
//   using MemFct_Ptr = Delegate_T::template MemberFunction_Ptr<Instance_T>;
//   using MemFctConst_Ptr = Delegate_T::template MemberFunctionConst_Ptr<Instance_T>;
//
//   friend typename Delegate_T;
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
//     return m_delegate->template setTrampolineFct<Instance_T&&, function>();
//   }
//
//   template <MemFctConst_Ptr function>
//   [[nodiscard]] decltype(auto) memFnConst() &&
//   {
//     return m_delegate->template setTrampolineFct<Instance_T&&, function>();
//   }
//
//   Delegate_T* m_delegate = {};
// };

} // namespace Delegate

module :private;
