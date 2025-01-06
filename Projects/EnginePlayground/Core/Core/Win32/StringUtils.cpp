#include "Core/Win32/StringUtils.h" // IWYU pragma: associated

#include "Core/Win32/WindowsHeader.h"

namespace Core::Win32::StringUtils
{
std::string wstringToString(const std::wstring_view& str)
{
  std::string result;
  const int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, str.data(), (int)str.size(), nullptr, 0, nullptr, nullptr);
  std::string strTo(sizeNeeded, 0);
  WideCharToMultiByte(CP_UTF8, 0, str.data(), (int)str.size(), strTo.data(), (int)strTo.size(), nullptr, nullptr);
  return result;
}
}
