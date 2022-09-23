#pragma once

#include <string_view>

namespace BuildConfig
{

enum class Compiler
{
  Clang,
  GCC,
  MSVC
};

#ifdef __clang__
constexpr auto compilerStr = std::string_view{"Clang"};
constexpr auto compiler = Compiler::Clang;
#elif __GNUC__
constexpr auto compilerStr = std::string_view{"Gnu GCC"};
constexpr auto compiler = Compiler::GCC;
#elif _MSC_VER
#define BUILDCONFIG_MSVC 1
constexpr auto compilerStr = std::string_view{"Microsoft Visual C++"};
constexpr auto compiler = Compiler::MSVC;
#else
  #error Compiler not supported.
#endif

} // namespace BuildConfig