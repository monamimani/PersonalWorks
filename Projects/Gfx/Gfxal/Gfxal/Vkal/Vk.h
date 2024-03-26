#pragma once

#include "Core/Config.h"
#include "Core/Win32/WindowsHeader.h"

#if defined(CORE_PLATFORM_WINDOWS)
  #define VK_USE_PLATFORM_WIN32_KHR
#endif

// import vulkan_hpp;

#include "vulkan/vulkan.hpp"
#include "vulkan/vulkan_raii.hpp"

namespace GfxVk
{
#if (VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1)
constexpr auto vkDispathDynamic = true;
#endif

} // namespace GfxVk
