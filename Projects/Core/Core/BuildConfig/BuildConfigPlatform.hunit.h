#pragma once

#include <string_view>

namespace BuildConfig
{
//Should I rename to OS since it represent more the operating system and the platform.
enum class Platform
{
  Android,
  Linux,
  Windows
};

#ifdef __linux__
constexpr auto platformStr = std::string_view{"Linux"};
constexpr auto platform = Platform::Linux
};
#elif defined __ANDROID__
constexpr auto platformStr = std::string_view{"Android"};
constexpr auto platform = Platform::Android;
#elif defined _WIN32
constexpr auto platformStr = std::string_view{"Windows"};
constexpr auto platform = Platform::Windows;
#else
  #error Platform not supported.
#endif

}