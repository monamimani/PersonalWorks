#pragma once

#include <tuple>

#include "Core/Win32/WindowsHeader.h"

namespace ApplicationCore::Win32
{

class Win32MessageLoop
{
public:
  std::tuple < bool, int32_t> pump()
  {
    MSG systemMsg{};
    bool isContinue = true;

    while (::PeekMessage(&systemMsg, nullptr, 0, 0, PM_REMOVE))
    {
      // Handle messages
      ::TranslateMessage(&systemMsg);
      ::DispatchMessage(&systemMsg);
      systemMsg = MSG{};
    }

    if (systemMsg.message == WM_QUIT)
    {
      isContinue = false;
      return { isContinue, static_cast<int32_t>(systemMsg.wParam)};
    }

    return { isContinue, 0};
  }

};

} // namespace ApplicationCore