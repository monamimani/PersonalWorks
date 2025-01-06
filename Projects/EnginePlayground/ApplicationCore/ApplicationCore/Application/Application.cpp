#include "ApplicationCore/Application/Application.h" // IWYU pragma: associated

#include "Core/Win32/StringUtils.h"

namespace ApplicationCore
{
Application::Application(const std::variant<std::string_view, std::wstring_view>& applicationName)
{
  if (const auto* appNameStr = std::get_if<std::string_view>(&applicationName))
  {
    m_applicationName = *appNameStr;
  }
  else if (const auto* appNameWStr = std::get_if<std::wstring_view>(&applicationName))
  {
    m_applicationName = Core::Win32::StringUtils::wstringToString(*appNameWStr);
  }
  else
  {
    throw std::bad_variant_access();
  }
}

} // namespace ApplicationCore
