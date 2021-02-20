#include "AppCore/App/Win32/Win32App.h"
#include "AppCore/Window/WindowDesc.h"
#include "Core/Config.h"
#include "Core/Win32/WindowsHeader.h"
#include "fmt/format.h"

#include <system_error>

namespace BasicWindowApp
{
class BasicWindowApp final : public AppCore::App::Win32::Win32WindowApp
{
public:
  BasicWindowApp(const AppCore::App::Win32::Win32AppDesc& appDesc, bool createConsole)
  : Win32WindowApp(appDesc, createConsole)
  {
  }

  void init() override
  {
    AppCore::Window::WindowDesc desc;
    
    desc.m_x = 64;
    desc.m_y = 64;
    desc.m_width = 1280;
    desc.m_height = 720;

    desc.m_title = "MainWindow";
    desc.m_id = "BasicWin32WindowClass";
    desc.m_nativeAppDesc = getWin32AppDesc();

    MakeWindow(desc);
  }

  void shutdown() override
  {
  }

private:
  void beforeMainLoop() override
  {
  }

  void afterMainLoop() override
  {
  }

  void update([[maybe_unused]] const AppContext& context) override
  {
  }
  void render([[maybe_unused]] const AppContext& context) override
  {
  }
};

} // namespace BasicWindowApp

// Indicates to hybrid graphics systems to prefer the discrete part by default
extern "C"
{
  __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
  __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

namespace
{
std::unique_ptr<BasicWindowApp::BasicWindowApp> m_app;
}

int WinMain(HINSTANCE hInstance, [[maybe_unused]] HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
  bool createConsole = true;
  AppCore::App::Win32::Win32AppDesc appDesc;

  appDesc.m_appInstance = hInstance;
  appDesc.m_applicationName = "BasicWindowApp";
  appDesc.m_cmdLine = lpCmdLine;
  appDesc.m_cmdShow = nShowCmd;

  m_app = std::make_unique<BasicWindowApp::BasicWindowApp>(appDesc, createConsole);

  m_app->init();

  auto result = m_app->run();

  m_app->shutdown();

  m_app.reset();
  return EXIT_SUCCESS;
}
