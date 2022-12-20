#pragma once

#include <concepts>
#include <functional>

import CoreConcepts;

namespace Delegate
{
template <typename F>
class DelegateStdFunction;

template <class R, class... Args>
class DelegateStdFunction<R(Args...)>
{
  using Function_Sig = R(Args...);
  using Function_Ptr = std::add_pointer_t<Function_Sig>;

  //template <typename Instance_T>
  //using InstanceType = std::remove_reference_t<Instance_T>;
  // template <typename Instance_T>
  // using MemberFunction_Sig = Ret(InstanceType<Instance_T>&, Args...);
  template <typename Instance_T>
  using MemberFunction_Ptr = R (Instance_T::*)(Args...);
  template <typename Instance_T>
  using MemberFunctionConst_Ptr = R (Instance_T::*)(Args...) const;
  // template <typename Instance_T>
  // using MemberFunctionConstOrNot_Ptr = std::conditional_t<std::is_const_v<InstanceType<Instance_T>>, MemberFunctionConst_Ptr<Instance_T>, MemberFunction_Ptr<Instance_T>>;

public:
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

  struct DelegateRAII
  {
    using Delegate_T = DelegateStdFunction<R(Args...)>;

    Delegate_T* const m_delegate = nullptr;

  public:
    [[deprecated("Remove when DelegateRAII is properly implemented.")]] DelegateRAII() = default;
    DelegateRAII(Delegate_T* const delegate)
    : m_delegate{delegate}
    {
    }
    ~DelegateRAII()
    {
      if (m_delegate)
      {
        m_delegate->unbind();
      }
    }
  };

  template <Core::InvocableAndReturn<R, Args...> auto F>
  inline constexpr [[nodiscard]] auto bind()
  {
    m_fct = F;

    return DelegateRAII{};
  }

  template <Core::FunctorAndReturn<R, Args...> Instance_T>
  inline constexpr [[nodiscard]] auto bind(Instance_T&& functor)
  {
    m_fct = [&functor](Args... args) -> R
    {
      return std::invoke(functor, std::forward<Args>(args)...);
    };

    return DelegateRAII{};
  }

  template <auto F, typename Instance_T>
  requires Core::InvocableAndReturnNTTP<F, R, Instance_T, Args...>
  inline constexpr [[nodiscard]] auto bind(Instance_T&& instance)
  {
    if constexpr (std::is_lvalue_reference_v<Instance_T&&>)
    {
      m_fct = [&instance](Args... args) -> R
      {
        return std::invoke(F, instance, std::forward<Args>(args)...);
      };
    }
    else if constexpr (std::is_rvalue_reference_v<Instance_T&&>)
    {
      m_fct = [instance_ = std::move(instance)](Args... args) mutable -> R
      {
        return std::invoke(F, instance_, std::forward<Args>(args)...);
      };
    }

    return DelegateRAII{};
  }

  template <typename Instance_T>
  [[nodiscard]] constexpr decltype(auto) bindObject(Instance_T&& instance)
  {
    return ObjectMemFnBinder<Instance_T>{this, std::forward<Instance_T>(instance)};
  }

  R operator()(Args... args) const
  {
    return m_fct(std::forward<Args>(args)...);
  }

  constexpr decltype(auto) invoke(Args&&... args)
  {
    return m_fct(std::forward<Args>(args)...);
  }

  constexpr void unbind() noexcept
  {
    reset();
  }

  constexpr void reset() noexcept
  {
    m_fct = {};
  }

  constexpr void swap(DelegateStdFunction& other)
  {
    using std::swap;

    swap(m_fct, other.m_fct);
  }

  constexpr bool isBound() const
  {
    return bool(m_fct);
  }

  constexpr explicit operator bool() const
  {
    return isBound();
  }

  bool operator==(const DelegateStdFunction&) const = default;

private:


  template <typename Instance_T>
  class ObjectMemFnBinder
  {
    using Type = std::remove_reference_t<Instance_T>;
    using MemFct_Ptr = MemberFunction_Ptr<Instance_T>;
    using MemFctConst_Ptr = MemberFunctionConst_Ptr<Instance_T>;

    template <auto function>
    void setTrampolineFct()
    {
      if constexpr (std::is_lvalue_reference_v<Instance_T&&>)
      {
        m_delegate->m_fct = [&instance = m_instance](Args... args) -> R
        {
          return std::invoke(function, instance, std::forward<Args>(args)...);
        };
      }
      else if constexpr (std::is_rvalue_reference_v<Instance_T&&>)
      {
        m_delegate->m_fct = [instance_ = std::move(m_instance)](Args... args) mutable -> R
        {
          return std::invoke(function, instance_, std::forward<Args>(args)...);
        };
      }
    }

  public:

    template <MemFct_Ptr function>
    [[nodiscard]] auto memFn()
    {
      setTrampolineFct<function>();

      // return DelegateRAII{&m_delegate};
      return DelegateRAII{};
    }

    template <MemFctConst_Ptr function>
    [[nodiscard]] auto memFnConst()
    {
      setTrampolineFct<function>();

      // return DelegateRAII{&m_delegate};
      return DelegateRAII{};
    }

    DelegateStdFunction<R(Args...)>* m_delegate = {};
    Instance_T&& m_instance;
  };

  std::function<R(Args...)> m_fct;
};

template <typename Signature>
inline void swap(DelegateStdFunction<Signature>& d1, DelegateStdFunction<Signature>& d2)
{
  d1.swap(d2);
}

} // namespace Delegate