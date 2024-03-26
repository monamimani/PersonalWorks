
#include <ranges>

#include "Gfxal/Vkal/Vk.h"
#include "TestUtilities/GoogleBenchmark.h"

namespace GfxBenchmark
{

vk::UniqueInstance g_instance;
vk::PhysicalDevice g_physicalDevice;

static void VkSetup([[maybe_unused]] const benchmark::State& state)
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

  g_instance = vk::createInstanceUnique(instanceInfo);

  if constexpr (GfxVk::vkDispathDynamic)
  {
    VULKAN_HPP_DEFAULT_DISPATCHER.init(g_instance.get());
  }

  auto physicalDevices = g_instance->enumeratePhysicalDevices();

  std::ranges::partition(physicalDevices, [](const vk::PhysicalDevice& physicalDevice) {
    auto propertiesChain = physicalDevice.getProperties2();
    return propertiesChain.properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu;
  });

  g_physicalDevice = physicalDevices[0];
}

static void VkTeardown([[maybe_unused]] const benchmark::State& state)
{}

static void vkPhysicalDeviceEnumerateDeviceExtensionProperties(benchmark::State& state)
{
  for (auto _ : state)
  {
    auto extensions = g_physicalDevice.enumerateDeviceExtensionProperties();
    benchmark::ClobberMemory();
    benchmark::DoNotOptimize(extensions);
  }
}

BENCHMARK(vkPhysicalDeviceEnumerateDeviceExtensionProperties)->Setup(VkSetup)->Teardown(VkTeardown);

static void vkPhysicalDeviceGetProperties2(benchmark::State& state)
{
  for (auto _ : state)
  {
    auto properties = g_physicalDevice.getProperties2();
    benchmark::ClobberMemory();
    benchmark::DoNotOptimize(properties);
  }
}

BENCHMARK(vkPhysicalDeviceGetProperties2)->Setup(VkSetup)->Teardown(VkTeardown);

static void vkPhysicalDeviceGetProperties2Chain(benchmark::State& state)
{
  for (auto _ : state)
  {
    vk::StructureChain<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceVulkan12Properties, vk::PhysicalDeviceVulkan13Properties> propertiesChain;
    g_physicalDevice.getProperties2(&propertiesChain.get());
    benchmark::ClobberMemory();
    benchmark::DoNotOptimize(propertiesChain);
  }
}

BENCHMARK(vkPhysicalDeviceGetProperties2Chain)->Setup(VkSetup)->Teardown(VkTeardown);

static void vkPhysicalDeviceGetFeatures2(benchmark::State& state)
{
  for (auto _ : state)
  {
    auto features = g_physicalDevice.getFeatures2();
    benchmark::ClobberMemory();
    benchmark::DoNotOptimize(features);
  }
}

BENCHMARK(vkPhysicalDeviceGetFeatures2)->Setup(VkSetup)->Teardown(VkTeardown);

static void vkPhysicalDeviceGetFeatures2Chain(benchmark::State& state)
{
  for (auto _ : state)
  {
    auto featuresChain = g_physicalDevice.getFeatures2<vk::PhysicalDeviceFeatures2, vk::PhysicalDeviceVulkan12Features, vk::PhysicalDeviceVulkan13Features>();
    benchmark::ClobberMemory();
    benchmark::DoNotOptimize(featuresChain);
  }
}

BENCHMARK(vkPhysicalDeviceGetFeatures2Chain)->Setup(VkSetup)->Teardown(VkTeardown);

static void vkPhysicalDeviceGetMemoryProperties2(benchmark::State& state)
{
  for (auto _ : state)
  {
    auto memoryProperties = g_physicalDevice.getMemoryProperties2();
    benchmark::ClobberMemory();
    benchmark::DoNotOptimize(memoryProperties);
  }
}

BENCHMARK(vkPhysicalDeviceGetMemoryProperties2)->Setup(VkSetup)->Teardown(VkTeardown);

static void vkPhysicalDeviceGetQueueFamilyProperties(benchmark::State& state)
{
  for (auto _ : state)
  {
    auto queueFamilyProperties = g_physicalDevice.getQueueFamilyProperties2();
    benchmark::ClobberMemory();
    benchmark::DoNotOptimize(queueFamilyProperties);
  }
}

BENCHMARK(vkPhysicalDeviceGetQueueFamilyProperties)->Setup(VkSetup)->Teardown(VkTeardown);

} // namespace GfxBenchmark
