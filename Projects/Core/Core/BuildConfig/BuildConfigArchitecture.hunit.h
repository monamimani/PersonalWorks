#pragma once

#include <string_view>

namespace BuildConfig
{

enum class Architecture
{
  X86,
  AMD64,
  ARM,
  ARM64
};

#if __x86_64__ || defined _M_X64
constexpr auto architectureStr = std::string_view{"AMD x86-64"};
constexpr auto architecture = Architecture::AMD64;
#elif defined __X86__ || defined __i386__ || defined _M_IX86
constexpr auto architectureStr = std::string_view{"Intel 386+"};
constexpr auto architecture = Architecture::X86;
#elif defined __aarch64__
constexpr auto architectureStr = std::string_view{"ARM64"};
constexpr auto architecture = Architecture::ARM64;
#elif defined ARM || defined __arm__ || defined _ARM
constexpr sauto architectureStr = std::string_view{"ARM"};
constexpr auto architecture = Architecture::ARM;
#else
  #error Architecture not supported.
#endif
}