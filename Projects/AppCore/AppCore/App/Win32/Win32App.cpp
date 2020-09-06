#include "AppCore/App/Win32/Win32App.h"

#include <iostream>
#include <stdexcept>

#include "AppCore/Window/Win32/Win32Window.h"
#include "AppCore/Window/Window.h"

namespace AppCore::App::Win32
{

Win32App::~Win32App() = default;

Win32App::Win32App(const Win32AppDesc& appDesc, bool createConsole)
    : App(appDesc.m_applicationName)
    , m_appInstance{appDesc.m_appInstance}
{

  if (createConsole)
  {
    // AllocConsole();
    // AttachConsole(GetCurrentProcessId());
    // FILE* stream;
    // freopen_s(&stream, "CON", "w", stdout);
    // assert(stream == stdout);
    // freopen_s(&stream, "CON", "w", stderr);
    // assert(stream == stderr);

    // SetConsoleTitle(TEXT(m_applicationName.c_str()));
  }

  //m_timer.SetFixedTimeStep(true);
  //m_timer.SetTargetDeltaTimeInSeconds(std::chrono::seconds(1) / 60.0);
}

int Win32App::run()
{
  MSG systemMsg{};
  try
  {
    HACCEL hAccelTable = {};

    while (isRunning())
    {
      while (::PeekMessage(&systemMsg, nullptr, 0, 0, PM_REMOVE))
      {

        if (!TranslateAccelerator(systemMsg.hwnd, hAccelTable, &systemMsg))
        {
          // Handle messages
          ::TranslateMessage(&systemMsg);
          ::DispatchMessage(&systemMsg);
        }
      }

      if (systemMsg.message == WM_QUIT)
      {
        break;
      }

      // Application code
      //m_timer.Tick([this]() {
      // std::cout << "Timer stats" << std::endl;
      // std::cout << "GetDeltaTimeInSeconds " << m_timer.GetDeltaTimeInSeconds() << std::endl;
      // std::cout << "GetFrameCount " << m_timer.GetFrameCount() << std::endl;
      // std::cout << "GetFramesPerSecond " << m_timer.GetFramesPerSecond() << std::endl;
      // std::cout << "GetTotalSeconds " << m_timer.GetTotalSeconds() << std::endl;
      // std::cout << std::endl;

      //  update(m_timer);
      //});
      //render(m_timer);
    }
  }
  catch (const std::runtime_error& e)
  {
    std::cerr << e.what() << std::endl;
    OutputDebugStringA(e.what());
    return EXIT_FAILURE;
  }

  return (int)systemMsg.wParam;
}
} // namespace AppCore::App::Win32