#include <system_error>

#include "AppCore/App/Win32/Win32App.h"
#include "AppCore/Window/WindowDesc.h"
#include "Core/Config.h"
#include "Core/Win32/WindowsHeader.h"

namespace GfxPlaground
{
class GfxPlaygroundApp : public AppCore::App::Win32::Win32App
{
public:

  GfxPlaygroundApp(const AppCore::App::Win32::Win32AppDesc& appDesc, bool createConsole)
      : Win32App(appDesc, createConsole)
  {
  
  }

  void beforeMainLoop() override
  {
  
  }

  void afterMainLoop() override
  {
  
  }

  void init() override
  {

  }

  void shutdown() override
  {
  }

  void update([[maybe_unused]] const AppContext& context) override
  {
  }
  void render([[maybe_unused]] const AppContext& context) override
  {
  }
};
} // namespace GfxPlaground


std::unique_ptr<GfxPlaground::GfxPlaygroundApp> m_app;

int WinMain(HINSTANCE hInstance, [[maybe_unused]] HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
  bool createConsole = true;
  AppCore::App::Win32::Win32AppDesc appDesc;

  appDesc.m_appInstance = hInstance;
  appDesc.m_applicationName = "GfxPlaygroundApp";
  appDesc.m_cmdLine = lpCmdLine;
  appDesc.m_cmdShow = nShowCmd;

  m_app = std::make_unique<GfxPlaground::GfxPlaygroundApp>(appDesc, createConsole);

  m_app->init();

  auto result = m_app->run();

  m_app->shutdown();

  return EXIT_SUCCESS;
}
