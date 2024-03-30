#include "Core/Config.h"
#include "TestUtilities/GoogleTest.h"

import Vkal.VulkanSystem;
import Vkal.Utilities;

namespace VkHalTest
{

class VulkanSystemTestsF: public testing::Test
{};

TEST(VulkanSystemTests, CreateVulkanSystem)
{
  auto systemDesc = VkHal::VulkanSystemDesc{.m_applicationName = "VulkanSystemTests",
                                            .m_engineName = "VulkanSystemTestsEngine",
                                            .m_applicationVersion = Core::makeVersion(0, 0, 1),
                                            .m_engineVersion = Core::makeVersion(0, 0, 1),
                                            .m_enableValidation = true,
                                            .m_enableDebugUtils = true,
                                            .m_isVerbose = false};

  VkHal::VulkanSystem vkSystem;

  ASSERT_NO_THROW({ vkSystem = VkHal::VulkanSystem(systemDesc); });
}

} // namespace VkHalTest
