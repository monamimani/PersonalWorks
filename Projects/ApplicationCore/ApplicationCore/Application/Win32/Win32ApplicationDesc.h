#pragma once

#include <optional>
#include <string_view>
#include <variant>

#include "Core/Win32/WindowsHeader.h"

namespace ApplicationCore::Win32
{

struct Win32ApplicationDesc
{
  HINSTANCE m_appInstance = nullptr;
  std::optional<LPSTR> m_cmdLine;
  std::optional<int32_t> m_cmdShow;
  bool m_createConsole = false;
  std::variant<std::string_view, std::wstring_view> m_applicationName;
};

} // namespace ApplicationCore::Win32