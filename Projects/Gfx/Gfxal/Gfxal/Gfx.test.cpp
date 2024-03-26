#include "TestUtilities/GoogleTest.h"

import VkHal;
import Vkal.PhysicalDevice;
import Core;

#include <concepts>
#include <cstddef>
#include <ranges>
#include <span>
#include <string>
#include <vector>

#include "Core/Config.h"


// struct WindowDesc
//{
//
//   HANDLE handle;
//   std::string name;
//   vk::Extent2D extent;
// };

namespace GfxTest
{

class VulkanTestsF: public testing::Test
{
protected:
  void SetUp() override
  {
    createInstance();
    createPhysicalDevice();
  }

  void TearDown() override
  {}

private:
  void createInstance()
  {
    auto systemDesc = VkHal::VkSystemDesc{.m_applicationName = "VkCreateInstanceHpp",
                                          .m_applicationVersion = Core::makeVersion(0, 0, 1),
                                          .m_engineName = "VkCreateInstaceHppEngine",
                                          .m_engineVersion = Core::makeVersion(0, 0, 1)};

    auto vkSystem = VkHal::VkSystem(systemDesc);
  }

  void createPhysicalDevice()
  {
    // vkPhysicalDevices = getPhysicalDevice();
    // if (!vkPhysicalDevices.empty())
    //{
    //   queueFamily = vkPhysicalDevices[0].getQueueFamily(vk::QueueFlagBits::eGraphics);
    // }
  }

  auto getPhysicalDevice()
  {
    // auto physicalDevices = vkInstance->enumeratePhysicalDevices();

    // std::vector<VkHal::PhysicalDevice> vkHalPhysicalDevices;
    // vkHalPhysicalDevices.reserve(physicalDevices.size());

    // std::ranges::transform(physicalDevices, std::back_inserter(vkHalPhysicalDevices), [](const vk::PhysicalDevice physicalDevice) {
    //   return VkHal::PhysicalDevice{physicalDevice};
    // });

    // std::ranges::sort(vkHalPhysicalDevices);

    // return vkHalPhysicalDevices;
  }

protected:
  // std::vector<VkHal::PhysicalDeviceQueueFamily> queueFamily;

private:
};

static_assert(std::totally_ordered<VkHal::PhysicalDevice>);

TEST(VulkanTests, VkCreateInstance)
{
  //auto systemDesc = VkHal::VkSystemDesc{.m_applicationName = "VkCreateInstanceHpp",
  //                                      .m_applicationVersion = Core::makeVersion(0, 0, 1),
  //                                      .m_engineName = "VkCreateInstaceHppEngine",
  //                                      .m_engineVersion = Core::makeVersion(0, 0, 1)};

  //VkHal::VkSystem vkSystem;

  //ASSERT_NO_THROW({ vkSystem = VkHal::VkSystem(systemDesc); });

  //EXPECT_EQ(vkSystem.m_supportedLayers.size(), vkSystem.m_requestedlayers.size());
  //EXPECT_EQ(vkSystem.m_unsupportedLayers.size(), 0);

  //EXPECT_EQ(vkSystem.m_supportedExtensions.size(), vkSystem.m_requestedextensions.size());
  //EXPECT_EQ(vkSystem.m_unsupportedExtensions.size(), 0);

  //ASSERT_TRUE(vkSystem.m_instance);

  //if constexpr (Core::BuildKind != Core::BuildKind::Shipping)
  //{
  //  ASSERT_TRUE(vkSystem.m_debugMessenger.has_value());
  //  ASSERT_TRUE(vkSystem.m_debugMessenger.value().m_debugUtilsMessenger);
  //}
}

TEST_F(VulkanTestsF, VkCreateDevice)
{
  // ASSERT_TRUE(vkInstance);
  // ASSERT_GT(vkPhysicalDevices.size(), 0);

  // for (const auto& physicalDevice : vkPhysicalDevices)
  //{
  //   ASSERT_GT(physicalDevice.getQueueFamily(vk::QueueFlagBits::eGraphics).size(), 0);
  //   ASSERT_GT(physicalDevice.getQueueFamily(vk::QueueFlagBits::eCompute).size(), 0);
  //   ASSERT_GT(physicalDevice.getQueueFamily(vk::QueueFlagBits::eTransfer).size(), 0);
  // }
}

} // namespace GfxTest
