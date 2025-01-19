module;
#include <new>
#include <utility>
#include <cstring>

export module CoreUtility;

import CoreConcepts;

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

// NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast)
template<typename T, typename U>
requires(IsImplicitLifetime<T>)
[[nodiscard]] constexpr T* start_lifetime_as(U* ptr) noexcept
{
  //auto* constP = const_cast<void*>(p);
  //auto bytes[] = new (constP) std::byte[sizeof(T)];
  //auto* ptr = reinterpret_cast<T*>(bytes);
  //return std::launder(ptr);

  return std::launder(static_cast<ConstVolatileAsT<U,T>*>(std::memmove(ptr, ptr, sizeof(T))));
}
// NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast)

} // namespace Core
