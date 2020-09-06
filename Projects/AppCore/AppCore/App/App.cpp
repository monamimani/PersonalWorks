#include "AppCore/App/App.h"


#include "Core/Win32/StringUtils.h"

namespace AppCore::App
{
App::App(const std::variant<std::string_view, std::wstring_view>& applicationName)
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

} // namespace AppCore::App