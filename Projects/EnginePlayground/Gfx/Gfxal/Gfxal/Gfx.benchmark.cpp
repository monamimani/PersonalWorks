#include "TestUtilities/GoogleBenchmark.h"

import GfxHAL;

#include "Gfxal/Vkal/Vk.h"
#include "Gfxal/Vkal/VkDefines.h"

namespace GfxBenchmark
{

static void vkCreateInstanceHpp(benchmark::State& state)
{
  for (auto _ : state)
  {
    if constexpr (GfxVk::vkDispathDynamic)
    {
      VULKAN_HPP_DEFAULT_DISPATCHER.init();
    }

    const vk::ApplicationInfo appInfo = {.pApplicationName = "VkCreateInstanceHpp",
                                         .applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0),
                                         .pEngineName = "VkCreateInstaceHppEngine",
                                         .engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0),
                                         .apiVersion = VK_API_VERSION_1_3};

    const vk::InstanceCreateInfo instanceInfo = {.pApplicationInfo = &appInfo};

#ifndef VULKAN_HPP_NO_SMART_HANDLE
    auto vkInstance = vk::createInstanceUnique(instanceInfo);

    if constexpr (GfxVk::vkDispathDynamic)
    {
      VULKAN_HPP_DEFAULT_DISPATCHER.init(vkInstance.get());
    }

// vkInstance.release();
#else
    auto vkInstance = vk::createInstance(instanceInfo);

    if constexpr (GfxVk::vkDispathDynamic)
    {
      VULKAN_HPP_DEFAULT_DISPATCHER.init(vkInstance);
    }

#endif //  VULKAN_HPP_NO_SMART_HANDLE

    benchmark::DoNotOptimize(vkInstance);
  }
}

BENCHMARK(vkCreateInstanceHpp);

static void vkCreateInstanceH(benchmark::State& state)
{
  for (auto _ : state)
  {
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "VkCreateInstanceH";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_3;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    VkInstance vkInstance = nullptr;
    VkCheck(vkCreateInstance(&createInfo, nullptr, &vkInstance));

    vkDestroyInstance(vkInstance, nullptr);

    benchmark::DoNotOptimize(vkInstance);
  }
}

BENCHMARK(vkCreateInstanceH);

static void vkCreatePhysicalDeviceHpp(benchmark::State& state)
{
  if constexpr (GfxVk::vkDispathDynamic)
  {
    VULKAN_HPP_DEFAULT_DISPATCHER.init();
  }

  const vk::ApplicationInfo appInfo = {.pApplicationName = "VkCreateInstanceHpp",
                                       .applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0),
                                       .pEngineName = "VkCreateInstaceHppEngine",
                                       .engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0),
                                       .apiVersion = VK_API_VERSION_1_3};

  const vk::InstanceCreateInfo instanceInfo = {.pApplicationInfo = &appInfo};

  auto vkInstance = vk::createInstanceUnique(instanceInfo);

  if constexpr (GfxVk::vkDispathDynamic)
  {
    VULKAN_HPP_DEFAULT_DISPATCHER.init(vkInstance.get());
  }

  for (auto _ : state)
  {
    auto physicalDevices = vkInstance->enumeratePhysicalDevices();

    struct PhysicalDeviceInfo
    {
      vk::PhysicalDevice physicalDevice;
      vk::PhysicalDeviceProperties2 physicalDeviceProperties;
      vk::PhysicalDeviceProperties properties;
      vk::PhysicalDeviceFeatures2 physicalDeviceFeatures;
      vk::PhysicalDeviceFeatures features;
      vk::PhysicalDeviceMemoryProperties2 physicalDeviceMemoryProperties;
      vk::PhysicalDeviceMemoryProperties memoryProperties;
    };

    std::vector<PhysicalDeviceInfo> physicalDeviceInfoList;
    physicalDeviceInfoList.reserve(physicalDevices.size());

    std::ranges::transform(physicalDevices, std::back_inserter(physicalDeviceInfoList), [](const vk::PhysicalDevice& physicalDevice) {
      const auto& physicalDeviceProperties = physicalDevice.getProperties2();
      const auto& physicalDeviceFeatures = physicalDevice.getFeatures2();
      const auto& physicalDeviceMemoryProperties = physicalDevice.getMemoryProperties2();
      return PhysicalDeviceInfo{.physicalDevice = physicalDevice,
                                .physicalDeviceProperties = physicalDeviceProperties,
                                .properties = physicalDeviceProperties.properties,
                                .physicalDeviceFeatures = physicalDeviceFeatures,
                                .features = physicalDeviceFeatures.features,
                                .physicalDeviceMemoryProperties = physicalDeviceMemoryProperties,
                                .memoryProperties = physicalDeviceMemoryProperties.memoryProperties};
    });

    std::ranges::sort(physicalDeviceInfoList, [](const PhysicalDeviceInfo& lhs, const PhysicalDeviceInfo& rhs) {
      auto lhsPhysicalDeviceTypeScore = lhs.properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu ? 1 : 0;
      auto rhsPhysicalDeviceTypeScore = rhs.properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu ? 1 : 0;

      if (lhsPhysicalDeviceTypeScore > rhsPhysicalDeviceTypeScore)
      {
        return true;
      }

      auto lhsMemoryHeapsSpan = std::span{lhs.memoryProperties.memoryHeaps.data(), lhs.memoryProperties.memoryHeapCount};
      auto rhsMemoryHeapsSpan = std::span{rhs.memoryProperties.memoryHeaps.data(), rhs.memoryProperties.memoryHeapCount};

      auto findDeviceLocalPred = [](const vk::MemoryHeap& memoryHeap) {
        return VkHal::flagsHasBits(memoryHeap.flags, vk::MemoryHeapFlagBits::eDeviceLocal);
      };

      const auto lhsMemoryHeapIt = std::ranges::find_if(lhsMemoryHeapsSpan, findDeviceLocalPred);
      const auto lhsDeviceLocalHeapSize = lhsMemoryHeapIt != lhsMemoryHeapsSpan.cend() ? lhsMemoryHeapIt->size : 0;
      const auto rhsMemoryHeapIt = std::ranges::find_if(rhsMemoryHeapsSpan, findDeviceLocalPred);
      const auto rhsDeviceLocalHeapSize = rhsMemoryHeapIt != rhsMemoryHeapsSpan.cend() ? rhsMemoryHeapIt->size : 0;

      return lhsDeviceLocalHeapSize > rhsDeviceLocalHeapSize && lhs.properties.limits.maxImageDimension2D > rhs.properties.limits.maxImageDimension2D;
    });

    std::vector<vk::PhysicalDevice> physicalDevicesSorted;
    physicalDevicesSorted.reserve(physicalDevices.size());
    std::ranges::transform(physicalDeviceInfoList, std::back_inserter(physicalDevicesSorted), [](const PhysicalDeviceInfo& physicalDeviceInfo) {
      return physicalDeviceInfo.physicalDevice;
    });

    benchmark::DoNotOptimize(physicalDevicesSorted);
  }
}

BENCHMARK(vkCreatePhysicalDeviceHpp);

} // namespace GfxBenchmark
