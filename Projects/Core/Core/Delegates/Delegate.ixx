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

namespace Delegate
{

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
  using UnderlineStorageType = void*;
  inline static constexpr auto UnderlineStorageAlingOf = alignof(UnderlineStorageType);
  inline static constexpr auto UnderlineStorageSize = 2 * sizeof(UnderlineStorageType);
  using Storage_T = Core::ErasedStorage<UnderlineStorageSize, UnderlineStorageAlingOf>;

  using Trampoline_T = Ret (*)(Storage_T&, Args&&...);

  using Function_Sig = Ret(Args...);

  template <typename Instance_T, typename Delegate_T>
  class ObjectMemFnBinder;

  friend class DelegateMulticast<Function_Sig>;

  friend struct DelegateRAII;

public:
  template <typename Instance_T>
  using InstanceType = std::remove_reference_t<Instance_T>;
  // template <typename Instance_T>
  // using MemberFunction_Sig = Ret(InstanceType<Instance_T>&, Args...);
  template <typename Instance_T>
  using MemberFunction_Ptr = Ret (InstanceType<Instance_T>::*)(Args...);
  template <typename Instance_T>
  using MemberFunctionConst_Ptr = Ret (InstanceType<Instance_T>::*)(Args...) const;
  template <typename Instance_T>
  using MemberFunctionConstOrNot_Ptr = std::conditional_t<std::is_const_v<InstanceType<Instance_T>>, MemberFunctionConst_Ptr<Instance_T>, MemberFunction_Ptr<Instance_T>>;

public:
  struct DelegateRAII;

  // Delegate() = default;
  ~Delegate()
  {
    m_function = nullptr;
    m_sentinel.reset();
  }

  // Delegate(const Delegate&) noexcept = default;
  // Delegate(Delegate&&) noexcept = default;
  // Delegate& operator=(const Delegate&) noexcept = default;
  // Delegate& operator=(Delegate&&) noexcept = default;

  template <typename Instance_T>
  static consteval decltype(auto) asMemFnPtr(MemberFunction_Ptr<Instance_T> fct)
  {
    return fct;
  }

  template <typename Instance_T>
  static consteval decltype(auto) asMemFnConstPtr(MemberFunctionConst_Ptr<Instance_T> fct)
  {
    return fct;
  }

  template <Core::InvocableAndReturn<Ret, Args...> auto function>
  [[nodiscard]] constexpr auto bind()
  {
    reset();
    m_function = [](Storage_T&, Args&&... args) -> Ret
    {
      return std::invoke_r<Ret>(function, std::forward<Args>(args)...);
    };

    return DelegateRAII{this};
  }

  template <Core::FunctorAndReturn<Ret, Args...> Instance_T>
  [[nodiscard]] constexpr auto bind(Instance_T&& functor)
  {

    reset();
    constructStorage(std::forward<Instance_T>(functor));
    return setTrampolineFct<Instance_T&&>();
  }

  template <Core::InvocableAndReturn<Ret, Args...> auto function, typename Instance_T>
  // requires Core::InvocableAndReturn<decltype(function), Ret, Instance_T, Args...>
  [[nodiscard]] constexpr decltype(auto) bind(Instance_T&& instance)
  {
    reset();
    constructStorage(std::forward<Instance_T>(instance));
    return setTrampolineFct<Instance_T&&, function>();
  }

  template <auto function, typename Instance_T>
  requires Core::InvocableAndReturnNTTP<function, Ret, Instance_T, Args...>
  [[nodiscard]] constexpr auto bind(Instance_T&& instance)
  {
    reset();
    constructStorage(std::forward<Instance_T>(instance));

    return setTrampolineFct<Instance_T&&, function>();
  }

  /// @brief Function to be used when the delegate function is overloaded (const, parameter or a template for exemple).
  /// @tparam Instance_T
  /// @param instance
  /// @return
  template <typename Instance_T>
  [[nodiscard]] constexpr auto bindObject(Instance_T&& instance)
  {
    reset();
    constructStorage(std::forward<Instance_T>(instance));
    return ObjectMemFnBinder<Instance_T&&, Delegate<Ret(Args...)>>{this};
  }

  [[nodiscard]] constexpr bool isBound() const
  {
    return m_function != nullptr;
  }

  constexpr void unbind() noexcept
  {
    reset();
  }

  constexpr void reset() noexcept
  {
    m_sentinel = std::make_shared<bool>();
    m_storage = {};
    m_function = nullptr;
  }

  constexpr void swap(Delegate& other)
  {
    using std::swap;

    swap(m_storage, other.m_storage);
    swap(m_function, other.m_function);
    swap(m_sentinel, other.m_sentinel);
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
    return m_function(m_storage, std::forward<Args>(args)...);
  }

  constexpr decltype(auto) invokeSafe(Args&&... args)
  {
    if constexpr (std::is_void_v<Ret>)
    {
      if (m_function)
      {
        m_function(m_storage, std::forward<Args>(args)...);
      }
      return;
    }
    else
    {
      std::optional<Ret> result;
      if (m_function)
      {
        result = std::optional{m_function(m_storage, std::forward<Args>(args)...)};
      }
      return result;
    }
  }

  bool operator==(const Delegate&) const = default;

public:
  template <typename Instance_T>
  [[nodiscard]] auto setTrampolineFct()
  {
    if constexpr (std::is_lvalue_reference_v<Instance_T&&>)
    {
      m_function = [](Storage_T& storage, Args&&... args) -> Ret
      {
        using Type = std::remove_reference_t<Instance_T>;
        auto* object = storage.asTypedPtr<Type*>();
        return std::invoke_r<Ret>((**object), std::forward<Args>(args)...);
      };
    }

    if constexpr (std::is_rvalue_reference_v<Instance_T&&>)
    {
      m_function = [](Storage_T& storage, Args&&... args) -> Ret
      {
        auto* object = storage.asTypedPtr<Instance_T>();
        return std::invoke_r<Ret>((*object), std::forward<Args>(args)...);
      };
    }

    return DelegateRAII{this};
  }

  template <typename Instance_T, auto function>
  [[nodiscard]] auto setTrampolineFct()
  {
    if constexpr (std::is_lvalue_reference_v<Instance_T&&>)
    {
      m_function = [](Storage_T& storage, Args&&... args) -> Ret
      {
        using Type = std::remove_reference_t<Instance_T>;
        auto* object = storage.asTypedPtr<Type*>();
        return std::invoke_r<Ret>(function, (*object), std::forward<Args>(args)...);
      };
    }

    if constexpr (std::is_rvalue_reference_v<Instance_T&&>)
    {
      m_function = [](Storage_T& storage, Args&&... args) -> Ret
      {
        auto* object = storage.asTypedPtr<Instance_T>();
        return std::invoke_r<Ret>(function, object, std::forward<Args>(args)...);
      };
    }

    return DelegateRAII{this};
  }

  // template <typename Instance_T>
  //[[nodiscard]] auto setTrampolineFct(MemberFunctionConstOrNot_Ptr<Instance_T> function)
  //{
  //   if constexpr (std::is_lvalue_reference_v<Instance_T&&>)
  //   {
  //     m_function = [](Storage_T& storage, Args&&... args) -> Ret
  //     {
  //       using Type = std::remove_reference_t<Instance_T>;
  //       auto* object = storage.asTypedPtr<Type*>();
  //       return std::invoke_r<Ret>(function, (*object), std::forward<Args>(args)...);
  //     };
  //   }

  //  if constexpr (std::is_rvalue_reference_v<Instance_T&&>)
  //  {
  //    m_function = [](Storage_T& storage, Args&&... args) -> Ret
  //    {
  //      auto* object = storage.asTypedPtr<Instance_T>();
  //      return std::invoke_r<Ret>(function, object, std::forward<Args>(args)...);
  //    };
  //  }

  //  //return DelegateRAII{this};
  //  return DelegateRAII{};
  //}

  template <typename Instance_T>
  void constructStorage(Instance_T&& instance)
  {
    if constexpr (std::is_lvalue_reference_v<Instance_T&&>)
    {
      using Type = std::remove_reference_t<Instance_T>;
      m_storage.construct<Type*>(&instance);
    }

    if constexpr (std::is_rvalue_reference_v<Instance_T&&>)
    {
      m_storage.construct<Instance_T&&>(std::forward<Instance_T>(instance));
    }
  }

private:
  Storage_T m_storage;
  Trampoline_T m_function = nullptr;
  std::shared_ptr<bool> m_sentinel = std::make_shared<bool>();
};

export template <typename Signature>
inline void swap(Delegate<Signature>& d1, Delegate<Signature>& d2)
{
  d1.swap(d2);
}

template <typename Ret, typename... Args>
struct [[nodiscard]] Delegate<Ret(Args...)>::DelegateRAII
{
  using Delegate_T = Delegate<Ret(Args...)>;

  Delegate_T* m_delegate = nullptr;
  std::weak_ptr<bool> m_sentinel;

public:
  DelegateRAII() noexcept = default;

  explicit DelegateRAII(Delegate_T* const delegate) noexcept
  : m_delegate{delegate}
  , m_sentinel{delegate->m_sentinel}
  {
  }

  ~DelegateRAII()
  {
    unbind();
  }

  DelegateRAII(const DelegateRAII& other) = delete;
  DelegateRAII(DelegateRAII&& other) noexcept
  : m_delegate{std::exchange(other.m_delegate, nullptr)}
  {
  }

  DelegateRAII& operator=(const DelegateRAII&) = delete;
  DelegateRAII& operator=(DelegateRAII&& other) noexcept
  {
    m_delegate = std::exchange(other.m_delegate, nullptr);
    return *this;
  }

  void unbind() noexcept
  {
    if (!m_sentinel.expired() && m_delegate)
    {
      m_delegate->unbind();
    }
  }
};

template <typename Ret, typename... Args>
template <typename Instance_T, typename Delegate_T>
class [[nodiscard]] Delegate<Ret(Args...)>::ObjectMemFnBinder
{
  using Type = std::remove_reference_t<Instance_T>;
  using MemFct_Ptr = Delegate_T::template MemberFunction_Ptr<Instance_T>;
  using MemFctConst_Ptr = Delegate_T::template MemberFunctionConst_Ptr<Instance_T>;

  friend typename Delegate_T;

  /// @brief Constructor is private so that only Delegate_T can construct it.
  ObjectMemFnBinder(Delegate_T* delegate)
  : m_delegate{delegate}
  {
  }

public:
  template <MemFct_Ptr function>
  [[nodiscard]] decltype(auto) memFn() &&
  {
    return m_delegate->template setTrampolineFct<Instance_T&&, function>();
  }

  template <MemFctConst_Ptr function>
  [[nodiscard]] decltype(auto) memFnConst() &&
  {
    return m_delegate->template setTrampolineFct<Instance_T&&, function>();
  }

  Delegate_T* m_delegate = {};
};

} // namespace Delegate

module :private;
