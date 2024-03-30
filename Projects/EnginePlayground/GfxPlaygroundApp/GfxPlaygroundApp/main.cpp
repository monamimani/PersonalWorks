#include <system_error>

#include "ApplicationCore/Application/Win32/Win32Application.h"
#include "ApplicationCore/Window/WindowDesc.h"
#include "Core/Config.h"
#include "Core/Win32/WindowsHeader.h"

namespace GfxPlaground
{
class GfxPlaygroundApp : public ApplicationCore::Win32::Win32Application
{
public:
  GfxPlaygroundApp(const ApplicationCore::Win32::Win32ApplicationDesc& appDesc)
  : Win32Application(appDesc)
  {
  }

  // void update([[maybe_unused]] const AppContext& context) override
  //{
  //}
  // void render([[maybe_unused]] const AppContext& context) override
  //{
  //}
};
} // namespace GfxPlaground

std::unique_ptr<GfxPlaground::GfxPlaygroundApp> m_app;

int WinMain(HINSTANCE hInstance, [[maybe_unused]] HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
  ApplicationCore::Win32::Win32ApplicationDesc appDesc;

  appDesc.m_appInstance = hInstance;
  appDesc.m_applicationName = "GfxPlaygroundApp";
  appDesc.m_cmdLine = lpCmdLine;
  appDesc.m_cmdShow = nShowCmd;
  appDesc.m_createConsole = true;

  m_app = std::make_unique<GfxPlaground::GfxPlaygroundApp>(appDesc);

  m_app->initialize();

  m_app->update();

  m_app->shutdown();

  return EXIT_SUCCESS;
}
