module;

#include <array>
#include <expected>
#include <ranges>
#include <source_location>
#include <stacktrace>
#include <string>

#include "Core/Exception.h"
#include "Gfxal/Vkal/Vk.h"
#include "fmt/core.h"
#include "fmt/format.h"

export module Vkal.Utilities;

namespace VkHal
{
[[nodiscard]] bool operator==(const std::string_view lhs, const vk::ArrayWrapper1D<char, VK_MAX_EXTENSION_NAME_SIZE>& rhs)
{
  return lhs == static_cast<std::string_view>(rhs);
}
} // namespace VkHal

export namespace VkHal
{
template<typename BitType>
bool flagsHasBits(vk::Flags<BitType> flags, BitType bits)
{
  return (flags & bits) == bits;
}

inline void vkResultCheck(vk::Result result,
                          const std::source_location& loc = std::source_location::current(),
                          const std::stacktrace& trace = std::stacktrace::current())
{
  if (result != vk::Result::eSuccess)
  {
    auto resultData = static_cast<std::underlying_type_t<vk::Result>>(result);
    // std::terminate();
    throw Core::Exception(resultData, vk::to_string(result), loc, trace);
  }
}

inline void vkResultCheck(VkResult result,
                          const std::source_location& loc = std::source_location::current(),
                          const std::stacktrace& trace = std::stacktrace::current())
{
  return vkResultCheck(static_cast<vk::Result>(result), loc, trace);
}

[[nodiscard]] auto areExtensionsSupported(std::span<const vk::ExtensionProperties> supportedExtensions, std::span<std::string_view> requestedExtensions)
    -> std::expected<bool, std::vector<std::string_view>>
{
  std::vector<std::string_view> notSupportedExtensions;

  std::ranges::for_each(requestedExtensions, [&supportedExtensions, &notSupportedExtensions](std::string_view requestedExtensionName) {
    if (not std::ranges::contains(supportedExtensions, requestedExtensionName, [](const vk::ExtensionProperties& extensionProperty) {
          return static_cast<std::string_view>(extensionProperty.extensionName);
        }))
    {
      notSupportedExtensions.push_back(requestedExtensionName);
    }
  });

  if (notSupportedExtensions.size() == 0)
  {
    return true;
  }

  return std::unexpected(notSupportedExtensions);
}

[[nodiscard]] const auto& getSupportedInstanceExtensionProperties()
{
  static auto instanceExtenstions = [] {
    auto instanceExtensionProperties = vk::enumerateInstanceExtensionProperties();
    std::ranges::sort(instanceExtensionProperties, std::ranges::less(), &vk::ExtensionProperties::extensionName);
    return instanceExtensionProperties;
  }();

  return instanceExtenstions;
}

[[nodiscard]] auto areInstanceExtensionsSupported(std::span<std::string_view> requestedExtensions) -> std::expected<bool, std::vector<std::string_view>>
{
  std::vector<vk::ExtensionProperties> supportedExtensions = getSupportedInstanceExtensionProperties();
  return areExtensionsSupported(supportedExtensions, requestedExtensions);
}

[[nodiscard]] const auto& getSupportedInstanceLayerProperties()
{
  static auto instanceLayers = [] {
    auto instanceLayerProperties = vk::enumerateInstanceLayerProperties();
    std::ranges::sort(instanceLayerProperties, std::ranges::less(), [](const vk::LayerProperties& layerProperty) {
      return static_cast<std::string_view>(layerProperty.layerName);
    });
    return instanceLayerProperties;
  }();

  return instanceLayers;
}

[[nodiscard]] auto areLayersSupported(std::span<std::string_view> requestedLayers) -> std::expected<bool, std::vector<std::string_view>>
{
  const auto supportedLayers = getSupportedInstanceLayerProperties();
  std::vector<std::string_view> notSupportedLayers;

  std::ranges::for_each(requestedLayers, [&supportedLayers, &notSupportedLayers](std::string_view requestedLayerName) {
    if (not std::ranges::contains(supportedLayers, requestedLayerName, [](const vk::LayerProperties& layerProperty) {
          return static_cast<std::string_view>(layerProperty.layerName);
        }))
    {
      notSupportedLayers.push_back(requestedLayerName);
    }
  });

  if (notSupportedLayers.size() == 0)
  {
    return true;
  }

  return std::unexpected(notSupportedLayers);
}

[[nodiscard]] auto getDeviceExtensions()
{
  auto extensions = std::vector<std::string_view>({VK_KHR_SWAPCHAIN_EXTENSION_NAME, VK_EXT_MEMORY_BUDGET_EXTENSION_NAME});
  std::ranges::sort(extensions);
  return extensions;
}

[[nodiscard]] auto getInstanceExtensions(bool enableDebugUtils)
{
  std::vector<std::string_view> extensions = {
    VK_KHR_SURFACE_EXTENSION_NAME,
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
    VK_KHR_ANDROID_SURFACE_EXTENSION_NAME,
#elif defined(VK_USE_PLATFORM_IOS_MVK)
    VK_MVK_IOS_SURFACE_EXTENSION_NAME,
#elif defined(VK_USE_PLATFORM_MACOS_MVK)
    VK_MVK_MACOS_SURFACE_EXTENSION_NAME,
#elif defined(VK_USE_PLATFORM_MIR_KHR)
    VK_KHR_MIR_SURFACE_EXTENSION_NAME,
#elif defined(VK_USE_PLATFORM_VI_NN)
    VK_NN_VI_SURFACE_EXTENSION_NAME,
#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
    VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME,
#elif defined(VK_USE_PLATFORM_WIN32_KHR)
    VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#elif defined(VK_USE_PLATFORM_XCB_KHR)
    VK_KHR_XCB_SURFACE_EXTENSION_NAME,
#elif defined(VK_USE_PLATFORM_XLIB_KHR)
    VK_KHR_XLIB_SURFACE_EXTENSION_NAME,
#elif defined(VK_USE_PLATFORM_XLIB_XRANDR_EXT)
    VK_EXT_ACQUIRE_XLIB_DISPLAY_EXTENSION_NAME,
#endif
  };

  if (enableDebugUtils)
  {
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  }

  // std::ranges::sort(extensions);
  return extensions;
}

[[nodiscard]] auto getLayers(bool enableValidation)
{
  std::vector<std::string_view> layers;
  if (enableValidation)
  {
    // static const auto vkLayerKhronosValidationName = "VK_LAYER_KHRONOS_validation";
    layers.push_back("VK_LAYER_KHRONOS_validation");
  }
  // std::ranges::sort(layers);
  return layers;
}

struct Version
{
  Version() = default;

  Version(uint32_t value)
  : m_value(value)
  {}

  uint32_t m_value = 0;
};

struct Size
{
  Size() = default;

  Size(vk::DeviceSize value)
  : m_value(value)
  {}

  vk::DeviceSize m_value = 0;
};

} // namespace VkHal

template<>
struct fmt::formatter<vk::ArrayWrapper1D<char, VK_MAX_EXTENSION_NAME_SIZE>>: fmt::formatter<std::string_view>
{
  auto format(const vk::ArrayWrapper1D<char, VK_MAX_EXTENSION_NAME_SIZE>& arrayChar, format_context& ctx) const
  {
    return fmt::format_to(ctx.out(), "{}", static_cast<std::string_view>(arrayChar));
  }
};

template<>
struct fmt::formatter<VkHal::Version>: fmt::formatter<std::string>
{
  auto format(const VkHal::Version& version, format_context& ctx) const
  {
    return fmt::format_to(
        ctx.out(), "{}.{}.{}", VK_API_VERSION_MAJOR(version.m_value), VK_API_VERSION_MINOR(version.m_value), VK_API_VERSION_PATCH(version.m_value));
  }
};

template<>
struct fmt::formatter<VkHal::Size>: fmt::formatter<std::string>
{
  auto format(const VkHal::Size byteSize, format_context& ctx) const -> decltype(ctx.out())
  {
    constexpr const vk::DeviceSize kib = 1'024;
    constexpr const vk::DeviceSize mib = kib * 1'024;
    constexpr const vk::DeviceSize gib = mib * 1'024;
    const auto size = byteSize.m_value;

    if (size >= gib)
    {
      return fmt::format_to(ctx.out(), "{:.2f} GiB", float(size) / gib);
    }
    else if (size >= mib)
    {
      return fmt::format_to(ctx.out(), "{:.2f} MiB", float(size) / mib);
    }
    else
    {
      return fmt::format_to(ctx.out(), "{} B", size);
    }
  }
};

template<typename T>
concept HasVkToString = requires(T t) {
  {
    vk::to_string(t)
  };
};

template<typename T>
requires(HasVkToString<T>)
struct fmt::formatter<T>: fmt::formatter<std::string>
{
  auto format(const T value, format_context& ctx) const -> decltype(ctx.out())
  {
    return fmt::format_to(ctx.out(), "{}", vk::to_string(value));
  }
};

// template<>
// struct fmt::formatter<vk::Extent2D>: std::formatter<std::string>
//{
//   auto format(const vk::Extent2D extent, format_context& ctx) const -> decltype(ctx.out())
//   {
//     return format_to(ctx.out(), "{} x {}", extent.width, extent.height);
//   }
// };
//
// template<>
// struct fmt::formatter<vk::Extent3D>: std::formatter<std::string>
//{
//   auto format(const vk::Extent3D extent, format_context& ctx) const -> decltype(ctx.out())
//   {
//     return format_to(ctx.out(), "{} x {} x {}", extent.width, extent.height, extent.depth);
//   }
// };
