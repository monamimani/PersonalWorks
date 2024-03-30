#pragma once

#include <optional>
#include <string>
#include <vector>

#include "Core/Win32/WindowsHeader.h"

namespace ApplicationCore::Win32
{

struct Win32ApplicationDesc
{
  HINSTANCE m_appInstance = nullptr;
  std::vector<std::string> m_cmdLine;
  std::optional<int32_t> m_cmdShow;
  bool m_createConsole = false;
  std::string m_applicationName;
};

} // namespace ApplicationCore::Win32