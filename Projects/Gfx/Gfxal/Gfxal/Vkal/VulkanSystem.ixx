module;

#include <cstdint>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "Gfxal/Vkal/Vk.h"

export module Vkal.VulkanSystem;

import Vkal.DebugMsg;
import Vkal.PhysicalDevice;

namespace VkHal
{}

export namespace VkHal
{
struct VulkanSystemDesc
{
  std::string_view m_applicationName;
  std::string_view m_engineName;
  uint32_t m_applicationVersion = {};
  uint32_t m_engineVersion = {};
  bool m_enableValidation = false;
  bool m_enableDebugUtils = false;
  bool m_isVerbose = false;
  bool m_preferIGPU = false;
};

//enum class QueueType
//{
//  Graphics,
//  Compute,
//  Transfer
//};
//
//QueueType getQueueTypeFromVkQueueFlagBits(vk::QueueFlagBits queueFlagBits)
//{
//  if (queueFlagBits & vk::QueueFlagBits::eGraphics)
//  {
//    return QueueType::Graphics;
//  }
//}

class VulkanSystem
{
public:
  VulkanSystem() = default;

  VulkanSystem(VulkanSystemDesc desc);

private:
  void createInstance(VulkanSystemDesc desc, std::span<std::string_view> requestedExtensions, std::span<std::string_view> requestedLayers);
  void createPhysicalDevices();
  void createDevice(std::span<std::string_view> requestedExtensions);
  void printInstanceInfo() const;
  void printDevicesInfo() const;

  static constexpr uint32_t m_targetVulkanVersion = VK_API_VERSION_1_3;

#if (VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1)
  vk::DynamicLoader m_loader;
#endif

  vk::UniqueInstance m_instance;
  std::optional<VkHal::DebugMessenger> m_debugMessenger;
  std::vector<VkHal::PhysicalDevice> m_physicalDevices;
  VkHal::PhysicalDevice m_physicalDevice;
  vk::UniqueDevice m_device;

  std::unordered_map<vk::QueueFlagBits, std::vector<vk::Queue>> m_queuesMap;
};

} // namespace VkHal
