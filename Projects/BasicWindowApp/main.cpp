#include <system_error>

#include "ApplicationCore/Application/Win32/Win32Application.h"
#include "ApplicationCore/Application/Win32/Win32ApplicationMainLoop.h"
#include "ApplicationCore/Application/Win32/Win32MessageLoop.h"
#include "ApplicationCore/Window/WindowDesc.h"
#include "Core/Config.h"
#include "Core/Win32/WindowsHeader.h"
#include <format>
#include <iostream>

namespace BasicWindowApp
{
class BasicWindowApp final : public ApplicationCore::Win32::Win32WindowApplication
{
public:
  BasicWindowApp(const ApplicationCore::Win32::Win32ApplicationDesc& appDesc)
  : Win32WindowApplication(appDesc)
  {
  }

  bool initialize() override
  {
    ApplicationCore::Window::WindowDesc desc;
    desc.m_x = 64;
    desc.m_y = 64;
    desc.m_width = 1280;
    desc.m_height = 720;
    desc.m_title = "MainWindow";
    desc.m_id = "BasicWin32WindowClass";
    desc.m_nativeAppDesc = getWin32AppDesc();
    MakeWindow(desc);
    return true;
  }
  void shutdown() override {}
  void update() override {}

private:
  // void update([[maybe_unused]] const AppContext& context) override {}
  // void render([[maybe_unused]] const AppContext& context) override {}
};
static_assert(ApplicationCore::Application_C<BasicWindowApp>);

} // namespace BasicWindowApp

// Indicates to hybrid graphics systems to prefer the discrete part by default
extern "C"
{
  // http://developer.download.nvidia.com/devzone/devcenter/gamegraphics/files/OptimusRenderingPolicies.pdf
  // The following line indicate that we prefer the high performance NVIDIA GPU if there are multiple GPUs
  // It has to be in the .exe module to be correctly detected.
  __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;

  // The AMD equivalent
  // Also has to be in the .exe module to be correctly detected.
  __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

namespace
{
std::unique_ptr<BasicWindowApp::BasicWindowApp> m_app;
}


namespace
{
  HMODULE getApplicationModuleHandle()
  {
    //Returns module handle where this function is running in: EXE or DLL
    HMODULE hModule{};
    ::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, nullptr, &hModule);
    return hModule;
  }

  HMODULE getThisModuleHandle()
  {
    //Returns module handle where this function is running in: EXE or DLL
    HMODULE hModule{};
    ::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCTSTR)getThisModuleHandle, &hModule);

    return hModule;
  }

  int ApplicationMain([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
  {
    ApplicationCore::Win32::Win32ApplicationDesc appDesc;

    STARTUPINFO startupInfo{};
    GetStartupInfo(&startupInfo);

    appDesc.m_appInstance = getApplicationModuleHandle();
    appDesc.m_applicationName = "BasicWindowApp";
    appDesc.m_createConsole = true;
    appDesc.m_cmdLine = std::vector<std::string>(argv, argv + argc);
    appDesc.m_cmdShow = startupInfo.wShowWindow;

    m_app = ApplicationCore::Application::CreateApplication<BasicWindowApp::BasicWindowApp>(appDesc);

    ApplicationCore::Win32::Win32MessageLoop win32MessageLoop;
    auto returnValue = ApplicationCore::Win32::Win32ApplicationMainLoop::runMainLoop(*m_app, win32MessageLoop);

    m_app.reset();
    return returnValue;

    return EXIT_SUCCESS;
  }
} // namespace

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
  return ApplicationMain(argc, argv);
}

int APIENTRY WinMain([[maybe_unused]] HINSTANCE hInstance, [[maybe_unused]] HINSTANCE hPrevInstance, [[maybe_unused]] LPSTR cmdline, [[maybe_unused]] int cmdshow)
{
  return ApplicationMain(__argc, __argv);
}