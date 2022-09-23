#include "ApplicationCore/Application/Win32/Win32Application.h"

#include <iostream>
#include <stdexcept>

#include "ApplicationCore/Window/Window.h"

namespace ApplicationCore::Win32
{

Win32Application::Win32Application(const Win32ApplicationDesc& appDesc)
: Application(appDesc.m_applicationName)
, m_appInstance{appDesc.m_appInstance}
, m_win32AppDesc{appDesc}
{

  if (appDesc.m_createConsole)
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

  // m_timer.SetFixedTimeStep(true);
  // m_timer.SetTargetDeltaTimeInSeconds(std::chrono::seconds(1) / 60.0);
}

Win32Application::~Win32Application() = default;

void Win32Application::pumpMessages()
{

  MSG systemMsg{};

  while (::PeekMessage(&systemMsg, nullptr, 0, 0, PM_REMOVE))
  {
    // Handle messages
    ::TranslateMessage(&systemMsg);
    ::DispatchMessage(&systemMsg);
  }

  // if (systemMsg.message == WM_QUIT)
  //{
  //  setRunning(false);
  //  break;
  //}
}

void Win32Application::update()
{
  // MSG systemMsg{};
  // try
  //{
  //  while (isRunning())
  //  {
  //    pumpMessages();

  //    AppContext context;
  //    context.m_appFrameTimeContext = m_timer.getFrameTimeContext();

  //    // Application code
  //    m_timer.tick([this, context]() {
  //      std::cout << "Timer stats" << std::endl;
  //      std::cout << "GetDeltaTimeInSeconds " << m_timer.getDeltaTimeInSeconds() << std::endl;
  //      std::cout << "GetFrameCount " << m_timer.getFrameCount() << std::endl;
  //      std::cout << "GetFramesPerSecond " << m_timer.getFramesPerSecond() << std::endl;
  //      std::cout << "GetTotalSeconds " << m_timer.getTotalSeconds() << std::endl;
  //      std::cout << std::endl;

  //      update(context);
  //    });
  //    render(context);
  //  }
  //}
  // catch (const std::runtime_error& e)
  //{
  //  std::cerr << e.what() << std::endl;
  //  OutputDebugStringA(e.what());
  //  return EXIT_FAILURE;
  //}

  // return (int)systemMsg.wParam;
}

Win32WindowApplication::Win32WindowApplication(const Win32ApplicationDesc& appDesc)
: Win32Application(appDesc)
{
}

Win32WindowApplication::~Win32WindowApplication() = default;

void Win32WindowApplication::MakeWindow(const ApplicationCore::Window::WindowDesc& desc)
{
  m_windowList.push_back(std::make_unique<ApplicationCore::Window::Window>(desc));
}

} // namespace ApplicationCore::Win32
