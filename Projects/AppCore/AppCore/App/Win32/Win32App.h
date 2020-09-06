#pragma once

#include <memory>
#include <string>

#include "AppCore/App/App.h"
#include "AppCore/App/Win32/Win32AppDesc.h"
#include "Core/Win32/WindowsHeader.h"

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

  private:
    HINSTANCE m_appInstance;


    std::unique_ptr<Window::Window> m_mainWindow;
  };
} // namespace App::Win32
} // namespace AppCore
