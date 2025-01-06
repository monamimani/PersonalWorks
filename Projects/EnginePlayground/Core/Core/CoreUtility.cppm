module;
//#include <new>
#include <type_traits>
#include <utility>

import CoreConcepts;
export module CoreUtility;

export namespace Core
{
// Do an assignment but guaranteeing the strong exception safety
template<class C>
[[nodiscard]] constexpr C& strongAssing(C& dest, C src)
{
  using std::swap;
  swap(dest, src);
  return dest;
}

// NOLINTBEGIN(readability-identifier-naming, readability-identifier-length, cppcoreguidelines-pro-type-reinterpret-cast)
export template<class T>
requires(std::is_trivially_copyable_v<T> && is_implicit_lifetime<T>)
[[nodiscard]] constexpr T* start_lifetime_as(void* p) noexcept
{
  //auto* constP = const_cast<void*>(p);
  //auto bytes[] = new (constP) std::byte[sizeof(T)];
  //auto* ptr = reinterpret_cast<T*>(bytes);
  //return std::launder(ptr);

  return std::launder(static_cast<T*>(std::memmove(p, p, sizeof(T))));
}

export template<class T>
[[nodiscard]] constexpr const T* start_lifetime_as(const void* p) noexcept
{
  return start_lifetime_as<T>(const_cast<void*>(p));
}

export template<class T>
[[nodiscard]] constexpr volatile T* start_lifetime_as(volatile void* p) noexcept
{
  return start_lifetime_as<T>(const_cast<void*>(p));
}

export template<class T>
[[nodiscard]] constexpr const volatile T* start_lifetime_as(const volatile void* p) noexcept
{
  return start_lifetime_as<T>(const_cast<void*>(p));
}
// NOLINTEND(readability-identifier-naming, readability-identifier-length, cppcoreguidelines-pro-type-reinterpret-cast)

} // namespace Core
