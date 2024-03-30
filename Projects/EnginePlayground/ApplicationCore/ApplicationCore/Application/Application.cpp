#include "ApplicationCore/Application/Application.h"


#include "Core/Win32/StringUtils.h"

namespace ApplicationCore
{
Application::Application(const std::variant<std::string_view, std::wstring_view>& applicationName)
{
  if (auto appName = std::get_if<std::string_view>(&applicationName))
  {
    m_applicationName = *appName;
  }
  else if (auto appName = std::get_if<std::wstring_view>(&applicationName))
  {
    m_applicationName = Core::Win32::StringUtils::wstringToString(*appName);
  }
  else
  {
    throw std::bad_variant_access();
  }
}

} // namespace ApplicationCore
