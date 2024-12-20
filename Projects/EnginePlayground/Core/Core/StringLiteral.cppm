module;
#include <algorithm>
#include <compare>
#include <cstddef>
#include <string_view>

export module CoreStringLiteral;

export namespace Core
{

template<typename CharT, std::size_t N>
struct BasicStringLiteral
{
  consteval BasicStringLiteral() = default;

  consteval BasicStringLiteral(const CharT (&str)[N])
  {
    std::copy_n(str, N, m_data);
  }

  consteval BasicStringLiteral(std::string_view& strView)
  {
    std::copy_n(strView, m_data);
  }

  [[nodiscard]] consteval auto size() const
  {
    return N - 1;
  }

  [[nodiscard]] consteval std::string_view view() const
  {
    return {m_data, m_data + size()};
  }

  [[nodiscard]] constexpr const CharT* data() const
  {
    return &m_data[0];
  }

  [[nodiscard]] constexpr CharT* data()
  {
    return &m_data[0];
  }

  [[nodiscard]] consteval CharT* begin() const
  {
    return &m_data[0];
  }

  [[nodiscard]] consteval const CharT* cbegin() const
  {
    return &m_data[0];
  }

  [[nodiscard]] consteval CharT* end() const
  {
    return &m_data[size()];
  }

  [[nodiscard]] consteval const CharT* cend() const
  {
    return &m_data[size()];
  }

  friend consteval auto operator<=>(const BasicStringLiteral&, const BasicStringLiteral&) = default;

  CharT m_data[N] = {};
};

template<typename CharT, std::size_t A, std::size_t B>
[[nodiscard]] consteval auto operator+(const BasicStringLiteral<CharT, A>& a, const BasicStringLiteral<CharT, B>& b)
{
  BasicStringLiteral<CharT, A + B - 1> ret;
  std::copy_n(a.m_data, a.size(), ret.m_data);
  std::copy_n(b.m_data, b.size(), ret.m_data + a.size());
  return ret;
}

template<typename CharT, std::size_t A, std::size_t B>
[[nodiscard]] consteval auto operator+(const BasicStringLiteral<CharT, A>& a, const char (&b)[B])
{
  return a + BasicStringLiteral<CharT, B>(b);
}

template<typename CharT, std::size_t A, std::size_t B>
[[nodiscard]] consteval auto operator+(const char (&a)[A], const BasicStringLiteral<CharT, B>& b)
{
  return BasicStringLiteral<CharT, A>(a) + b;
}

template<typename CharT, std::size_t N>
BasicStringLiteral(const CharT (&str)[N]) -> BasicStringLiteral<CharT, N>;
} // namespace Core

export namespace Core
{
template<std::size_t N>
using StringLiteral = BasicStringLiteral<char, N>;
}
