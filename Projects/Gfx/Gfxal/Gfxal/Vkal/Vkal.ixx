module;

#include <algorithm>
#include <ranges>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "Core/Exception.h"
#include "Gfxal/Vkal/Vk.h"
#include "TestUtilities/TestsFriend.h"

export module VkHal;

export import Vkal.DebugMsg;
export import Vkal.PhysicalDevice;

namespace ranges = std::ranges;
namespace views = std::ranges::views;

TEST_FRIEND_FORWARD_DECLARE_NS(GfxTest, BasicVulkanTestsF, VkCreateInstance);

namespace VkHal
{
const auto g_vkExtensionProjName = [](const vk::ExtensionProperties& extensionProp) {
  return std::string_view{extensionProp.extensionName};
};
std::vector<vk::ExtensionProperties> m_instanceExtensionProperties;

const auto g_vkLayerProjName = [](const vk::LayerProperties& layerProp) {
  return std::string_view{layerProp.layerName};
};

[[nodiscard]] std::vector<std::string> getDeviceExtensions()
{
  return {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
}

[[nodiscard]] std::vector<std::string_view> getInstanceExtensions()
{
  std::vector<std::string_view> extensions;
  extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
  extensions.push_back(VK_KHR_ANDROID_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_IOS_MVK)
  extensions.push_back(VK_MVK_IOS_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_MACOS_MVK)
  extensions.push_back(VK_MVK_MACOS_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_MIR_KHR)
  extensions.push_back(VK_KHR_MIR_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_VI_NN)
  extensions.push_back(VK_NN_VI_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
  extensions.push_back(VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_WIN32_KHR)
  extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_XCB_KHR)
  extensions.push_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_XLIB_KHR)
  extensions.push_back(VK_KHR_XLIB_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_XLIB_XRANDR_EXT)
  extensions.push_back(VK_EXT_ACQUIRE_XLIB_DISPLAY_EXTENSION_NAME);
#endif
  return extensions;
}

} // namespace VkHal

export namespace VkHal
{

struct VkSystemDesc
{
  std::string m_applicationName;
  uint32_t m_applicationVersion = {};
  std::string m_engineName;
  uint32_t m_engineVersion = {};
};

class VkSystem
{
  TEST_FRIEND_NS(GfxTest, BasicVulkanTestsF, VkCreateInstance);

public:
  VkSystem() = default;

  VkSystem(const VkSystemDesc& systemDesc)
  {
    if constexpr (GfxVk::vkDispathDynamic)
    {
      VULKAN_HPP_DEFAULT_DISPATCHER.init();
    }

    const vk::ApplicationInfo appInfo = {.pApplicationName = systemDesc.m_applicationName.data(),
                                         .applicationVersion = systemDesc.m_applicationVersion,
                                         .pEngineName = systemDesc.m_engineName.data(),
                                         .engineVersion = systemDesc.m_engineVersion,
                                         .apiVersion = m_targetVulkanVersion};

    m_instanceExtensionProperties = vk::enumerateInstanceExtensionProperties();
    ranges::sort(m_instanceExtensionProperties, ranges::less(), g_vkExtensionProjName);

    m_instanceLayerProperties = vk::enumerateInstanceLayerProperties();
    ranges::sort(m_instanceLayerProperties, ranges::less(), g_vkLayerProjName);

    m_requestedextensions = getInstanceExtensions();

    if constexpr (Core::BuildKind != Core::BuildKind::Shipping)
    {
      static const auto vkLayerKhronosValidationName = "VK_LAYER_KHRONOS_validation";
      // Enable standard validation layer to find as much errors as possible!
      if (std::none_of(m_requestedlayers.begin(), m_requestedlayers.end(), [](std::string_view layer) {
            return layer == vkLayerKhronosValidationName;
          }))
      {
        m_requestedlayers.push_back(vkLayerKhronosValidationName);
      }

      static const auto vkExtDebugUtilsExtName = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
      if (std::none_of(m_requestedextensions.begin(), m_requestedextensions.end(), [](std::string_view layer) {
            return layer == vkExtDebugUtilsExtName;
          }))
      {
        m_requestedextensions.push_back(vkExtDebugUtilsExtName);
      }
    }

    std::tie(m_supportedLayers, m_unsupportedLayers) = validateLayers(m_requestedlayers);

    if (!m_unsupportedLayers.empty())
    {
      std::terminate();
    }

    std::vector<const char*> layersView;
    layersView.reserve(m_requestedlayers.size());
    ranges::copy(views::transform(m_supportedLayers,
                                  [](std::string_view layerName) {
                                    return layerName.data();
                                  }),
                 std::back_inserter(layersView));

    std::tie(m_supportedExtensions, m_unsupportedExtensions) = gatherSuportedAndUnsuportedExtensions(m_requestedextensions);

    if (!m_unsupportedExtensions.empty())
    {
      std::terminate();
    }

    std::vector<const char*> extensionsView;
    extensionsView.reserve(m_supportedExtensions.size());
    ranges::copy(views::transform(m_supportedExtensions,
                                  [](std::string_view extName) {
                                    return extName.data();
                                  }),
                 std::back_inserter(extensionsView));

    const auto instanceInfoChain = makeInstanceCreateInfoChain(appInfo, layersView, extensionsView);

    m_instance = vk::createInstanceUnique(instanceInfoChain.get<vk::InstanceCreateInfo>());

    if constexpr (GfxVk::vkDispathDynamic)
    {
      VULKAN_HPP_DEFAULT_DISPATCHER.init(m_instance.get());
    }

    if constexpr (Core::BuildKind != Core::BuildKind::Shipping)
    {
      m_debugMessenger = DebugMessenger(m_instance.get(), true);
    }
  }

private:
  [[nodiscard]] auto validateExtension(std::vector<std::string> requestedExtensions)
  {
    ranges::sort(requestedExtensions, ranges::less());

    return ranges::includes(m_instanceExtensionProperties, requestedExtensions, ranges::less(), g_vkExtensionProjName);
  }

  [[nodiscard]] auto gatherSuportedAndUnsuportedExtensions(std::span<std::string_view> requestedExtensions)
  {
    ranges::sort(requestedExtensions);

    std::vector<std::string> supportedExtensions;
    std::vector<std::string> unsupportedExtensions;
    supportedExtensions.reserve(requestedExtensions.size());
    unsupportedExtensions.reserve(requestedExtensions.size());

    for (const auto& extensionName : requestedExtensions)
    {
      if (ranges::contains(m_instanceExtensionProperties, extensionName, g_vkExtensionProjName))
      {
        supportedExtensions.emplace_back(extensionName);
      }
      else
      {
        unsupportedExtensions.emplace_back(extensionName);
      }
    }

    return std::make_tuple(supportedExtensions, unsupportedExtensions);
  }

  [[nodiscard]] auto validateLayers(std::span<std::string_view> requestedLayers)
  {
    ranges::sort(requestedLayers);

    std::vector<std::string> supportedLayers;
    std::vector<std::string> unsupportedLayers;

    supportedLayers.reserve(requestedLayers.size());
    unsupportedLayers.reserve(requestedLayers.size());

    for (const auto& layerName : requestedLayers)
    {
      if (ranges::contains(m_instanceLayerProperties, layerName, g_vkLayerProjName))
      {
        supportedLayers.emplace_back(layerName);
      }
      else
      {
        unsupportedLayers.emplace_back(layerName);
      }
    }

    return std::make_tuple(supportedLayers, unsupportedLayers);
  }

  [[nodiscard]] static auto makeInstanceCreateInfoChain(const vk::ApplicationInfo& appInfo,
                                                        const std::vector<const char*>& layers,
                                                        const std::vector<const char*>& extensions)
  {
    auto instanceCreateInfo = vk::InstanceCreateInfo{
        .pApplicationInfo = &appInfo,
    };
    instanceCreateInfo.setPEnabledLayerNames({static_cast<uint32_t>(layers.size()), layers.data()});
    instanceCreateInfo.setPEnabledExtensionNames({static_cast<uint32_t>(extensions.size()), extensions.data()});

    if constexpr (Core::BuildKind == Core::BuildKind::Shipping)
    {
      auto instanceCreateInfoChain = vk::StructureChain<vk::InstanceCreateInfo>{instanceCreateInfo};

      return instanceCreateInfoChain;
    }
    else
    {

      vk::StructureChain<vk::InstanceCreateInfo, vk::DebugUtilsMessengerCreateInfoEXT> instanceCreateInfoChain(
          instanceCreateInfo, VkHal::DebugMessenger::makeDebugMessengerCreateInfo(false));

      return instanceCreateInfoChain;
    }
  }

  static constexpr uint32_t m_targetVulkanVersion = VK_API_VERSION_1_3;

  std::vector<std::string_view> m_requestedlayers;
  std::vector<std::string_view> m_requestedextensions;
  std::vector<std::string> m_supportedLayers;
  std::vector<std::string> m_unsupportedLayers;
  std::vector<std::string> m_supportedExtensions;
  std::vector<std::string> m_unsupportedExtensions;

  std::vector<vk::LayerProperties> m_instanceLayerProperties;
  std::vector<VkHal::PhysicalDevice> m_physicalDevices;

  vk::UniqueInstance m_instance = {};
  std::optional<VkHal::DebugMessenger> m_debugMessenger;
};

} // namespace VkHal
