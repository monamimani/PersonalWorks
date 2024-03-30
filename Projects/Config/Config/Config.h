#pragma once

#include "Config/ConfigGenerated.h"
#include <cstdint>

#if defined(NDEBUG)
  #ifdef CONFIG_DEBUG
    #error "NDEBUG is defined but CONFIG_DEBUG is defined."
  #endif

  #ifndef CONFIG_RELEASE
    #error "NDEBUG is defined but CONFIG_RELEASE is not defined."
  #endif
#endif

#if not defined(NDEBUG)
  #ifndef CONFIG_DEBUG
    #error "NDEBUG is not defined CONFIG_DEBUG is not defined."
  #endif

  #ifdef CONFIG_RELEASE
    #error "NDEBUG is not defined but CONFIG_RELEASE is defined."
  #endif
#endif

#if defined(CONFIG_TESTS) and defined(BUILD_KIND_SHIPPING)
    #error "CONFIG_TESTS is defined in a BUILD_KIND_SHIPPING build."
#endif

namespace Config
{
enum class BuildKind
{
  Development,
  QA,
  Profile,
  Shipping
};

enum class BuildType
{
  Debug,
  Release
};

#ifdef CONFIG_TESTS
constexpr auto isTestBuild = true;
#else
constexpr auto isTestBuild = false;
#endif

#ifdef CONFIG_DEBUG
inline constexpr auto isDebugBuild = true;
inline constexpr auto BuildType = BuildType::Debug;
#endif

#ifdef CONFIG_RELEASE
inline constexpr auto isDebugBuild = false;
inline constexpr auto BuildType = BuildType::Release;
#endif

inline constexpr auto isOptimizedBuild = !isDebugBuild;

#ifdef CONFIG_DEV
inline constexpr auto BuildKind = BuildKind::Development;
#elif CONFIG_QA
inline constexpr auto BuildKind = BuildKind::QA;
#elif CONFIG_PROFILE
inline constexpr auto BuildKind = BuildKind::Profile;
#elif CONFIG_SHIPPING
inline constexpr auto BuildKind = BuildKind::Shipping;
#else
  #error "Build kind is undefined"
#endif

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

} // namespace Config
