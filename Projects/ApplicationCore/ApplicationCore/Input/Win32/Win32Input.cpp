#include "ApplicationCore/Input/Win32/Win32Input.h"

namespace ApplicationCore::Input::Win32
{

uint32_t Win32Input::convertKeyboardKeyToVirtualKeyCode(Keyboard::Key key)
{
  uint32_t virtualKey = 0;
  switch (key)
  {
    case Keyboard::Key::A:
      virtualKey = 'A';
      break;
    case Keyboard::Key::B:
      virtualKey = 'B';
      break;
    case Keyboard::Key::C:
      virtualKey = 'C';
      break;
    case Keyboard::Key::D:
      virtualKey = 'D';
      break;
    case Keyboard::Key::E:
      virtualKey = 'E';
      break;
    case Keyboard::Key::F:
      virtualKey = 'F';
      break;
    case Keyboard::Key::G:
      virtualKey = 'G';
      break;
    case Keyboard::Key::H:
      virtualKey = 'H';
      break;
    case Keyboard::Key::I:
      virtualKey = 'I';
      break;
    case Keyboard::Key::J:
      virtualKey = 'J';
      break;
    case Keyboard::Key::K:
      virtualKey = 'K';
      break;
    case Keyboard::Key::L:
      virtualKey = 'L';
      break;
    case Keyboard::Key::M:
      virtualKey = 'M';
      break;
    case Keyboard::Key::N:
      virtualKey = 'N';
      break;
    case Keyboard::Key::O:
      virtualKey = 'O';
      break;
    case Keyboard::Key::P:
      virtualKey = 'P';
      break;
    case Keyboard::Key::Q:
      virtualKey = 'Q';
      break;
    case Keyboard::Key::R:
      virtualKey = 'R';
      break;
    case Keyboard::Key::S:
      virtualKey = 'S';
      break;
    case Keyboard::Key::T:
      virtualKey = 'T';
      break;
    case Keyboard::Key::U:
      virtualKey = 'U';
      break;
    case Keyboard::Key::V:
      virtualKey = 'V';
      break;
    case Keyboard::Key::W:
      virtualKey = 'W';
      break;
    case Keyboard::Key::X:
      virtualKey = 'X';
      break;
    case Keyboard::Key::Y:
      virtualKey = 'Y';
      break;
    case Keyboard::Key::Z:
      virtualKey = 'Z';
      break;
    case Keyboard::Key::Num0:
      virtualKey = '0';
      break;
    case Keyboard::Key::Num1:
      virtualKey = '1';
      break;
    case Keyboard::Key::Num2:
      virtualKey = '2';
      break;
    case Keyboard::Key::Num3:
      virtualKey = '3';
      break;
    case Keyboard::Key::Num4:
      virtualKey = '4';
      break;
    case Keyboard::Key::Num5:
      virtualKey = '5';
      break;
    case Keyboard::Key::Num6:
      virtualKey = '6';
      break;
    case Keyboard::Key::Num7:
      virtualKey = '7';
      break;
    case Keyboard::Key::Num8:
      virtualKey = '8';
      break;
    case Keyboard::Key::Num9:
      virtualKey = '9';
      break;
    case Keyboard::Key::Escape:
      virtualKey = VK_ESCAPE;
      break;
    case Keyboard::Key::LControl:
      virtualKey = VK_LCONTROL;
      break;
    case Keyboard::Key::LShift:
      virtualKey = VK_LSHIFT;
      break;
    case Keyboard::Key::LAlt:
      virtualKey = VK_LMENU;
      break;
    case Keyboard::Key::LSystem:
      virtualKey = VK_LWIN;
      break;
    case Keyboard::Key::RControl:
      virtualKey = VK_RCONTROL;
      break;
    case Keyboard::Key::RShift:
      virtualKey = VK_RSHIFT;
      break;
    case Keyboard::Key::RAlt:
      virtualKey = VK_RMENU;
      break;
    case Keyboard::Key::RSystem:
      virtualKey = VK_RWIN;
      break;
    case Keyboard::Key::Menu:
      virtualKey = VK_APPS;
      break;
    case Keyboard::Key::LBracket:
      virtualKey = VK_OEM_4;
      break;
    case Keyboard::Key::RBracket:
      virtualKey = VK_OEM_6;
      break;
    case Keyboard::Key::Semicolon:
      virtualKey = VK_OEM_1;
      break;
    case Keyboard::Key::Comma:
      virtualKey = VK_OEM_COMMA;
      break;
    case Keyboard::Key::Period:
      virtualKey = VK_OEM_PERIOD;
      break;
    case Keyboard::Key::Quote:
      virtualKey = VK_OEM_7;
      break;
    case Keyboard::Key::Slash:
      virtualKey = VK_OEM_2;
      break;
    case Keyboard::Key::Backslash:
      virtualKey = VK_OEM_5;
      break;
    case Keyboard::Key::Tilde:
      virtualKey = VK_OEM_3;
      break;
    case Keyboard::Key::Equal:
      virtualKey = VK_OEM_PLUS;
      break;
    case Keyboard::Key::Hyphen:
      virtualKey = VK_OEM_MINUS;
      break;
    case Keyboard::Key::Space:
      virtualKey = VK_SPACE;
      break;
    case Keyboard::Key::Enter:
      virtualKey = VK_RETURN;
      break;
    case Keyboard::Key::Backspace:
      virtualKey = VK_BACK;
      break;
    case Keyboard::Key::Tab:
      virtualKey = VK_TAB;
      break;
    case Keyboard::Key::PageUp:
      virtualKey = VK_PRIOR;
      break;
    case Keyboard::Key::PageDown:
      virtualKey = VK_NEXT;
      break;
    case Keyboard::Key::End:
      virtualKey = VK_END;
      break;
    case Keyboard::Key::Home:
      virtualKey = VK_HOME;
      break;
    case Keyboard::Key::Insert:
      virtualKey = VK_INSERT;
      break;
    case Keyboard::Key::Delete:
      virtualKey = VK_DELETE;
      break;
    case Keyboard::Key::Add:
      virtualKey = VK_ADD;
      break;
    case Keyboard::Key::Subtract:
      virtualKey = VK_SUBTRACT;
      break;
    case Keyboard::Key::Multiply:
      virtualKey = VK_MULTIPLY;
      break;
    case Keyboard::Key::Divide:
      virtualKey = VK_DIVIDE;
      break;
    case Keyboard::Key::Left:
      virtualKey = VK_LEFT;
      break;
    case Keyboard::Key::Right:
      virtualKey = VK_RIGHT;
      break;
    case Keyboard::Key::Up:
      virtualKey = VK_UP;
      break;
    case Keyboard::Key::Down:
      virtualKey = VK_DOWN;
      break;
    case Keyboard::Key::Numpad0:
      virtualKey = VK_NUMPAD0;
      break;
    case Keyboard::Key::Numpad1:
      virtualKey = VK_NUMPAD1;
      break;
    case Keyboard::Key::Numpad2:
      virtualKey = VK_NUMPAD2;
      break;
    case Keyboard::Key::Numpad3:
      virtualKey = VK_NUMPAD3;
      break;
    case Keyboard::Key::Numpad4:
      virtualKey = VK_NUMPAD4;
      break;
    case Keyboard::Key::Numpad5:
      virtualKey = VK_NUMPAD5;
      break;
    case Keyboard::Key::Numpad6:
      virtualKey = VK_NUMPAD6;
      break;
    case Keyboard::Key::Numpad7:
      virtualKey = VK_NUMPAD7;
      break;
    case Keyboard::Key::Numpad8:
      virtualKey = VK_NUMPAD8;
      break;
    case Keyboard::Key::Numpad9:
      virtualKey = VK_NUMPAD9;
      break;
    case Keyboard::Key::F1:
      virtualKey = VK_F1;
      break;
    case Keyboard::Key::F2:
      virtualKey = VK_F2;
      break;
    case Keyboard::Key::F3:
      virtualKey = VK_F3;
      break;
    case Keyboard::Key::F4:
      virtualKey = VK_F4;
      break;
    case Keyboard::Key::F5:
      virtualKey = VK_F5;
      break;
    case Keyboard::Key::F6:
      virtualKey = VK_F6;
      break;
    case Keyboard::Key::F7:
      virtualKey = VK_F7;
      break;
    case Keyboard::Key::F8:
      virtualKey = VK_F8;
      break;
    case Keyboard::Key::F9:
      virtualKey = VK_F9;
      break;
    case Keyboard::Key::F10:
      virtualKey = VK_F10;
      break;
    case Keyboard::Key::F11:
      virtualKey = VK_F11;
      break;
    case Keyboard::Key::F12:
      virtualKey = VK_F12;
      break;
    case Keyboard::Key::F13:
      virtualKey = VK_F13;
      break;
    case Keyboard::Key::F14:
      virtualKey = VK_F14;
      break;
    case Keyboard::Key::F15:
      virtualKey = VK_F15;
      break;
    case Keyboard::Key::Pause:
      virtualKey = VK_PAUSE;
      break;
    default:
      virtualKey = 0;
      break;
  }

  return virtualKey;
}

Keyboard::Key Win32Input::convertVirtualKeyCodeToKeyboardKey(WPARAM key, LPARAM flags)
{
  switch (key)
  {
    case VK_SHIFT:
    {
      // Check the scancode to distinguish between left and right shift
      static uint32_t lShift   = MapVirtualKeyW(VK_LSHIFT, MAPVK_VK_TO_VSC);
      uint32_t        scancode = static_cast<uint32_t>((flags & (0xFF << 16)) >> 16);
      return scancode == lShift ? Input::Keyboard::Key::LShift : Input::Keyboard::Key::RShift;
    }

    case VK_MENU:
      // Check the "extended" flag to distinguish between left and right alt
      return (HIWORD(flags) & KF_EXTENDED) ? Input::Keyboard::Key::RAlt : Input::Keyboard::Key::LAlt;

    case VK_CONTROL:
      // Check the "extended" flag to distinguish between left and right control
      return (HIWORD(flags) & KF_EXTENDED) ? Input::Keyboard::Key::RControl : Input::Keyboard::Key::LControl;

    // Other keys are reported properly
    case VK_LWIN:
      return Input::Keyboard::Key::LSystem;
    case VK_RWIN:
      return Input::Keyboard::Key::RSystem;
    case VK_APPS:
      return Input::Keyboard::Key::Menu;
    case VK_OEM_1:
      return Input::Keyboard::Key::Semicolon;
    case VK_OEM_2:
      return Input::Keyboard::Key::Slash;
    case VK_OEM_PLUS:
      return Input::Keyboard::Key::Equal;
    case VK_OEM_MINUS:
      return Input::Keyboard::Key::Hyphen;
    case VK_OEM_4:
      return Input::Keyboard::Key::LBracket;
    case VK_OEM_6:
      return Input::Keyboard::Key::RBracket;
    case VK_OEM_COMMA:
      return Input::Keyboard::Key::Comma;
    case VK_OEM_PERIOD:
      return Input::Keyboard::Key::Period;
    case VK_OEM_7:
      return Input::Keyboard::Key::Quote;
    case VK_OEM_5:
      return Input::Keyboard::Key::Backslash;
    case VK_OEM_3:
      return Input::Keyboard::Key::Tilde;
    case VK_ESCAPE:
      return Input::Keyboard::Key::Escape;
    case VK_SPACE:
      return Input::Keyboard::Key::Space;
    case VK_RETURN:
      return Input::Keyboard::Key::Enter;
    case VK_BACK:
      return Input::Keyboard::Key::Backspace;
    case VK_TAB:
      return Input::Keyboard::Key::Tab;
    case VK_PRIOR:
      return Input::Keyboard::Key::PageUp;
    case VK_NEXT:
      return Input::Keyboard::Key::PageDown;
    case VK_END:
      return Input::Keyboard::Key::End;
    case VK_HOME:
      return Input::Keyboard::Key::Home;
    case VK_INSERT:
      return Input::Keyboard::Key::Insert;
    case VK_DELETE:
      return Input::Keyboard::Key::Delete;
    case VK_ADD:
      return Input::Keyboard::Key::Add;
    case VK_SUBTRACT:
      return Input::Keyboard::Key::Subtract;
    case VK_MULTIPLY:
      return Input::Keyboard::Key::Multiply;
    case VK_DIVIDE:
      return Input::Keyboard::Key::Divide;
    case VK_PAUSE:
      return Input::Keyboard::Key::Pause;
    case VK_F1:
      return Input::Keyboard::Key::F1;
    case VK_F2:
      return Input::Keyboard::Key::F2;
    case VK_F3:
      return Input::Keyboard::Key::F3;
    case VK_F4:
      return Input::Keyboard::Key::F4;
    case VK_F5:
      return Input::Keyboard::Key::F5;
    case VK_F6:
      return Input::Keyboard::Key::F6;
    case VK_F7:
      return Input::Keyboard::Key::F7;
    case VK_F8:
      return Input::Keyboard::Key::F8;
    case VK_F9:
      return Input::Keyboard::Key::F9;
    case VK_F10:
      return Input::Keyboard::Key::F10;
    case VK_F11:
      return Input::Keyboard::Key::F11;
    case VK_F12:
      return Input::Keyboard::Key::F12;
    case VK_F13:
      return Input::Keyboard::Key::F13;
    case VK_F14:
      return Input::Keyboard::Key::F14;
    case VK_F15:
      return Input::Keyboard::Key::F15;
    case VK_LEFT:
      return Input::Keyboard::Key::Left;
    case VK_RIGHT:
      return Input::Keyboard::Key::Right;
    case VK_UP:
      return Input::Keyboard::Key::Up;
    case VK_DOWN:
      return Input::Keyboard::Key::Down;
    case VK_NUMPAD0:
      return Input::Keyboard::Key::Numpad0;
    case VK_NUMPAD1:
      return Input::Keyboard::Key::Numpad1;
    case VK_NUMPAD2:
      return Input::Keyboard::Key::Numpad2;
    case VK_NUMPAD3:
      return Input::Keyboard::Key::Numpad3;
    case VK_NUMPAD4:
      return Input::Keyboard::Key::Numpad4;
    case VK_NUMPAD5:
      return Input::Keyboard::Key::Numpad5;
    case VK_NUMPAD6:
      return Input::Keyboard::Key::Numpad6;
    case VK_NUMPAD7:
      return Input::Keyboard::Key::Numpad7;
    case VK_NUMPAD8:
      return Input::Keyboard::Key::Numpad8;
    case VK_NUMPAD9:
      return Input::Keyboard::Key::Numpad9;
    case 'A':
      return Input::Keyboard::Key::A;
    case 'Z':
      return Input::Keyboard::Key::Z;
    case 'E':
      return Input::Keyboard::Key::E;
    case 'R':
      return Input::Keyboard::Key::R;
    case 'T':
      return Input::Keyboard::Key::T;
    case 'Y':
      return Input::Keyboard::Key::Y;
    case 'U':
      return Input::Keyboard::Key::U;
    case 'I':
      return Input::Keyboard::Key::I;
    case 'O':
      return Input::Keyboard::Key::O;
    case 'P':
      return Input::Keyboard::Key::P;
    case 'Q':
      return Input::Keyboard::Key::Q;
    case 'S':
      return Input::Keyboard::Key::S;
    case 'D':
      return Input::Keyboard::Key::D;
    case 'F':
      return Input::Keyboard::Key::F;
    case 'G':
      return Input::Keyboard::Key::G;
    case 'H':
      return Input::Keyboard::Key::H;
    case 'J':
      return Input::Keyboard::Key::J;
    case 'K':
      return Input::Keyboard::Key::K;
    case 'L':
      return Input::Keyboard::Key::L;
    case 'M':
      return Input::Keyboard::Key::M;
    case 'W':
      return Input::Keyboard::Key::W;
    case 'X':
      return Input::Keyboard::Key::X;
    case 'C':
      return Input::Keyboard::Key::C;
    case 'V':
      return Input::Keyboard::Key::V;
    case 'B':
      return Input::Keyboard::Key::B;
    case 'N':
      return Input::Keyboard::Key::N;
    case '0':
      return Input::Keyboard::Key::Num0;
    case '1':
      return Input::Keyboard::Key::Num1;
    case '2':
      return Input::Keyboard::Key::Num2;
    case '3':
      return Input::Keyboard::Key::Num3;
    case '4':
      return Input::Keyboard::Key::Num4;
    case '5':
      return Input::Keyboard::Key::Num5;
    case '6':
      return Input::Keyboard::Key::Num6;
    case '7':
      return Input::Keyboard::Key::Num7;
    case '8':
      return Input::Keyboard::Key::Num8;
    case '9':
      return Input::Keyboard::Key::Num9;
  }

  return Input::Keyboard::Key::Unknown;
}

bool Win32Input::isKeyPressed(Keyboard::Key key)
{
  auto virtualKey = convertKeyboardKeyToVirtualKeyCode(key);
  return (GetAsyncKeyState(virtualKey) & 0x8000) != 0;
}

bool Win32Input::isMouseButtonPressed(Mouse::Button button)
{
  int virtualKey = 0;
  switch (button)
  {
    case Mouse::Button::Left:
      virtualKey = GetSystemMetrics(SM_SWAPBUTTON) ? VK_RBUTTON : VK_LBUTTON;
      break;
    case Mouse::Button::Right:
      virtualKey = GetSystemMetrics(SM_SWAPBUTTON) ? VK_LBUTTON : VK_RBUTTON;
      break;
    case Mouse::Button::Middle:
      virtualKey = VK_MBUTTON;
      break;
    case Mouse::Button::XButton1:
      virtualKey = VK_XBUTTON1;
      break;
    case Mouse::Button::XButton2:
      virtualKey = VK_XBUTTON2;
      break;
    default:
      virtualKey = 0;
      break;
  }

  return (GetAsyncKeyState(virtualKey) & 0x8000) != 0;
}

Mouse::Position Win32Input::getMousePosition()
{
  POINT point;
  GetCursorPos(&point);
  return Mouse::Position{point.x, point.y};
}

void Win32Input::setMousePosition(const Mouse::Position& position)
{
  SetCursorPos(position.x, position.y);
}

} // namespace ApplicationCore::Input::Win32
