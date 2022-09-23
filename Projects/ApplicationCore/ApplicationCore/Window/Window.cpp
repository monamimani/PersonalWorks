#include "ApplicationCore/Window/Window.h"

#include "Core/Config.h"
#include "ApplicationCore/Window/WindowImpl.h"

#ifdef _WIN32
#include "ApplicationCore/Window/Win32/Win32Window.h"
#endif // DEBUG



namespace ApplicationCore::Window
{

Window::Window(const WindowDesc& desc)
{
  if constexpr (Core::platform == Core::Platform::Windows)
  {
    m_pimpl = std::make_unique<Win32::Win32Window>(desc);  
  }

}

Window::~Window() = default;
}


