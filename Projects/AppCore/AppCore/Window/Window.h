#pragma once

#include <memory>

#include "AppCore/Window/WindowDesc.h"

namespace AppCore::Window
{
class WindowImpl;

class Window
{
public:
  explicit Window(const WindowDesc& desc);
  ~Window();

private:
  std::unique_ptr<WindowImpl> m_pimpl;
};
} // namespace AppCore::Window