#pragma once

#include "AppCore/App/App.h"
#include "AppCore/App/Win32/Win32AppDesc.h"
#include "AppCore/Window/WindowDesc.h"
#include "Core/Win32/WindowsHeader.h"


#include <memory>
#include <string>
#include <vector>

namespace AppCore
{
namespace Window
{
  class Window;
}

namespace App::Win32
{
  class Win32App : public App
  {
  public:
    Win32App(const Win32AppDesc& appDesc, bool createConsole);
    ~Win32App();

    int run() override;

  protected:
    HINSTANCE getHInstance() const { return m_appInstance; }
    Win32AppDesc getWin32AppDesc() const
    {
      return m_win32AppDesc;
    }

    virtual void update(const AppContext& context) = 0;
    virtual void render(const AppContext& context) = 0;

  private:
    HINSTANCE m_appInstance;
    Win32AppDesc m_win32AppDesc;
  };

  class Win32WindowApp : public Win32App
  {
  public:
    Win32WindowApp(const Win32AppDesc& appDesc, bool createConsole);
    ~Win32WindowApp();
  protected:
    void MakeWindow(const AppCore::Window::WindowDesc& desc);
  private:
    std::vector<std::unique_ptr<Window::Window>> m_mainWindow;
  };
} // namespace App::Win32
} // namespace AppCore
