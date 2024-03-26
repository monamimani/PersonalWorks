module;

#include <format>
#include <print>
#include <ranges>
#include <string>

#include "fmt/format.h"
#include "Gfxal/Vkal/Vk.h"

export module Vkal.DebugMsg;

namespace VkHal
{
VKAPI_ATTR VkBool32 VKAPI_CALL debugMsgCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                void*)
{
  namespace ranges = std::ranges;

  auto msgServerity = vk::to_string(static_cast<vk::DebugUtilsMessageSeverityFlagBitsEXT>(messageSeverity));
  auto msgType = vk::to_string(static_cast<vk::DebugUtilsMessageTypeFlagsEXT>(messageType));
  auto callbackData = reinterpret_cast<const vk::DebugUtilsMessengerCallbackDataEXT*>(pCallbackData);

  fmt::println("VULKAN_DEBUG_MSG[{}: {}] MessageId: {:#x}, Name: {}",
               msgServerity,
               msgType,
               static_cast<uint32_t>(callbackData->messageIdNumber),
               callbackData->pMessageIdName);
  fmt::println("{}", callbackData->pMessage);

  constexpr auto labelPrinter = [](const vk::DebugUtilsLabelEXT& debugUtilsLabel) {
    fmt::println("        labelName = <{}>", debugUtilsLabel.pLabelName);
  };

  if (callbackData->queueLabelCount > 0)
  {
    fmt::println("    Queue Labels:");
    auto queueLabelSpan = std::span{callbackData->pQueueLabels, callbackData->queueLabelCount};
    ranges::for_each(queueLabelSpan, labelPrinter);
  }

  if (callbackData->cmdBufLabelCount > 0)
  {
    fmt::println("    CommandBuffer Labels:");
    auto cmdBufLabelSpan = std::span{callbackData->pCmdBufLabels, callbackData->cmdBufLabelCount};
    ranges::for_each(cmdBufLabelSpan, labelPrinter);
  }

  auto objectSpan = std::span{callbackData->pObjects, callbackData->objectCount};
  ranges::for_each(ranges::views::enumerate(objectSpan), [](auto&& tuple) {
    const auto& [i, objectNameInfo] = tuple;
    fmt::println("    Object {}", i);
    fmt::println("        objectType   = {}", vk::to_string(static_cast<vk::ObjectType>(objectNameInfo.objectType)));
    fmt::println("        objectHandle = {:#x}", objectNameInfo.objectHandle);

    auto objName = objectNameInfo.pObjectName ? objectNameInfo.pObjectName : "Unknown";
    fmt::println("        objectName   = {}", objName);
  });

  return VK_FALSE;
}
} // namespace VkHal

namespace GfxTest
{
class BasicVulkanTestsF_VkCreateInstance_Test;
}

export namespace VkHal
{
class DebugMessenger
{
  friend class GfxTest::BasicVulkanTestsF_VkCreateInstance_Test;

public:
  static auto makeDebugMessengerCreateInfo(bool isVerbose)
  {
    constinit static auto severityFlags = vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError;
    constinit static auto messageTypeFlags =
        vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation;

    auto debugUtilsMessengerCreateInfo =
        vk::DebugUtilsMessengerCreateInfoEXT{.messageSeverity = severityFlags, .messageType = messageTypeFlags, .pfnUserCallback = &VkHal::debugMsgCallback};

    if (isVerbose)
    {
      debugUtilsMessengerCreateInfo.messageSeverity |= vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose;
    }

    return debugUtilsMessengerCreateInfo;
  }

  DebugMessenger(const vk::Instance& vkInstance, bool isVerbose)
  {
    m_debugUtilsMessenger = vkInstance.createDebugUtilsMessengerEXTUnique(makeDebugMessengerCreateInfo(isVerbose));
  }

private:
  vk::UniqueDebugUtilsMessengerEXT m_debugUtilsMessenger;
};
} // namespace VkHal

module :private;
