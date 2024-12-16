#pragma once

#include <string_view>
#include <string>

namespace Core::Win32::StringUtils
{
std::string wstringToString(const std::wstring_view& str);
}
