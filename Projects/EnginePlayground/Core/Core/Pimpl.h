#pragma once
#include <compare>
#include <concepts>
#include <memory>
#include <type_traits>

// inspired from https://github.com/oliora/samples/blob/master/spimpl.h But I simplified removing the dependency on unique_ptr it simplified the class greatly.

namespace Core
{
template <typename T>
class ImplPtr
{
public:
  using pointer = T*;
  using const_pointer = typename std::add_const<T>::type*;
  using reference = T&;
  using const_reference = typename std::add_const<T>::type&;
  using element_type = T;

  constexpr ImplPtr() noexcept
  : m_ptr{nullptr}
  {
  }

  ~ImplPtr() noexcept
  {
    delete m_ptr;
    m_ptr = nullptr;
  }

  constexpr ImplPtr(std::nullptr_t) noexcept
  : ImplPtr()
  {
  }

  constexpr ImplPtr(pointer p) noexcept
  : m_ptr(std::move(p))
  {
    p = nullptr;
  }

  constexpr ImplPtr(const ImplPtr& implPtr) noexcept(noexcept(std::is_nothrow_copy_constructible_v<T>)) requires(std::copy_constructible<T>)
  : m_ptr{nullptr}
  {
    *this = implPtr;
  }

  constexpr ImplPtr& operator=(const ImplPtr& implPtr) noexcept(noexcept(std::is_nothrow_assignable_v<T, T>)) requires(std::is_assignable_v<T, T>)
  {
    if (this != &implPtr)
    {
      if (m_ptr)
      {
        (*m_ptr) = (*implPtr.m_ptr);
      }
      else
      {
        m_ptr = new T{*(implPtr.m_ptr)};
      }

    }
    return *this;
  }

  constexpr ImplPtr(ImplPtr&& implPtr) noexcept
  : m_ptr{std::move(implPtr.m_ptr)}
  {
    implPtr.m_ptr = nullptr;
  }

  constexpr ImplPtr& operator=(ImplPtr&& implPtr) noexcept
  {
    m_ptr = std::move(implPtr.m_ptr);
    implPtr.m_ptr = nullptr;
    return *this;
  }

  // clang-format off
  template <std::convertible_to<T*> U>
  constexpr ImplPtr(U* u) noexcept(noexcept(m_ptr(u)))
  : m_ptr(u)
  {
  }
  
  // clang-format on

  constexpr reference operator*()
  {
    return *m_ptr;
  }
  constexpr const_reference operator*() const
  {
    return *m_ptr;
  }

  constexpr pointer operator->() noexcept
  {
    return get();
  }
  constexpr const_pointer operator->() const noexcept
  {
    return get();
  }

  constexpr pointer get() noexcept
  {
    return m_ptr;
  }
  constexpr const_pointer get() const noexcept
  {
    return m_ptr;
  }

  constexpr void swap(ImplPtr& u) noexcept(noexcept(std::declval<pointer>(), std::declval<ImplPtr::pointer>())) //noexcept(std::is_nothrow_swappable_v<ImplPtr>)
  {
    using std::swap;
    swap(m_ptr, u.m_ptr);
  }

  constexpr pointer release() noexcept
  {
    auto tmp = m_ptr;
    m_ptr = nullptr;
    return tmp;
  }

  constexpr explicit operator bool() const noexcept
  {
    return static_cast<bool>(m_ptr != nullptr);
  }

  auto operator<=>(const ImplPtr<T>&) const = default;

private:
  T* m_ptr = nullptr;
};

template <class T>
inline void swap(ImplPtr<T>& lhs, ImplPtr<T>& rhs) noexcept(noexcept(lhs.swap(rhs)))
{
  lhs.swap(rhs);
}

template <class T, class... Args>
inline ImplPtr<T> makeImplPtr(Args&&... args)
{
  return ImplPtr<T>(new T(std::forward<Args>(args)...));
}
} // namespace Core

namespace std
{
template <typename T>
struct hash<Core::ImplPtr<T>>
{
  using argument_type = Core::ImplPtr<T>;
  using result_type = size_t;

  result_type operator()(const argument_type& arg) const noexcept
  {
    return hash<typename argument_type::const_pointer>()(arg.get());
  }
};
} // namespace std


/*
Used this way

Header file:
#include "Pimpl.h"

class ConcreteType {
public:
    ConcreteType(int a);

    ...
    
private:
    class Impl;
    Core::ImplPtr<Impl> m_impl; 
};

Source file:

ConcreteType::ConcreteType(int a)
: m_impl(Core::makeImplPtr<Impl>(a))
{}

*/