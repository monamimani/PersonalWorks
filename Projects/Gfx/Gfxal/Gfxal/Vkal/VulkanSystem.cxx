module;

#include <algorithm>
#include <exception>
#include <ranges>
#include <stdexcept>

#include "Core/Config.h"
#include "Gfxal/Vkal/Vk.h"
#include "fmt/format.h"
#include "fmt/ranges.h"
#include "vulkan/vulkan_hpp_macros.hpp"

module Vkal.VulkanSystem;

import Vkal.Utilities;

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

namespace VkHal
{
VulkanSystem::VulkanSystem(VulkanSystemDesc desc)
{
  namespace ranges = std::ranges;
  namespace views = std::ranges::views;

  if constexpr (GfxVk::vkDispathDynamic)
  {
    if (!m_loader.success())
    {
      throw std::runtime_error("Failed to load Vulkan library.");
    }
    VULKAN_HPP_DEFAULT_DISPATCHER.init(m_loader);
  }

  if constexpr (Core::isDebugBuild)
  {
    printInstanceInfo();
  }

  if (const auto vulkanVersion = vk::enumerateInstanceVersion(); vulkanVersion < m_targetVulkanVersion)
  {
    throw std::runtime_error(
        fmt::format("System does not support required version. Required version: {}, System version: {}", m_targetVulkanVersion, vulkanVersion));
  }

  auto requestedLayers = getLayers(desc.m_enableValidation);
  if (const auto result = areLayersSupported(requestedLayers); not result)
  {
    throw std::runtime_error(fmt::format("Requested Vulkan layers not suported: {}", result.error()));
  }

  auto requestedInstanceExtensions = getInstanceExtensions(desc.m_enableDebugUtils);
  if (const auto result = areInstanceExtensionsSupported(requestedInstanceExtensions); not result)
  {
    throw std::runtime_error(fmt::format("Requested Vulkan instanceExtensions not suported: {}", result.error()));
  }

  createInstance(desc, requestedInstanceExtensions, requestedLayers);

  createPhysicalDevices();

  if constexpr (Core::isDebugBuild)
  {
    printDevicesInfo();
  }

  auto requestedDeviceExtensions = getDeviceExtensions();

  createDevice(requestedDeviceExtensions);
}

void VulkanSystem::createInstance(VulkanSystemDesc desc, std::span<std::string_view> requestedExtensions, std::span<std::string_view> requestedLayers)
{

  std::vector<const char*> layersView;
  layersView.reserve(requestedLayers.size());
  std::ranges::copy(std::ranges::views::transform(requestedLayers,
                                                  [](std::string_view layerName) {
                                                    return layerName.data();
                                                  }),
                    std::back_inserter(layersView));

  std::vector<const char*> extensionsView;
  extensionsView.reserve(requestedExtensions.size());
  std::ranges::copy(std::ranges::views::transform(requestedExtensions,
                                                  [](std::string_view extName) {
                                                    return extName.data();
                                                  }),
                    std::back_inserter(extensionsView));

  const vk::ApplicationInfo appInfo = {.pApplicationName = desc.m_applicationName.data(),
                                       .applicationVersion = desc.m_applicationVersion,
                                       .pEngineName = desc.m_engineName.data(),
                                       .engineVersion = desc.m_engineVersion,
                                       .apiVersion = m_targetVulkanVersion};

  auto instanceCreateInfo = vk::InstanceCreateInfo{
      .pApplicationInfo = &appInfo,
  };
  instanceCreateInfo.setPEnabledLayerNames(layersView);
  instanceCreateInfo.setPEnabledExtensionNames(extensionsView);

  auto instanceCreateInfoChain = vk::StructureChain<vk::InstanceCreateInfo, vk::DebugUtilsMessengerCreateInfoEXT>{
      instanceCreateInfo, VkHal::DebugMessenger::makeDebugMessengerCreateInfo(desc.m_isVerbose)};

  if (not desc.m_enableDebugUtils)
  {
    instanceCreateInfoChain.unlink<vk::DebugUtilsMessengerCreateInfoEXT>();
  }

  m_instance = vk::createInstanceUnique(instanceCreateInfoChain.get());

  if constexpr (GfxVk::vkDispathDynamic)
  {
    VULKAN_HPP_DEFAULT_DISPATCHER.init(m_instance.get());
  }

  if (desc.m_enableDebugUtils)
  {
    m_debugMessenger = DebugMessenger(m_instance.get(), desc.m_isVerbose);
  }
}

void VulkanSystem::createPhysicalDevices()
{
  auto physicalDevices = m_instance->enumeratePhysicalDevices();

  m_physicalDevices.reserve(physicalDevices.size());

  std::ranges::transform(physicalDevices, std::back_inserter(m_physicalDevices), [](const vk::PhysicalDevice physicalDevice) {
    return VkHal::PhysicalDevice{physicalDevice};
  });

  std::ranges::sort(m_physicalDevices, std::ranges::greater{});
}

void VulkanSystem::createDevice(std::span<std::string_view> requestedExtensions)
{
  const auto filterLambda = [requestedExtensions](const VkHal::PhysicalDevice& physicalDevice) {
    return physicalDevice.isVulkanVersionSupported(m_targetVulkanVersion) && physicalDevice.areExtensionsSuported(requestedExtensions).has_value();
  };

  auto selectedPhysicalDevices = m_physicalDevices | std::views::filter(filterLambda) | std::ranges::to<std::vector<PhysicalDevice>>();
  if (selectedPhysicalDevices.empty())
  {
    throw std::runtime_error(fmt::format("Can't find a supported Vulkan physical device."));
  }

  m_physicalDevice = selectedPhysicalDevices[0];

  auto queueDeviceQueries = std::vector<AcquireQueueQuery>{
      {.m_queueFlagBits = vk::QueueFlagBits::eGraphics, .m_nbQueueToAcquire = 1},
      { .m_queueFlagBits = vk::QueueFlagBits::eCompute, .m_nbQueueToAcquire = 1},
      {.m_queueFlagBits = vk::QueueFlagBits::eTransfer, .m_nbQueueToAcquire = 1}
  };

  auto queueResponse = m_physicalDevice.acquireDeviceQueues(queueDeviceQueries);

  std::unordered_map<uint32_t, std::vector<float>> queueIndexToPrioritiesMap{};
  for (const auto& usageType : queueResponse.m_queueUsageType)
  {
    auto& priorities = queueIndexToPrioritiesMap[usageType.m_index];
    auto newPriorities = std::vector<float>(usageType.m_count, 1.0f);
    std::ranges::move(newPriorities, std::back_inserter(priorities));
  }

  std::vector<vk::DeviceQueueCreateInfo> deviceQueueCreateInfoList;
  deviceQueueCreateInfoList.reserve(queueResponse.m_queueAcquiredCountMap.size());

  std::ranges::transform(queueResponse.m_queueAcquiredCountMap,
                         std::back_inserter(deviceQueueCreateInfoList),
                         [&queueIndexToPrioritiesMap](const std::pair<uint32_t, uint32_t>& pairIndexCount) {
                           auto [index, count] = pairIndexCount;
                           auto& queuePrioritiesList = queueIndexToPrioritiesMap[index];
                           const auto queueCreateInfo =
                               vk::DeviceQueueCreateInfo{.queueFamilyIndex = index, .queueCount = count, .pQueuePriorities = queuePrioritiesList.data()};
                           return queueCreateInfo;
                         });

  const auto deviceFeatures = vk::PhysicalDeviceFeatures{.logicOp = true,
                                                         .multiDrawIndirect = true,
                                                         .fillModeNonSolid = true,
                                                         .depthBounds = true,
                                                         .wideLines = true,
                                                         .largePoints = true,
                                                         .shaderFloat64 = true,
                                                         .shaderInt64 = true,
                                                         .shaderInt16 = true};

  std::vector<const char*> extensionsView;
  extensionsView.reserve(requestedExtensions.size());
  std::ranges::transform(requestedExtensions, std::back_inserter(extensionsView), [](std::string_view extName) {
    return extName.data();
  });

  auto deviceCreateInfo = vk::DeviceCreateInfo{.pEnabledFeatures = &deviceFeatures};
  deviceCreateInfo.setQueueCreateInfos(deviceQueueCreateInfoList);
  deviceCreateInfo.setPEnabledExtensionNames(extensionsView);

  m_device = m_physicalDevice.createDevice(deviceCreateInfo);

  for (const auto& queueUsageType : queueResponse.m_queueUsageType)
  {
    auto& queueList = m_queuesMap[queueUsageType.m_type];
    for (uint32_t i = 0; i < queueUsageType.m_count; i++)
    {
      queueList.emplace_back(m_device->getQueue(queueUsageType.m_index, i));
    }
  }
}

void VulkanSystem::printInstanceInfo() const
{
  fmt::println("=======================================");
  fmt::println("===== Vulkan Instance Information =====");
  fmt::println("=======================================");

  const Version instanceVersion = vk::enumerateInstanceVersion();
  fmt::println("Vulkan supported instance version: {}", instanceVersion);
  fmt::println("");

  const auto instanceExtensions = vk::enumerateInstanceExtensionProperties();
  fmt::println("Instance Extensions:");
  std::ranges::for_each(instanceExtensions, [](const vk::ExtensionProperties& props) {
    fmt::println("- {} v{}", props.extensionName, props.specVersion);
  });
  fmt::println("");

  auto instanceLayers = vk::enumerateInstanceLayerProperties();
  std::ranges::sort(instanceLayers, std::ranges::less{}, &vk::LayerProperties::layerName);
  fmt::println("Instance Layers({}): ", instanceLayers.size());
  std::ranges::for_each(instanceLayers, [](const vk::LayerProperties& props) {
    fmt::println("- {} v{} (Vulkan {}) - {}", props.layerName, props.implementationVersion, Version(props.specVersion), props.description);

    auto layerExtensions = vk::enumerateInstanceExtensionProperties(std::string{props.layerName.data()});
    std::ranges::for_each(layerExtensions, [](const vk::ExtensionProperties& props) {
      fmt::println("  - {} v{}", props.extensionName, props.specVersion);
    });
  });
  fmt::println("");
  fmt::println("===========================================");
  fmt::println("===== End Vulkan Instance Information =====");
  fmt::println("===========================================");
}

void VulkanSystem::printDevicesInfo() const
{
  fmt::println("=====================================");
  fmt::println("===== Vulkan Device Information =====");
  fmt::println("=====================================");

  for (const auto& physicalDevice : m_physicalDevices)
  {
    physicalDevice.printDeviceInfo();
    fmt::println("");
  }

  fmt::println("=========================================");
  fmt::println("===== End Vulkan Device Information =====");
  fmt::println("=========================================");
}
} // namespace VkHal
