#pragma once

#include "Config/Config.h"

#if defined(CORE_PLATFORM_WINDOWS)
  #include "Core/Win32/WindowsHeader.h"
  #define VK_USE_PLATFORM_WIN32_KHR
#endif

#include "vulkan/vulkan.hpp"
//#include "vulkan/vulkan_raii.hpp"

//import vulkan_hpp;


namespace GfxVk
{
#if (VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1)
constexpr auto vkDispathDynamic = true;
#endif

} // namespace GfxVk
