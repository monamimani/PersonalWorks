module;
#include <algorithm>
#include <cstddef>
#include <string_view>
#include <array>

export module CoreStringLiteral;

export namespace Core
{

template<typename CharT, std::size_t N>
struct BasicStringLiteral
{
  consteval BasicStringLiteral() = default;

  consteval explicit BasicStringLiteral(const std::array<CharT, N>& str)
  {
    std::copy_n(str, N, m_data);
  }

  consteval explicit BasicStringLiteral(std::string_view& strView)
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
    return &m_data.data();
  }

  [[nodiscard]] constexpr CharT* data()
  {
    return &m_data.data();
  }

  [[nodiscard]] consteval CharT* begin() const
  {
    return data();
  }

  [[nodiscard]] consteval const CharT* cbegin() const
  {
    return data();
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

  std::array<CharT, N> m_data = {};
};

template<typename CharT, std::size_t LhsSize, std::size_t RhsSize>
[[nodiscard]] consteval auto operator+(const BasicStringLiteral<CharT, LhsSize>& lhs, const BasicStringLiteral<CharT, RhsSize>& rhs)
{
  BasicStringLiteral<CharT, LhsSize + RhsSize - 1> ret;
  std::copy_n(lhs.m_data, lhs.size(), ret.m_data);
  std::copy_n(rhs.m_data, rhs.size(), ret.m_data + lhs.size());
  return ret;
}

template<typename CharT, std::size_t LhsSize, std::size_t RhsSize>
[[nodiscard]] consteval auto operator+(const BasicStringLiteral<CharT, LhsSize>& lhs, const std::array<CharT, RhsSize>& rhs)
{
  return lhs + BasicStringLiteral<CharT, RhsSize>(rhs);
}

template<typename CharT, std::size_t LhsSize, std::size_t RhsSize>
[[nodiscard]] consteval auto operator+(const std::array<CharT, LhsSize>& lhs, const BasicStringLiteral<CharT, RhsSize>& rhs)
{
  return BasicStringLiteral<CharT, LhsSize>(lhs) + rhs;
}

template<typename CharT, std::size_t N>
BasicStringLiteral(const std::array<CharT, N>) -> BasicStringLiteral<CharT, N>;
} // namespace Core

export namespace Core
{
template<std::size_t N>
using StringLiteral = BasicStringLiteral<char, N>;
}
