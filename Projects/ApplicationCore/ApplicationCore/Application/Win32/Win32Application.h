#pragma once

#include <memory>
#include <string>
#include <vector>

#include "ApplicationCore/Application/Application.h"
#include "ApplicationCore/Application/Win32/Win32ApplicationDesc.h"
#include "ApplicationCore/Window/WindowDesc.h"
#include "Core/Win32/WindowsHeader.h"

namespace ApplicationCore
{
namespace Window
{
  class Window;
}

namespace Win32
{
  class Win32Application : public Application
  {
  public:
    Win32Application(const Win32ApplicationDesc& appDesc);
    ~Win32Application();

    void update() override;

  protected:
    HINSTANCE getHInstance() const { return m_appInstance; }
    Win32ApplicationDesc getWin32AppDesc() const { return m_win32AppDesc; }

    // virtual void update(const AppContext& context) = 0;
    // virtual void render(const AppContext& context) = 0;

  private:
    void pumpMessages();

    HINSTANCE m_appInstance;
    Win32ApplicationDesc m_win32AppDesc;
  };

  class Win32WindowApplication : public Win32Application
  {
  public:
    Win32WindowApplication(const Win32ApplicationDesc& appDesc);
    ~Win32WindowApplication();

  protected:
    void MakeWindow(const ApplicationCore::Window::WindowDesc& desc);

  private:
    std::vector<std::unique_ptr<Window::Window>> m_windowList;
  };
} // namespace Win32
} // namespace ApplicationCore
