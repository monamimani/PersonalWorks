#include "ApplicationCore/Window/Window.h"

#include "ApplicationCore/Window/WindowImpl.h"
#include "Core/Config.h"

#ifdef _WIN32
  #include "ApplicationCore/Window/Win32/Win32Window.h"
#endif

namespace ApplicationCore::Window
{

Window::Window(const WindowDesc& desc)
{
#ifdef _WIN32
  if constexpr (Core::platform == Core::Platform::Windows)
  {
    m_pimpl = std::make_unique<Win32::Win32Window>(desc);
  }
#endif

#ifdef __linux__
  if constexpr (Core::platform == Core::Platform::Linux)
  {
    (void)desc;
    m_pimpl = {};
  }
#endif
}

Window::~Window() = default;
} // namespace ApplicationCore::Window
