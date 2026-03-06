#pragma once
#include "Config/Config.h"

#if defined(CORE_PLATFORM_WINDOWS)
  #include "Core/Win32/WindowsHeader.h" // IWYU pragma: export
  #define VK_USE_PLATFORM_WIN32_KHR
#endif

#if defined(CORE_PLATFORM_LINUX)
  #define VK_USE_PLATFORM_WAYLAND_KHR
#endif

#include "vulkan/vulkan_core.h" // IWYU pragma: export
#include "vulkan/vulkan_hpp_macros.hpp" // IWYU pragma: export

#if defined(VK_USE_PLATFORM_WIN32_KHR)
  #include "vulkan/vulkan_win32.h"
#endif

#if defined(VK_USE_PLATFORM_WAYLAND_KHR)
  #include "vulkan/vulkan_wayland.h"
#endif

namespace GfxVk
{
#if (VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1)
constexpr auto vkDispathDynamic = true;
#endif

} // namespace GfxVk
