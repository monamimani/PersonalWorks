module;
#include <type_traits>
#include <utility>

export module Core:Utility;

namespace Core
{
export template <typename U, typename V>
using CopyConstness_t = std::conditional_t<std::is_const_v<std::remove_reference_t<U>>, const V, V>;

// Do an assignment but garanteing the strong exception safety
export template <class C>
C& strongAssing(C& dest, C src)
{
  using std::swap;
  swap(dest, src);
  return dest;
}

} // namespace Core