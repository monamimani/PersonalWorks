#include "AppCore/App/Win32/Win32App.h"


#include "AppCore/Window/Window.h"

#include <iostream>
#include <stdexcept>

namespace AppCore::App::Win32
{

Win32App::~Win32App() = default;

Win32App::Win32App(const Win32AppDesc& appDesc, bool createConsole)
: App(appDesc.m_applicationName)
, m_appInstance{appDesc.m_appInstance}
, m_win32AppDesc{appDesc}
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
    while (isRunning())
    {
      while (::PeekMessage(&systemMsg, nullptr, 0, 0, PM_REMOVE))
      {
        // Handle messages
        ::TranslateMessage(&systemMsg);
        ::DispatchMessage(&systemMsg);
      }

      if (systemMsg.message == WM_QUIT)
      {
        setRunning(false);
        break;
      }

      AppContext context;
      context.m_appFrameTimeContext = m_timer.getFrameTimeContext();

      // Application code
      m_timer.tick([this, context]() {
        std::cout << "Timer stats" << std::endl;
        std::cout << "GetDeltaTimeInSeconds " << m_timer.getDeltaTimeInSeconds() << std::endl;
        std::cout << "GetFrameCount " << m_timer.getFrameCount() << std::endl;
        std::cout << "GetFramesPerSecond " << m_timer.getFramesPerSecond() << std::endl;
        std::cout << "GetTotalSeconds " << m_timer.getTotalSeconds() << std::endl;
        std::cout << std::endl;

        update(context);
      });
      render(context);
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

Win32WindowApp::Win32WindowApp(const Win32AppDesc& appDesc, bool createConsole)
: Win32App(appDesc, createConsole)
{

}

Win32WindowApp::~Win32WindowApp() = default;

void Win32WindowApp::MakeWindow(const AppCore::Window::WindowDesc& desc)
{
  m_mainWindow.push_back(std::make_unique<AppCore::Window::Window>(desc));
}

} // namespace AppCore::App::Win32
