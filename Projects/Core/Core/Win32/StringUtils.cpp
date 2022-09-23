#include "Core/Win32/StringUtils.h"

#include "Core/Win32/WindowsHeader.h"

namespace Core::Win32::StringUtils
{
std::string wstringToString(const std::wstring_view& str)
{
  std::string result;
  int size_needed = WideCharToMultiByte(CP_UTF8, 0, str.data(), (int)str.size(), nullptr, 0, nullptr, nullptr);
  std::string strTo(size_needed, 0);
  WideCharToMultiByte(CP_UTF8, 0, str.data(), (int)result.size(), &strTo[0], size_needed, nullptr, nullptr);
  return result;
}
}