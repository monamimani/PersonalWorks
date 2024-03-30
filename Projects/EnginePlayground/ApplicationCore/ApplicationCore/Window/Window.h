#pragma once

#include <memory>

#include "ApplicationCore/Window/WindowDesc.h"

namespace ApplicationCore::Window
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
} // namespace ApplicationCore::Window