#pragma once

#include <string_view>

#include "Core/ConfigGenerated.h"
#include "Core/Endian.h"

namespace Core
{

enum class BuildKind
{
  Debug,
  Release,
  Shipping
};

#ifdef CORE_IS_DEBUG
constexpr auto isDebugBuild = true;
constexpr auto BuildKind = BuildKind::Debug;
#elif CORE_IS_RELEASE
constexpr auto isDebugBuild = false;
constexpr auto BuildKind = BuildKind::Release;
#elif CORE_IS_SHIPPING
constexpr auto isDebugBuild = false;
constexpr auto BuildKind = BuildKind::Shipping;
#else
  #error "Build kind is undefined"
#endif

constexpr auto isOptimizedBuild = !isDebugBuild;

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
constexpr auto compilerStr = std::string_view{"Microsoft Visual C++"};
constexpr auto compiler = Compiler::MSVC;
#else
  #error Compiler not supported.
#endif

enum class Platform
{
  Android,
  Linux,
  Windows
};

#ifdef __linux__
constexpr auto platformStr = std::string_view{"Linux"};
constexpr auto platform = Platform::Linux;

  #ifndef CORE_PLATFORM_LINUX
    #define CORE_PLATFORM_LINUX
  #endif

#elif __ANDROID__
constexpr auto platformStr = std::string_view{"Android"};
constexpr auto platform = Platform::Android;

  #ifndef CORE_PLATFORM_ANDROID
    #define CORE_PLATFORM_ANDROID
  #endif

#elif _WIN32
constexpr auto platformStr = std::string_view{"Windows"};
constexpr auto platform = Platform::Windows;

  #ifndef CORE_PLATFORM_WINDOWS
    #define CORE_PLATFORM_WINDOWS
  #endif

#else
  #error Platform not supported.
#endif

// if constexpr (platform == Platform::Windows)
//{
//
// }
// else if constexpr (platform == Platform::Linux)
//{
// }

consteval uint32_t makeVersion(uint8_t major, uint8_t minor, uint8_t patch)
{
  return (static_cast<uint32_t>(major) << 22U) | (static_cast<uint32_t>(minor) << 12U) | static_cast<uint32_t>(patch);
}

} // namespace Core
