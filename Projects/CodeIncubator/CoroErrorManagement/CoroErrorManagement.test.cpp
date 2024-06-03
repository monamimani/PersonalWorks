#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <random>

#include "Core/Overloaded.h"
#include "CoroErrorManagement/CoroErrorManagementUtilities.h"

using namespace std::literals;
using namespace CoroErrorMgmt;

TEST_CASE("StdExpected", "[CoroErrorMgmt]")
{
  auto extensions = std::vector{"VK_KHR_16bit_storage"sv,
                                "VK_KHR_8bit_storage"sv,
                                "VK_KHR_android_surface"sv,
                                "VK_KHR_bind_memory2"sv,
                                "VK_KHR_create_renderpass2"sv,
                                "VK_KHR_dedicated_allocation"sv,
                                "VK_KHR_depth_stencil_resolve"sv,
                                "VK_KHR_descriptor_update_template"sv,
                                "VK_KHR_device_group"sv,
                                "VK_KHR_device_group_creation"sv};
  auto requestedExtensions = std::vector{"VK_KHR_16bit_storage"sv,
                                         "VK_KHR_8bit_storage"sv,
                                         "VK_KHR_android_surface"sv,
                                         "VK_KHR_create_renderpass2"sv,
                                         "VK_KHR_descriptor_update_template"sv,
                                         "VK_KHR_device_group"sv,
                                         "VK_KHR_device_group_creation"sv};

  auto notSupportredExtensions = std::vector{"VK_KHR_display"sv, "VK_KHR_display_swapchain"sv, "VK_KHR_draw_indirect_count"sv};

  SECTION("Success Path")
  {
    REQUIRE(validateExtensions(extensions, requestedExtensions) == true);
  }
  SECTION("Not Sorted")
  {
    std::random_device rd;
    std::mt19937 gen{rd()};
    auto shuffledExtensions = extensions;
    std::ranges::shuffle(shuffledExtensions, gen);
    auto shuffledRequestedExtensions = requestedExtensions;
    std::ranges::shuffle(shuffledRequestedExtensions, gen);

    REQUIRE(std::holds_alternative<ExtensionsNotSorted>(validateExtensions(shuffledExtensions, requestedExtensions).error()));
    REQUIRE(std::holds_alternative<ExtensionsNotSorted>(validateExtensions(extensions, shuffledRequestedExtensions).error()));
    REQUIRE(std::holds_alternative<ExtensionsNotSorted>(validateExtensions(shuffledExtensions, shuffledRequestedExtensions).error()));

    SECTION("Fix Extensions Not Sorted")
    {
      auto notSortedVisitor = Overloaded{[](const ExtensionsNotSorted& error) {
                                           std::ranges::sort(error.m_extensions);
                                         },
                                         [](const auto& error) {
                                           throw error;
                                         }};

      auto resultShuffledExtensions = validateExtensions(shuffledExtensions, requestedExtensions);
      while (!resultShuffledExtensions.has_value())
      {
        std::visit(notSortedVisitor, resultShuffledExtensions.error());
        resultShuffledExtensions = validateExtensions(shuffledExtensions, requestedExtensions);
      }
      REQUIRE(resultShuffledExtensions.value());

      auto resultShuffledRequestedExtensions = validateExtensions(shuffledExtensions, shuffledRequestedExtensions);
      while (!resultShuffledRequestedExtensions.has_value())
      {
        std::visit(notSortedVisitor, resultShuffledRequestedExtensions.error());
        resultShuffledRequestedExtensions = validateExtensions(shuffledExtensions, shuffledRequestedExtensions);
      }
      REQUIRE(resultShuffledRequestedExtensions.value());

      std::ranges::shuffle(shuffledExtensions, gen);
      std::ranges::shuffle(shuffledRequestedExtensions, gen);
      auto resultShuffled = validateExtensions(shuffledExtensions, shuffledRequestedExtensions);
      while (!resultShuffled.has_value())
      {
        std::visit(notSortedVisitor, resultShuffled.error());
        resultShuffled = validateExtensions(shuffledExtensions, shuffledRequestedExtensions);
      }
      REQUIRE(resultShuffled.value());
    }
  }

  SECTION("Not Supported")
  {
    requestedExtensions.append_range(notSupportredExtensions);

    auto notSupportedVisitor =
        Overloaded{[](const ExtensionsNotSorted& error) {
                     std::ranges::sort(error.m_extensions);
                   },
                   [&requestedExtensions](ExtensionsNotSupported& error) {
                     std::vector<std::string_view> supportedExtensions;
                     std::ranges::set_difference(requestedExtensions, error.m_notSupportedExtensions, std::back_inserter(supportedExtensions));
                     requestedExtensions = supportedExtensions;
                   },
                   [](const auto& error) {
                     throw error;
                   }};

    REQUIRE(std::holds_alternative<ExtensionsNotSupported>(validateExtensions(extensions, requestedExtensions).error()));

    SECTION("Fix Extension Not Supported")
    {
      auto resultNotSupported = validateExtensions(extensions, requestedExtensions);
      while (!resultNotSupported.has_value())
      {
        std::visit(notSupportedVisitor, resultNotSupported.error());
        resultNotSupported = validateExtensions(extensions, requestedExtensions);
      }

      REQUIRE(resultNotSupported.value());
    }
  }
}

/*
TEST_CASE("StdExpected", "[CoroErrorMgmtBenchmark]")
{
  auto extensions = std::vector{"VK_KHR_16bit_storage"sv,
                                "VK_KHR_8bit_storage"sv,
                                "VK_KHR_android_surface"sv,
                                "VK_KHR_bind_memory2"sv,
                                "VK_KHR_create_renderpass2"sv,
                                "VK_KHR_dedicated_allocation"sv,
                                "VK_KHR_depth_stencil_resolve"sv,
                                "VK_KHR_descriptor_update_template"sv,
                                "VK_KHR_device_group"sv,
                                "VK_KHR_device_group_creation"sv};
  auto requestedExtensions = std::vector{"VK_KHR_16bit_storage"sv,
                                         "VK_KHR_8bit_storage"sv,
                                         "VK_KHR_android_surface"sv,
                                         "VK_KHR_create_renderpass2"sv,
                                         "VK_KHR_descriptor_update_template"sv,
                                         "VK_KHR_device_group"sv,
                                         "VK_KHR_device_group_creation"sv};

  std::random_device rd;
  std::mt19937 gen{rd()};
  auto shuffledExtensions = extensions;
  std::ranges::shuffle(shuffledExtensions, gen);

  BENCHMARK("Fix Extensions Not Sorted")
  {
    auto notSortedVisitor = Overloaded{[](const ExtensionsNotSorted& error) {
                                         std::ranges::sort(error.m_extensions);
                                       },
                                       [](const auto& error) {
                                         throw error;
                                       }};

    auto resultShuffledExtensions = validateExtensions(shuffledExtensions, requestedExtensions);
    while (!resultShuffledExtensions.has_value())
    {
      std::visit(notSortedVisitor, resultShuffledExtensions.error());
      resultShuffledExtensions = validateExtensions(shuffledExtensions, requestedExtensions);
    }
    return resultShuffledExtensions;
  };
}
*/

TEST_CASE("StdExpectedCoro", "[CoroErrorMgmt]")
{
  auto extensions = std::vector{"VK_KHR_16bit_storage"sv,
                                "VK_KHR_8bit_storage"sv,
                                "VK_KHR_android_surface"sv,
                                "VK_KHR_bind_memory2"sv,
                                "VK_KHR_create_renderpass2"sv,
                                "VK_KHR_dedicated_allocation"sv,
                                "VK_KHR_depth_stencil_resolve"sv,
                                "VK_KHR_descriptor_update_template"sv,
                                "VK_KHR_device_group"sv,
                                "VK_KHR_device_group_creation"sv};
  auto requestedExtensions = std::vector{"VK_KHR_16bit_storage"sv,
                                         "VK_KHR_8bit_storage"sv,
                                         "VK_KHR_android_surface"sv,
                                         "VK_KHR_create_renderpass2"sv,
                                         "VK_KHR_descriptor_update_template"sv,
                                         "VK_KHR_device_group"sv,
                                         "VK_KHR_device_group_creation"sv};

  auto notSupportredExtensions = std::vector{"VK_KHR_display"sv, "VK_KHR_display_swapchain"sv, "VK_KHR_draw_indirect_count"sv};

  SECTION("Success Path")
  {
    REQUIRE(validateExtensionsCoro(extensions, requestedExtensions).hasValue() == true);
  }

  SECTION("Not Sorted")
  {
    std::random_device rd;
    std::mt19937 gen{rd()};
    auto shuffledExtensions = extensions;
    std::ranges::shuffle(shuffledExtensions, gen);
    auto shuffledRequestedExtensions = requestedExtensions;
    std::ranges::shuffle(shuffledRequestedExtensions, gen);

    REQUIRE(std::holds_alternative<ExtensionsNotSorted*>(validateExtensionsCoro(shuffledExtensions, requestedExtensions).getError()));
    REQUIRE(std::holds_alternative<ExtensionsNotSorted*>(validateExtensionsCoro(extensions, shuffledRequestedExtensions).getError()));
    REQUIRE(std::holds_alternative<ExtensionsNotSorted*>(validateExtensionsCoro(shuffledExtensions, shuffledRequestedExtensions).getError()));
  }

  SECTION("Fix Extensions Not Sorted explicit")
  {
    auto notSortedVisitor = Overloaded{[](ExtensionsNotSorted* error) {
                                         std::ranges::sort(error->m_extensions);
                                       },
                                       [](const auto& error) {
                                         throw error;
                                       }};

    std::random_device rd;
    std::mt19937 gen{rd()};

    {
      auto shuffledExtensions = extensions;
      std::ranges::shuffle(shuffledExtensions, gen);
      std::ranges::sort(requestedExtensions);
      auto result = validateExtensionsCoro(shuffledExtensions, requestedExtensions);
      REQUIRE(result.hasValue() == false);
      while (!result.hasValue())
      {
        result.visit(notSortedVisitor);
      }
      REQUIRE(result.hasValue());
      REQUIRE(result.getResult() == true);
    }

    {
      std::ranges::sort(extensions);
      auto shuffledRequestedExtensions = requestedExtensions;
      std::ranges::shuffle(shuffledRequestedExtensions, gen);

      auto result = validateExtensionsCoro(extensions, shuffledRequestedExtensions);
      REQUIRE(result.hasValue() == false);
      while (!result.hasValue())
      {
        result.visit(notSortedVisitor);
      }
      REQUIRE(result.hasValue());
      REQUIRE(result.getResult() == true);
    }

    {
      auto shuffledExtensions = extensions;
      std::ranges::shuffle(shuffledExtensions, gen);
      auto shuffledRequestedExtensions = requestedExtensions;
      std::ranges::shuffle(shuffledRequestedExtensions, gen);

      auto result = validateExtensionsCoro(shuffledExtensions, shuffledRequestedExtensions);
      REQUIRE(result.hasValue() == false);
      while (!result.hasValue())
      {
        result.visit(notSortedVisitor);
      }
      REQUIRE(result.hasValue());
      REQUIRE(result.getResult() == true);
    }
  }

  SECTION("Fix Extensions Not Sorted implicit")
  {
    auto notSortedVisitor = Overloaded{[](ExtensionsNotSorted* error) {
                                         std::ranges::sort(error->m_extensions);
                                       },
                                       [](const auto& error) {
                                         throw error;
                                       }};

    std::random_device rd;
    std::mt19937 gen{rd()};

    {
      auto shuffledExtensions = extensions;
      std::ranges::shuffle(shuffledExtensions, gen);
      std::ranges::sort(requestedExtensions);
      auto result = validateExtensionsCoro(shuffledExtensions, requestedExtensions);
      REQUIRE(result.hasValue() == false);
      REQUIRE(result.getResult(notSortedVisitor) == true);
      REQUIRE(result.hasValue());
    }

    {
      std::ranges::sort(extensions);
      auto shuffledRequestedExtensions = requestedExtensions;
      std::ranges::shuffle(shuffledRequestedExtensions, gen);

      auto result = validateExtensionsCoro(extensions, shuffledRequestedExtensions);
      REQUIRE(result.hasValue() == false);
      REQUIRE(result.getResult(notSortedVisitor) == true);
      REQUIRE(result.hasValue());
    }

    {
      auto shuffledExtensions = extensions;
      std::ranges::shuffle(shuffledExtensions, gen);
      auto shuffledRequestedExtensions = requestedExtensions;
      std::ranges::shuffle(shuffledRequestedExtensions, gen);

      auto result = validateExtensionsCoro(shuffledExtensions, shuffledRequestedExtensions);
      REQUIRE(result.hasValue() == false);
      REQUIRE(result.getResult(notSortedVisitor) == true);
      REQUIRE(result.hasValue());
    }
  }

  SECTION("Not Supported")
  {
    requestedExtensions.append_range(notSupportredExtensions);

    auto notSupportedVisitor =
        Overloaded{[](ExtensionsNotSorted* error) {
                     std::ranges::sort(error->m_extensions);
                   },
                   [](ExtensionsNotSupported* error) {
                     std::vector<std::string_view> supportedExtensions;
                     std::ranges::set_difference(error->m_requestedExtensions, error->m_notSupportedExtensions, std::back_inserter(supportedExtensions));
                     error->m_requestedExtensions  = supportedExtensions;
                   },
                   [](const auto& error) {
                     throw error;
                   }};

    REQUIRE(std::holds_alternative<ExtensionsNotSupported*>(validateExtensionsCoro(extensions, requestedExtensions).getError()));

    SECTION("Fix Extension Not Supported Implicit")
    {
      auto result = validateExtensionsCoro(extensions, requestedExtensions);
      REQUIRE(result.hasValue() == false);
      REQUIRE(result.getResult(notSupportedVisitor) == true);
      REQUIRE(result.hasValue());
    }
  }
}
