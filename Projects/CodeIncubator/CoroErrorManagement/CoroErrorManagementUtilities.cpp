#include "CoroErrorManagement/CoroErrorManagementUtilities.h"

#include <algorithm>
#include <iterator>

#include <iostream>
#include <format>

template<>
struct std::formatter<std::span<std::string_view>>: std::formatter<std::string_view>
{
  auto format(const std::span<std::string_view>& span, std::format_context& ctx) const
  {
    for (const auto& sv: span)
    {
      std::format_to(ctx.out(), "{},", sv);
    }

    return std::format_to(ctx.out(), "{}", 0);
  }
};

namespace CoroErrorMgmt
{

std::expected<bool, std::variant<ExtensionsNotSorted, ExtensionsNotSupported>> validateExtensions(std::span<std::string_view> supportedExtensions,
                                                                                                  std::span<std::string_view> requestedExtensions)
{
  std::vector<std::string_view> notSupportedExtensions;

  if (not std::ranges::is_sorted(supportedExtensions))
  {
    return std::unexpected(ExtensionsNotSorted{supportedExtensions});
  }

  if (not std::ranges::is_sorted(requestedExtensions))
  {
    return std::unexpected(ExtensionsNotSorted{requestedExtensions});
  }

  std::ranges::set_difference(requestedExtensions, supportedExtensions, std::back_inserter(notSupportedExtensions));

  // std::ranges::for_each(requestedExtensions, [&supportedExtensions, &notSupportedExtensions](std::string_view requestedExtensionName) {
  //   if (not std::ranges::contains(supportedExtensions, requestedExtensionName))
  //   {
  //     notSupportedExtensions.push_back(requestedExtensionName);
  //   }
  // });

  if (notSupportedExtensions.size() > 0)
  {
    return std::unexpected(ExtensionsNotSupported{.m_notSupportedExtensions = notSupportedExtensions});
  }

  return true;
}

CoroResultErrorMgmt<bool, ExtensionsNotSorted*, ExtensionsNotSupported*> validateExtensionsCoro(std::span<std::string_view> supportedExtensions,
                                                                                              std::span<std::string_view> requestedExtensions)
{

  std::vector<std::string_view> notSupportedExtensions;

  while (not std::ranges::is_sorted(supportedExtensions))
  {
    auto error = ExtensionsNotSorted{supportedExtensions};
    co_yield &error;
  }

  while (not std::ranges::is_sorted(requestedExtensions))
  {
    auto error = ExtensionsNotSorted{requestedExtensions};
    co_yield &error;
  }

  std::ranges::set_difference(requestedExtensions, supportedExtensions, std::back_inserter(notSupportedExtensions));

  // std::ranges::for_each(requestedExtensions, [&supportedExtensions, &notSupportedExtensions](std::string_view requestedExtensionName) {
  //   if (not std::ranges::contains(supportedExtensions, requestedExtensionName))
  //   {
  //     notSupportedExtensions.push_back(requestedExtensionName);
  //   }
  // });

  while (notSupportedExtensions.size() > 0)
  {
    auto error = ExtensionsNotSupported{.m_notSupportedExtensions = notSupportedExtensions, .m_requestedExtensions = std::vector(std::from_range, requestedExtensions)}; 
    co_yield &error;
    requestedExtensions = error.m_requestedExtensions;
    notSupportedExtensions.clear();
    std::ranges::set_difference(requestedExtensions, supportedExtensions, std::back_inserter(notSupportedExtensions));
  }

  co_return true;
}

} // namespace CoroErrorMgmt
