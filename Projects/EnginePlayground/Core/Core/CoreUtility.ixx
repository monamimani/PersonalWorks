module;
#include <type_traits>
#include <utility>
#include <memory>

export module CoreUtility;

namespace Core
{
export template <typename U, typename V>
using CopyConstness_t = std::conditional_t<std::is_const_v<std::remove_reference_t<U>>, const V, V>;

// Do an assignment but garanteing the strong exception safety
template <class C>
[[nodiscard]] C& strongAssing(C& dest, C src)
{
  using std::swap;
  swap(dest, src);
  return dest;
}

export template <class T>
[[nodiscard]] auto start_lifetime_as(void* p) noexcept
{
  const auto mp = const_cast<void*>(p);
  const auto bytes = new (mp) std::byte[sizeof(T)];
  const auto ptr = reinterpret_cast<T*>(bytes);
  return std::launder<T>(ptr);
}

export template <class T>
[[nodiscard]] auto start_lifetime_as(const void* p) noexcept
{
  const auto mp = const_cast<void*>(p);
  const auto bytes = new (mp) std::byte[sizeof(T)];
  const auto ptr = reinterpret_cast<const T*>(bytes);
  return std::launder<const T>(ptr);
}

} // namespace Core