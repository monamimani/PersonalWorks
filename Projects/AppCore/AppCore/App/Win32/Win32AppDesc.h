#pragma once

#include <optional>
#include <variant>
#include <string_view>

#include "Core/Win32/WindowsHeader.h"

namespace AppCore::App::Win32
{

struct Win32AppDesc
{
  HINSTANCE m_appInstance = nullptr;
  std::optional<LPSTR> m_cmdLine;
  std::optional<int32_t> m_cmdShow;
  std::variant<std::string_view, std::wstring_view> m_applicationName;
};

} // namespace AppCore::Window