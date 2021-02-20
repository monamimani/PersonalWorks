#pragma once

#include "WindowsHeader.h"

#include <string>
#include <system_error>

namespace Win32Util
{
std::string GetLastErrorAsString()
{
  auto message = std::string{};

  auto errorMessageID = ::GetLastError();
  if (errorMessageID != ERROR_SUCCESS)
  {
    message = std::system_category().message(errorMessageID);
  }

  return message;
}
} // namespace Win32Util
