module;
#include <functional>
#include <optional>
#include <type_traits>

export module StaticFunction;

import CoreConcepts;
import ErasedStorage;

namespace Core
{
export template<typename Signature>
class StaticFunction;

export template<typename Ret, typename... Args>
class StaticFunction<Ret(Args...)> final
{

  using UnderlineStorageType = void*;
  inline static constexpr auto UnderlineStorageAlingOf = alignof(UnderlineStorageType);
  inline static constexpr auto UnderlineStorageSize = 2 * sizeof(UnderlineStorageType);
  using Storage_T = Core::ErasedStorage<UnderlineStorageSize, UnderlineStorageAlingOf>;

  using Trampoline_T = Ret (*)(Storage_T&, Args&&...);
  using Function_Sig = Ret(Args...);

  template<typename Instance_T, typename StaticFunction_T>
  class ObjectMemFnBinder;

public:
  // template <typename Instance_T>
  // using InstanceType = std::remove_reference_t<Instance_T>;
  template<typename Instance_T>
  using MemberFunctionPtr = Ret (Instance_T::*)(Args...);
  template<typename Instance_T>
  using MemberFunctionConstPtr = Ret (Instance_T::*)(Args...) const;

  // template <typename Instance_T>
  // using MemberFunctionConstOrNot_Ptr = std::conditional_t<std::is_const_v<InstanceType<Instance_T>>, MemberFunctionConstPtr<Instance_T>, MemberFunctionPtr<Instance_T>>;

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
  constexpr void bind()
  {
    reset();
    m_function = [](Storage_T&, Args&&... args) -> Ret {
      return std::invoke_r<Ret>(function, std::forward<Args>(args)...);
    };
  }

  template<Core::FunctorAndReturn<Ret, Args...> Instance_T>
  constexpr void bind(Instance_T&& functor)
  {
    reset();
    constructStorage(std::forward<Instance_T>(functor));
    setTrampolineFct<Instance_T>();
  }

  template<auto function, typename Instance_T>
  requires Core::InvocableAndReturnNTTP<function, Ret, Instance_T, Args...>
  constexpr void bind(Instance_T&& instance)
  {
    reset();
    constructStorage(std::forward<Instance_T>(instance));
    setTrampolineFct<Instance_T, function>();
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
    m_storage = {};
    m_function = nullptr;
  }

  constexpr void swap(StaticFunction& other)
  {
    using std::swap;

    swap(m_storage, other.m_storage);
    swap(m_function, other.m_function);
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

  [[nodiscard]] constexpr decltype(auto) invokeSafe(Args&&... args)
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

  bool operator==(const StaticFunction&) const = default;

private:
  template<typename Instance_T>
  void setTrampolineFct()
  {
    using Type = std::conditional_t<std::is_lvalue_reference_v<Instance_T&&>, std::add_pointer_t<std::remove_reference_t<Instance_T>>, std::remove_reference_t<Instance_T>>;

    m_function = [](Storage_T& storage, Args&&... args) -> Ret {
      auto* object = storage.asTypedPtr<Type>();
      return std::invoke_r<Ret>((*object), std::forward<Args>(args)...);
    };
  }

  template<typename Instance_T, auto function>
  void setTrampolineFct()
  {
    using Type = std::conditional_t<std::is_lvalue_reference_v<Instance_T&&>, std::add_pointer_t<std::remove_reference_t<Instance_T>>, std::remove_reference_t<Instance_T>>;

    m_function = [](Storage_T& storage, Args&&... args) -> Ret {
      auto* object = storage.asTypedPtr<Type>();
      return std::invoke_r<Ret>(function, object, std::forward<Args>(args)...);
    };
  }


  template<typename Instance_T>
  void constructStorage(Instance_T&& instance)
  {
    if constexpr (std::is_pointer_v<Instance_T>)
    {
      m_storage.construct(instance);
    }
    else if constexpr (std::is_lvalue_reference_v<Instance_T>)
    {
      m_storage.construct(&instance);
    }
    else if constexpr (std::is_rvalue_reference_v<Instance_T&&>)
    {
      m_storage.construct(std::forward<Instance_T>(instance));
    }
    else
    {
      std::unreachable();
    }
  }

private:
  Storage_T m_storage;
  Trampoline_T m_function = nullptr;
};
} // namespace Core

module :private;
