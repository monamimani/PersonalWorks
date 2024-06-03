#pragma once

#include <expected>
#include <functional>
#include <span>
#include <string_view>
#include <variant>
#include <vector>

#include "CoroErrorManagement/CoroResult.h"

namespace CoroErrorMgmt
{
enum class ErrorKind
{
  NotSorted,
  NotSupported
};

struct ExtensionsNotSorted
{
  std::span<std::string_view> m_extensions;
};

struct ExtensionsNotSupported
{
  std::vector<std::string_view> m_notSupportedExtensions;
  std::vector<std::string_view> m_requestedExtensions;
};

[[nodiscard]] std::expected<bool, std::variant<ExtensionsNotSorted, ExtensionsNotSupported>> validateExtensions(
    std::span<std::string_view> supportedExtensions, std::span<std::string_view> requestedExtensions);

CoroResultErrorMgmt<bool, ExtensionsNotSorted*, ExtensionsNotSupported*> validateExtensionsCoro(std::span<std::string_view> supportedExtensions,
                                                                                                            std::span<std::string_view> requestedExtensions);

} // namespace CoroErrorMgmt
