#pragma once

#include "ApplicationCore/Input/Keyboard.h"
#include "ApplicationCore/Input/Mouse.h"
#include "Core/Win32/WindowsHeader.h"

#include <cstdint>

namespace ApplicationCore::Input::Win32
{
class Win32Input
{

public:
  static uint32_t      convertKeyboardKeyToVirtualKeyCode(Keyboard::Key);
  static Keyboard::Key convertVirtualKeyCodeToKeyboardKey(WPARAM key, LPARAM flags);

  static bool isKeyPressed(Keyboard::Key key);

  static bool isMouseButtonPressed(Mouse::Button button);

  static Mouse::Position getMousePosition();

  //static Vector2i getMousePosition(const WindowBase& relativeTo);

  static void setMousePosition(const Mouse::Position& position);

  //static void setMousePosition(const Vector2i& position, const WindowBase& relativeTo);

private:
};
} // namespace ApplicationCore::Input::Win32