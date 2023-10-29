#pragma once

#include <utility>

namespace Core
{
// if ((Flags & EFlags::Flag1) != EFlags::None)

// Defenition enum classes bitwise operators so they can be used as bit flags
#define ENUM_CLASS_FLAGS(Enum)                                                         \
  inline constexpr Enum& operator|=(Enum& lhs, Enum rhs)                               \
  {                                                                                    \
    static_assert(std::is_enum_v<Enum>);                                               \
    return lhs = static_cast<Enum>(std::to_underlying(lhs) | std::to_underlying(rhs)); \
  }                                                                                    \
  inline constexpr Enum& operator&=(Enum& lhs, Enum rhs)                               \
  {                                                                                    \
    static_assert(std::is_enum_v<Enum>);                                               \
    return lhs = static_cast<Enum>(std::to_underlying(lhs) & std::to_underlying(rhs)); \
  }                                                                                    \
  inline constexpr Enum& operator^=(Enum& lhs, Enum rhs)                               \
  {                                                                                    \
    static_assert(std::is_enum_v<Enum>);                                               \
    return lhs = static_cast<Enum>(std::to_underlying(lhs) ^ std::to_underlying(rhs)); \
  }                                                                                    \
  inline constexpr Enum operator|(Enum lhs, Enum rhs)                                  \
  {                                                                                    \
    static_assert(std::is_enum_v<Enum>);                                               \
    return static_cast<Enum>(std::to_underlying(lhs) | std::to_underlying(rhs));       \
  }                                                                                    \
  inline constexpr Enum operator&(Enum lhs, Enum rhs)                                  \
  {                                                                                    \
    static_assert(std::is_enum_v<Enum>);                                               \
    return static_cast<Enum>(std::to_underlying(lhs) & std::to_underlying(rhs));       \
  }                                                                                    \
  inline constexpr Enum operator^(Enum lhs, Enum rhs)                                  \
  {                                                                                    \
    static_assert(std::is_enum_v<Enum>);                                               \
    return static_cast<Enum>(std::to_underlying(lhs) ^ std::to_underlying(rhs));       \
  }                                                                                    \
  inline constexpr bool operator!(Enum e)                                              \
  {                                                                                    \
    static_assert(std::is_enum_v<Enum>);                                               \
    return !std::to_underlying(e);                                                     \
  }                                                                                    \
  inline constexpr Enum operator~(Enum e)                                              \
  {                                                                                    \
    static_assert(std::is_enum_v<Enum>);                                               \
    return static_cast<Enum>(~std::to_underlying(e));                                  \
  }

// Friends all bitwise operators for enum classes so the definition can be kept private / protected.
#define FRIEND_ENUM_CLASS_FLAGS(Enum)                  \
  friend Enum& operator|=(Enum& lhs, Enum rhs);        \
  friend Enum& operator&=(Enum& lhs, Enum rhs);        \
  friend Enum& operator^=(Enum& lhs, Enum rhs);        \
  friend constexpr Enum operator|(Enum lhs, Enum rhs); \
  friend constexpr Enum operator&(Enum lhs, Enum rhs); \
  friend constexpr Enum operator^(Enum lhs, Enum rhs); \
  friend constexpr bool operator!(Enum E);             \
  friend constexpr Enum operator~(Enum E);

template <typename Enum>
requires std::is_enum_v<Enum>
constexpr bool enumHasFlags(Enum flags, Enum contains)
{
  return (std::to_underlying(flags) & std::to_underlying(contains)) == std::to_underlying(contains);
}

template <typename Enum>
requires std::is_enum_v<Enum>
constexpr bool enumHasAnyFlags(Enum flags, Enum contains)
{
  return (std::to_underlying(flags) & std::to_underlying(contains)) != 0;
}

template <typename Enum>
requires std::is_enum_v<Enum>
constexpr void enumAddFlags(Enum& flags, Enum flagsToAdd)
{
  flags |= flagsToAdd;
}

template <typename Enum>
requires std::is_enum_v<Enum>
constexpr void enumRemoveFlags(Enum& flags, Enum flagsToRemove)
{
  flags &= static_cast<Enum>(~flagsToRemove);
}

} // namespace Core