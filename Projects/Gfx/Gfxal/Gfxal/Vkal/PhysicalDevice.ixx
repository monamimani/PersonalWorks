module;

#include <algorithm>
#include <atomic>
#include <bitset>
#include <climits>
#include <cmath>
#include <compare>
#include <concepts>
#include <expected>
#include <iterator>
#include <ranges>
#include <span>
#include <tuple>
#include <unordered_map>

#include "Core/EnumClassFlags.h"
#include "Core/Exception.h"
#include "Gfxal/Vkal/Vk.h"
#include "fmt/format.h"

export module Vkal.PhysicalDevice;

import Core;
import Vkal.Utilities;

namespace VkHal
{}

export namespace VkHal
{

using QueueFamilyIndex = uint32_t;

struct QueueProperties
{
  QueueFamilyIndex m_index = uint32_t(-1);
  vk::QueueFlags m_queueFlags = {};
  uint32_t m_queueCount = {};
  uint32_t m_timestampNbValidBits = {};
  vk::Extent3D m_minImageTransferGranularity = {};

  bool isGraphicsQueue() const
  {
    return supportQueueOperations(vk::QueueFlagBits::eGraphics);
  }

  bool isComputeQueue() const
  {
    return supportQueueOperations(vk::QueueFlagBits::eCompute);
  }

  bool isTransferQueue() const
  {
    return supportQueueOperations(vk::QueueFlagBits::eTransfer);
  }

  bool supportQueueOperations(vk::QueueFlagBits queueFlagBits) const
  {
    return bool(m_queueFlags & queueFlagBits);
  }

  void printQueuePropertiesInfo() const
  {
    fmt::println("{:>2}:  Count: {}, Operations {}, Timestamp {} bits, {}x{}x{} Transfer Granularity",
                 m_index,
                 m_queueCount,
                 m_queueFlags,
                 m_timestampNbValidBits,
                 m_minImageTransferGranularity.width,
                 m_minImageTransferGranularity.height,
                 m_minImageTransferGranularity.depth);
  }

  bool operator==(const QueueProperties&) const = default;

  friend auto operator<=>(const QueueProperties& lhs, const QueueProperties& rhs)
  {
    constexpr auto BitCount = sizeof(vk::QueueFlags::MaskType) * CHAR_BIT;
    auto lhsBitsetCount = std::bitset<BitCount>{static_cast<vk::QueueFlags::MaskType>(lhs.m_queueFlags)}.count();
    auto rhsBitsetCount = std::bitset<BitCount>{static_cast<vk::QueueFlags::MaskType>(rhs.m_queueFlags)}.count();

    if (const auto bitsetCountCmp = lhsBitsetCount <=> rhsBitsetCount; bitsetCountCmp != 0)
    {
      return bitsetCountCmp;
    }

    return lhs.m_queueCount <=> rhs.m_queueCount;
  }
};

struct AcquireQueueQuery
{
  vk::QueueFlagBits m_queueFlagBits = {};
  uint32_t m_nbQueueToAcquire = {};
};

struct AcquireQueueResponse
{
  struct QueueUsageInfo
  {
    uint32_t m_index{};
    vk::QueueFlagBits m_type{};
    uint32_t m_count{};
  };

  std::vector<QueueUsageInfo> m_queueUsageType;
  std::unordered_map<uint32_t, uint32_t> m_queueAcquiredCountMap;
};

struct QueueFamilies
{
  QueueFamilies() = default;

  QueueFamilies(const std::vector<vk::QueueFamilyProperties>& queuePropertiesList)
  {
    m_queuePropertiesList.reserve(queuePropertiesList.size());

    std::ranges::transform(queuePropertiesList | std::ranges::views::enumerate,
                           std::back_inserter(m_queuePropertiesList),
                           [](std::tuple<ptrdiff_t, vk::QueueFamilyProperties> tuple) {
                             const auto& [index, props] = tuple;
                             return QueueProperties{.m_index = static_cast<uint32_t>(index),
                                                    .m_queueFlags = props.queueFlags,
                                                    .m_queueCount = props.queueCount,
                                                    .m_timestampNbValidBits = props.timestampValidBits,
                                                    .m_minImageTransferGranularity = props.minImageTransferGranularity};
                           });

    std::ranges::sort(m_queuePropertiesList, std::ranges::greater());
  }

  template<typename... Ext_T>
  QueueFamilies(const std::vector<vk::StructureChain<vk::QueueFamilyProperties2, Ext_T...>>& queuePropertiesList)
  {
    m_queuePropertiesList.reserve(queuePropertiesList.size());

    std::ranges::transform(queuePropertiesList | std::ranges::views::enumerate,
                           std::back_inserter(m_queuePropertiesList),
                           [](std::tuple<ptrdiff_t, vk::StructureChain<vk::QueueFamilyProperties2, Ext_T...>> tuple) {
                             const auto& [index, props2] = tuple;
                             const auto& props = props2.get<vk::QueueFamilyProperties2>().queueFamilyProperties;
                             return QueueProperties{.m_index = static_cast<uint32_t>(index),
                                                    .m_queueFlags = props.queueFlags,
                                                    .m_queueCount = props.queueCount,
                                                    .m_timestampNbValidBits = props.timestampValidBits,
                                                    .m_minImageTransferGranularity = props.minImageTransferGranularity};
                           });

    std::ranges::sort(m_queuePropertiesList, std::ranges::greater());
  }

  auto getQueueFamilies(vk::QueueFlagBits queueFlagBits) const
  {
    const auto pred = [queueFlagBits](const QueueProperties& props) {
      return props.supportQueueOperations(queueFlagBits);
    };

    auto queueFamilyList = m_queuePropertiesList | std::views::filter(pred) | std::ranges::to<std::vector<QueueProperties>>();

    std::ranges::sort(queueFamilyList, [](const QueueProperties& lhs, const QueueProperties& rhs) {
      constexpr auto BitCount = sizeof(vk::QueueFlags::MaskType) * CHAR_BIT;
      auto lhsBitsetCount = std::bitset<BitCount>{static_cast<vk::QueueFlags::MaskType>(lhs.m_queueFlags)}.count();
      auto rhsBitsetCount = std::bitset<BitCount>{static_cast<vk::QueueFlags::MaskType>(rhs.m_queueFlags)}.count();

      if (lhsBitsetCount < rhsBitsetCount)
      {
        return true;
      }

      if (lhs.m_queueCount > rhs.m_queueCount)
      {
        return true;
      }

      if (lhs.m_timestampNbValidBits > rhs.m_timestampNbValidBits)
      {
        return true;
      }

      return lhs.m_minImageTransferGranularity > rhs.m_minImageTransferGranularity;
    });

    return queueFamilyList;
  }

  [[nodiscard]] auto acquireDeviceQueues(std::span<AcquireQueueQuery> queueQueries)
  {
    AcquireQueueResponse result;

    for (const auto query : queueQueries)
    {
      auto nbQueuesToAcquire = query.m_nbQueueToAcquire;

      const auto& queueFamilies = getQueueFamilies(query.m_queueFlagBits);

      for (const auto& queuefamily : queueFamilies)
      {
        auto& queueAcquiredCount = result.m_queueAcquiredCountMap.try_emplace(queuefamily.m_index, 0).first->second;

        if (queueAcquiredCount < queuefamily.m_queueCount)
        {
          auto usageInfo = AcquireQueueResponse::QueueUsageInfo{.m_index = queuefamily.m_index,
                                                                .m_type = query.m_queueFlagBits,
                                                                .m_count = std::min(nbQueuesToAcquire, queuefamily.m_queueCount - queueAcquiredCount)};

          result.m_queueUsageType.push_back(usageInfo);

          queueAcquiredCount += usageInfo.m_count;
          nbQueuesToAcquire -= usageInfo.m_count;
        }

        if (nbQueuesToAcquire == 0)
        {
          break;
        }
      }

      if (nbQueuesToAcquire > 0)
      {
        throw Core::Exception{result, fmt::format("Failed to Acquire all the queues.")};
      }
    }

    return result;
  }

  void ReleaseQueue([[maybe_unused]] QueueFamilyIndex index, [[maybe_unused]] uint32_t queueCount)
  {
    std::unreachable();
  }

  void printQueFamiliesInfo() const
  {
    fmt::println("Queue Families ({}):", m_queuePropertiesList.size());
    for (auto element : m_queuePropertiesList)
    {
      element.printQueuePropertiesInfo();
    }
  }

  std::vector<QueueProperties> m_queuePropertiesList;
  std::unordered_map<uint32_t, uint32_t> m_queueAcquiredCountMap = {};
};

class PhysicalDevice
{

public:
  PhysicalDevice() = default;

  PhysicalDevice(const vk::PhysicalDevice& physicalDevice)
  : m_physicalDevice{physicalDevice}
  , m_extensions{physicalDevice.enumerateDeviceExtensionProperties()}
  , m_queueFamilies{getQueueFamilyPropertiesChain()}
  {
  }

  bool isVulkanVersionSupported(uint32_t requestedVulkanVersion) const
  {
    return getProperties().apiVersion > requestedVulkanVersion;
  }

  [[nodiscard]] auto areExtensionsSuported(std::span<std::string_view> requestedExtensions) const
  {
    return areExtensionsSupported(m_extensions, requestedExtensions);
  }

  [[nodiscard]] auto acquireDeviceQueues(std::span<AcquireQueueQuery> queueQueries)
  {
    return m_queueFamilies.acquireDeviceQueues(queueQueries);
  }

  void ReleaseQueue(QueueFamilyIndex index, uint32_t queueCount)
  {
    m_queueFamilies.ReleaseQueue(index, queueCount);
  }

  vk::UniqueDevice createDevice(const vk::DeviceCreateInfo& deviceCreateInfo)
  {
    return m_physicalDevice.createDeviceUnique(deviceCreateInfo);
  }

  void printDeviceInfo() const;

  friend auto operator==(const PhysicalDevice& lhs, const PhysicalDevice& rhs);
  friend auto operator<=>(const PhysicalDevice& lhs, const PhysicalDevice& rhs);

private:
  template<typename... Ext_T>
  [[nodiscard]] auto getPropertiesChain() const
  {
    vk::StructureChain<vk::PhysicalDeviceProperties2, Ext_T...> propertiesChain;
    m_physicalDevice.getProperties2(&propertiesChain.get());
    return propertiesChain;
  }

  [[nodiscard]] auto getProperties() const
  {
    return getPropertiesChain().get().properties;
  }

  [[nodiscard]] auto getLimits() const
  {
    return getProperties().limits;
  }

  template<typename... Ext_T>
  [[nodiscard]] auto getFeaturesChain() const
  {
    vk::StructureChain<vk::PhysicalDeviceFeatures2, Ext_T...> featuresChain;
    m_physicalDevice.getFeatures2(&featuresChain.get());
    return featuresChain;
  }

  [[nodiscard]] auto getFeatures() const
  {
    return getFeaturesChain().get().features;
  }

  template<typename... Ext_T>
  [[nodiscard]] auto getMemoryPropertiesChain() const
  {
    vk::StructureChain<vk::PhysicalDeviceMemoryProperties2, Ext_T...> chain;
    m_physicalDevice.getMemoryProperties2(&chain.get());
    return chain;
  }

  [[nodiscard]] auto getMemoryProperties() const
  {
    return getMemoryPropertiesChain().get().memoryProperties;
  }

  [[nodiscard]] auto getMemoryHeaps() const
  {
    const auto& memoryProps = getMemoryProperties();
    return std::vector{
        std::from_range, std::span{memoryProps.memoryHeaps.data(), memoryProps.memoryHeapCount}
    };
  }

  [[nodiscard]] auto getMemoryTypes() const
  {
    const auto& memoryProps = getMemoryProperties();
    return std::vector{
        std::from_range, std::span{memoryProps.memoryTypes.data(), memoryProps.memoryTypeCount}
    };
  }

  template<typename... Ext_T>
  [[nodiscard]] auto getQueueFamilyPropertiesChain() const
  {
    using ChainType = vk::StructureChain<vk::QueueFamilyProperties2, Ext_T...>;
    using AllocatorType = std::vector<ChainType>::allocator_type;
    auto chainList = m_physicalDevice.getQueueFamilyProperties2<ChainType, AllocatorType>(VULKAN_HPP_DEFAULT_DISPATCHER);
    return chainList;
  }

  [[nodiscard]] auto getFirstDeviceLocalHeapSize() const;

  vk::PhysicalDevice m_physicalDevice;
  std::vector<vk::ExtensionProperties> m_extensions;
  QueueFamilies m_queueFamilies;
};

inline auto PhysicalDevice::getFirstDeviceLocalHeapSize() const
{
  auto memoryHeaps = getMemoryHeaps();

  auto findDeviceLocalPred = [](const vk::MemoryHeap& memoryHeap) {
    return VkHal::flagsHasBits(memoryHeap.flags, vk::MemoryHeapFlagBits::eDeviceLocal);
  };

  const auto memoryHeapIt = std::ranges::find_if(memoryHeaps, findDeviceLocalPred);
  const auto deviceLocalHeapSize = memoryHeapIt != memoryHeaps.cend() ? memoryHeapIt->size : 0;

  return deviceLocalHeapSize;
}

void PhysicalDevice::printDeviceInfo() const
{
  const auto& props = getProperties();
  const auto& limits = getLimits();

  fmt::println("Device Name: {}", props.deviceName);
  fmt::println("Device Type: {}", props.deviceType);
  fmt::println("Supported Vulkan version: {}", Version(props.apiVersion));
  fmt::println("");

  fmt::println("Max Push Constant Size: {} B", limits.maxPushConstantsSize);
  fmt::println("Max Sampler Anisotropy: {:.1f}", limits.maxSamplerAnisotropy);
  fmt::println("Line Width: [{:.1f}, {:.1f}]", limits.lineWidthRange[0], limits.lineWidthRange[1]);
  fmt::println("");

  fmt::println("Device Extensions ({}):", m_extensions.size());
  std::ranges::for_each(m_extensions, [](const vk::ExtensionProperties& props) {
    fmt::println("- {} v{}", props.extensionName, props.specVersion);
  });
  fmt::println("");

  const auto& memoryHeaps = getMemoryHeaps();
  fmt::println("Memory Heaps ({}):", memoryHeaps.size());
  std::ranges::for_each(memoryHeaps | std::ranges::views::enumerate, [](const std::tuple<ptrdiff_t, vk::MemoryHeap>& tuple) {
    const auto& [i, memoryHeap] = tuple;
    fmt::println("{:>2}: {} {}", i, Size(memoryHeap.size), memoryHeap.flags);
  });
  fmt::println("");

  const auto& memoryTypes = getMemoryTypes();
  fmt::println("Memory Types ({}):", memoryTypes.size());
  std::ranges::for_each(memoryTypes | std::ranges::views::enumerate, [](const std::tuple<ptrdiff_t, vk::MemoryType>& tuple) {
    const auto& [i, memoryType] = tuple;
    fmt::println("{:>2}: Heap index {} {}", i, memoryType.heapIndex, memoryType.propertyFlags);
  });
  fmt::println("");

  m_queueFamilies.printQueFamiliesInfo();
}

auto operator==(const PhysicalDevice& lhs, const PhysicalDevice& rhs)
{
  return lhs.m_physicalDevice == rhs.m_physicalDevice;
}

auto operator<=>(const PhysicalDevice& lhs, const PhysicalDevice& rhs)
{
  if (auto cmp = lhs.m_physicalDevice <=> rhs.m_physicalDevice; cmp == 0)
  {
    return cmp;
  }

  constexpr size_t bitsInUint32 = sizeof(uint32_t) * CHAR_BIT;
  constexpr size_t mostSignificantBit = bitsInUint32 - 1;
  auto lhsBitsetScore = std::bitset<bitsInUint32>{};
  auto rhsBitsetScore = std::bitset<bitsInUint32>{};

  lhsBitsetScore.set(mostSignificantBit, lhs.getProperties().deviceType == vk::PhysicalDeviceType::eDiscreteGpu);
  rhsBitsetScore.set(mostSignificantBit, rhs.getProperties().deviceType == vk::PhysicalDeviceType::eDiscreteGpu);

  lhsBitsetScore.set(mostSignificantBit - 1, lhs.getFirstDeviceLocalHeapSize() > rhs.getFirstDeviceLocalHeapSize());
  rhsBitsetScore.set(mostSignificantBit - 1, lhs.getFirstDeviceLocalHeapSize() < rhs.getFirstDeviceLocalHeapSize());

  lhsBitsetScore.set(mostSignificantBit - 2, lhs.m_queueFamilies.getQueueFamilies(vk::QueueFlagBits::eTransfer).size() > 0);
  rhsBitsetScore.set(mostSignificantBit - 2, rhs.m_queueFamilies.getQueueFamilies(vk::QueueFlagBits::eTransfer).size() > 0);

  lhsBitsetScore.set(mostSignificantBit - 3, lhs.m_queueFamilies.getQueueFamilies(vk::QueueFlagBits::eCompute).size() > 0);
  rhsBitsetScore.set(mostSignificantBit - 3, rhs.m_queueFamilies.getQueueFamilies(vk::QueueFlagBits::eCompute).size() > 0);

  lhsBitsetScore.set(mostSignificantBit - 4, lhs.getLimits().maxImageDimension2D > rhs.getLimits().maxImageDimension2D);
  rhsBitsetScore.set(mostSignificantBit - 4, lhs.getLimits().maxImageDimension2D < rhs.getLimits().maxImageDimension2D);

  auto lhsScore = lhsBitsetScore.to_ulong();
  auto rhsScore = rhsBitsetScore.to_ulong();

  if (auto cmp = lhsScore <=> rhsScore; cmp != 0)
  {
    return cmp;
  }

  return lhs.m_physicalDevice <=> rhs.m_physicalDevice;
}

} // namespace VkHal
