
#include "ApplicationCore/Window/Win32/Win32Window.h"

#include "ApplicationCore/Window/Event.h"
#include "Core/Assert.h"
#include "Core/Win32/Win32Utility.h"
#include <format>

#include <iostream>
#include <optional>
#include <stdexcept>
#include <unordered_map>
#include <windowsx.h>

namespace ApplicationCore::Window::Win32
{

LRESULT windowClassProcDispatcher(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

std::optional<DEVMODE> findBestDisplayMode(uint32_t width, uint32_t height, uint32_t bpp)
{
  std::optional<DEVMODE> best = {};

  DEVMODE dm = {};
  dm.dmSize = sizeof(DEVMODE);

  uint32_t maxFreq = 0;

  for (uint32_t n = 0; EnumDisplaySettings(nullptr, n, &dm); n++)
  {
    if (dm.dmPelsWidth == width && dm.dmPelsHeight == height && dm.dmBitsPerPel == bpp)
    {
      if (dm.dmFields & DM_ORIENTATION && dm.dmOrientation != DMDO_DEFAULT)
      {
        // only select default orientation
        continue;
      }

      if (dm.dmDisplayFrequency > maxFreq)
      {
        maxFreq = dm.dmDisplayFrequency;
        best = dm;
      }
    }
  }

  return best;
}

struct BasicWin32WindowClass
{
  BasicWin32WindowClass(HINSTANCE appInstance)
  {

    // Setup m_windowHandle class attributes.
    auto intResource = IDI_APPLICATION;
    windowClassEx.cbSize = sizeof(WNDCLASSEX);
    windowClassEx.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    windowClassEx.lpfnWndProc = &windowClassProcDispatcher;
    windowClassEx.cbClsExtra = 0;
    windowClassEx.cbWndExtra = 0;
    windowClassEx.hInstance = appInstance;
    windowClassEx.hIcon = LoadIcon(windowClassEx.hInstance, intResource);
    windowClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClassEx.hbrBackground = static_cast<HBRUSH>(GetStockObject(DKGRAY_BRUSH));
    windowClassEx.lpszMenuName = NULL;
    windowClassEx.lpszClassName = BasicWin32WindowClass::className;
    windowClassEx.hIconSm = LoadIcon(windowClassEx.hInstance, intResource);

    if (!::RegisterClassEx(&windowClassEx))
    {
      throw(std::runtime_error("Failed to register the window class."));
    }
  }

  ~BasicWin32WindowClass()
  {
    ::UnregisterClass(windowClassEx.lpszClassName, windowClassEx.hInstance);
  }

  static constexpr auto className = "BasicWin32WindowClassEx";

  WNDCLASSEX windowClassEx{};
};

static std::shared_ptr<BasicWin32WindowClass> getBasicWin32WindowClass(HINSTANCE appInstance)
{
  static auto basicWin32WindowClass = std::make_shared<BasicWin32WindowClass>(appInstance);
  return basicWin32WindowClass;
}

Win32Window::Win32Window(const WindowDesc& desc)
: m_desc{desc}
, m_lastCursor(LoadCursor(NULL, IDC_ARROW))
{
  try
  {
    m_nativeDesc = std::any_cast<ApplicationCore::Win32::Win32ApplicationDesc&>(m_desc.m_nativeAppDesc);

    m_windowClass = getBasicWin32WindowClass(m_nativeDesc.m_appInstance);

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    if (m_desc.m_isFullscreen)
    {
      auto bestDevModeScreenSetting = findBestDisplayMode(screenWidth, screenHeight, 32);

      if (bestDevModeScreenSetting)
      {
        if (ChangeDisplaySettings(&bestDevModeScreenSetting.value(), CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
        {
          if (MessageBox(NULL, "Fullscreen Display Settings not supported!\n Switch to window mode?", "Error", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
          {
            m_desc.m_isFullscreen = false;
          }
          else
          {
            throw(std::runtime_error("Failed to change fullscreen display setting."));
          }
        }
      }
    }

    uint32_t dwExStyle = 0;
    int32_t dwStyle = 0;
    int32_t width = 0;
    int32_t height = 0;
    enum class Style : uint32_t
    {
      Windowed = WS_OVERLAPPEDWINDOW,
      AeroBorderless = WS_POPUP | WS_THICKFRAME,
      BasicBorderless = WS_CAPTION | WS_OVERLAPPED | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU

    };

    if (m_desc.m_isFullscreen)
    {
      dwExStyle = WS_EX_APPWINDOW;
      dwStyle = WS_POPUP | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

      width = screenWidth;
      height = screenHeight;
    }
    else
    {
      dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
      // dwExStyle &=
      //    ~(WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);

      width = (int32_t)m_desc.m_width;
      height = (int32_t)m_desc.m_height;
    }

    //dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
    //dwStyle = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

    RECT clientRectAdjusted{
        (int32_t)m_desc.m_x,
        (int32_t)m_desc.m_y,
        width,
        height};
    AdjustWindowRectEx(&clientRectAdjusted, dwStyle, false, dwExStyle);

    auto windowTitle = m_desc.m_title.c_str();

    // Setup m_windowHandle initialization attributes.
    CREATESTRUCT cs;
    ZeroMemory(&cs, sizeof(cs));

    cs.x = m_desc.m_x;                                          // Window X position
    cs.y = m_desc.m_y;                                          // Window Y position
    cs.cx = clientRectAdjusted.right - clientRectAdjusted.left; // Window width
    cs.cy = clientRectAdjusted.bottom - clientRectAdjusted.top; // Window height
    cs.hInstance = m_nativeDesc.m_appInstance;                  // Window instance.
    cs.lpszClass = m_windowClass->windowClassEx.lpszClassName;  // Window class name
    cs.lpszName = m_desc.m_title.c_str();                       // Window title
    cs.style = dwStyle;                                         // Window style
    cs.dwExStyle = dwExStyle;                                   // Window extended style
    cs.lpCreateParams = this;                                   // Create params

    // Create the m_windowHandle.
    m_windowHandle = ::CreateWindowEx(cs.dwExStyle, cs.lpszClass, cs.lpszName, cs.style, cs.x, cs.y, cs.cx, cs.cy, cs.hwndParent, cs.hMenu, cs.hInstance, cs.lpCreateParams);

    if (!m_windowHandle)
    {
      throw(std::runtime_error(std::format("Failed to create the window. LastError: {}", Win32Util::GetLastErrorAsString())));
    }

    m_previousSize.width = width;
    m_previousSize.height = height;

    if (!m_desc.m_isFullscreen && m_desc.m_isCentered)
    {
      // Center on screen
      uint32_t x = (GetSystemMetrics(SM_CXSCREEN) - clientRectAdjusted.right) / 2;
      uint32_t y = (GetSystemMetrics(SM_CYSCREEN) - clientRectAdjusted.bottom) / 2;
      SetWindowPos(m_windowHandle, 0, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
    }

    if (m_desc.m_isVisible)
    {
      ShowWindow(m_windowHandle, SW_SHOW);
      SetForegroundWindow(m_windowHandle);
      SetFocus(m_windowHandle);
    }

    ::UpdateWindow(m_windowHandle);
  }
  catch (std::exception e)
  {

    std::cerr << e.what() << std::endl;
    OutputDebugString(e.what());
    std::abort();
  }
  catch (...)
  {
    //TODO: Deal with excpetions
    std::abort();
  }
}

Win32Window::~Win32Window()
{
}

void Win32Window::setGrabCursor(bool isGrabbed)
{
  m_cursorGrabbed = isGrabbed;
  execGrabCursor(m_cursorGrabbed);
}

void Win32Window::setKeyRepeatedEnabled(bool enabled)
{
  m_keyRepeatEnabled = enabled;
}

void Win32Window::setMouseCursorVisible(bool visible)
{
  m_cursorVisible = visible;
  SetCursor(m_cursorVisible ? m_lastCursor : nullptr);
}

void Win32Window::cleanup()
{
  setTracking(false);

  ReleaseCapture();
}

Size Win32Window::getSize() const
{
  RECT rect;
  GetClientRect(m_windowHandle, &rect);

  return Size(rect.right - rect.left, rect.bottom - rect.top);
}

void Win32Window::execGrabCursor(bool isGrabbed)
{
  if (isGrabbed)
  {
    RECT rect;
    GetClientRect(m_windowHandle, &rect);
    MapWindowPoints(m_windowHandle, nullptr, reinterpret_cast<LPPOINT>(&rect), 2);
    ClipCursor(&rect);
  }
  else
  {
    ClipCursor(nullptr);
  }
}

void Win32Window::setTracking(bool enabled)
{
  TRACKMOUSEEVENT mouseEvent;
  mouseEvent.cbSize = sizeof(TRACKMOUSEEVENT);
  mouseEvent.dwFlags = enabled ? TME_LEAVE : TME_CANCEL;
  mouseEvent.hwndTrack = m_windowHandle;
  mouseEvent.dwHoverTime = HOVER_DEFAULT;
  TrackMouseEvent(&mouseEvent);
}

Input::Keyboard::Key Win32Window::convertVirtualKeyCode(WPARAM key, LPARAM flags)
{
  switch (key)
  {
    case VK_SHIFT:
    {
      // Check the scancode to distinguish between left and right shift
      static uint32_t lShift = MapVirtualKeyW(VK_LSHIFT, MAPVK_VK_TO_VSC);
      uint32_t scancode = static_cast<uint32_t>((flags & (0xFF << 16)) >> 16);
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

Event::KeyModifierState getKeyModifierState()
{
  Event::KeyModifierState modifierState;
  modifierState.alt = HIWORD(GetKeyState(VK_MENU)) != 0;
  modifierState.ctrl = HIWORD(GetKeyState(VK_CONTROL)) != 0;
  modifierState.shift = HIWORD(GetKeyState(VK_SHIFT)) != 0;
  modifierState.system = HIWORD(GetKeyState(VK_LWIN)) || HIWORD(GetKeyState(VK_RWIN));
  return modifierState;
}

LRESULT Win32Window::processEvent(UINT msg, WPARAM wParam, LPARAM lParam)
{
  return DefWindowProc(m_windowHandle, msg, wParam, lParam);
  //https://docs.microsoft.com/en-us/windows/win32/winmsg/window-notifications
  //https://docs.microsoft.com/en-us/windows/win32/inputdev/keyboard-input-notifications
  LRESULT result = 0;
  switch (msg)
  {
    case WM_DESTROY:
    {
      cleanup();
      PostQuitMessage(EXIT_SUCCESS);
      result = EXIT_SUCCESS;

      break;
    }
    //case WM_SETCURSOR:
    //{
    //  // The mouse has moved, if the cursor is in our window we must refresh the cursor
    //  if (LOWORD(lParam) == HTCLIENT)
    //  {
    //    SetCursor(m_cursorVisible ? m_lastCursor : NULL);
    //  }

    //  break;
    //}
    case WM_CLOSE:
    {
      //DestroyWindow(m_windowHandle);
      auto event = Event{Event::CloseData()};
      pushEvent(event);
      result = EXIT_SUCCESS;
      break;
    }
    case WM_SIZE:
    {
      if (wParam != SIZE_MINIMIZED && !m_resizing && m_previousSize != getSize())
      {
        auto currentSize = getSize();

        auto eventData = Event::ResizeData();
        eventData.width = currentSize.width;
        eventData.height = currentSize.height;
        auto event = Event{eventData};
        pushEvent(event);

        m_previousSize = currentSize;

        execGrabCursor(m_cursorGrabbed);
      }

      break;
    }
    case WM_ENTERSIZEMOVE:
    {
      // Start resizing
      m_resizing = true;
      execGrabCursor(false);
      break;
    }
    case WM_EXITSIZEMOVE:
    {
      // Stop resizing
      m_resizing = false;

      // This event can also mean a window move. So we ignore if the window was just moved and not resized
      if (m_previousSize != getSize())
      {
        auto currentSize = getSize();

        auto eventData = Event::ResizeData();
        eventData.width = currentSize.width;
        eventData.height = currentSize.height;
        auto event = Event{eventData};
        pushEvent(event);

        m_previousSize = currentSize;
      }

      execGrabCursor(m_cursorGrabbed);
      break;
    }
    case WM_GETMINMAXINFO:
    {
      //TODO Understand what this is for.
      MINMAXINFO* minMaxInfo = reinterpret_cast<MINMAXINFO*>(lParam);
      minMaxInfo->ptMaxTrackSize.x = 50000;
      minMaxInfo->ptMaxTrackSize.y = 50000;
      break;
    }
    case WM_SETFOCUS:
    {
      execGrabCursor(m_cursorGrabbed);

      auto eventData = Event::FocusData();
      eventData.takeFocus = true;

      pushEvent(Event{eventData});
      break;
    }
    case WM_KILLFOCUS:
    {
      execGrabCursor(false);

      auto eventData = Event::FocusData();
      eventData.takeFocus = false;

      pushEvent(Event{eventData});
      break;
    }
    case WM_CHAR:
    {
      if (m_keyRepeatEnabled || (lParam & (1 << 30)) == 0)
      {
        uint32_t character = static_cast<uint32_t>(wParam);

        if ((character >= 0xD800) && (character <= 0xDBFF))
        {
          // First part of a surrogate pair: store it and wait for the second one
          m_surrogate = static_cast<uint16_t>(character);
        }
        else
        {
          // Check if it is the second part of a surrogate pair, or a regular character
          if ((character >= 0xDC00) && (character <= 0xDFFF))
          {
            // Convert the UTF-16 surrogate pair to a single UTF-32 value
            uint16_t utf16[] = {m_surrogate, static_cast<uint16_t>(character)};
            CORE_ASSERT(false, "Investigate if this can be done in std c++.");
            //sf::Utf16::toUtf32(utf16, utf16 + 2, &character);
            m_surrogate = 0;
          }

          auto eventData = Event::FocusData();
          eventData.takeFocus = true;

          pushEvent(Event{eventData});
        }
      }
      break;
    }
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
    {
      if (m_keyRepeatEnabled || ((HIWORD(lParam) & KF_REPEAT) == 0))
      {
        auto eventData = Event::KeyboardData();
        eventData.keyState = Event::ButtonState::Pressed;
        eventData.modifier = getKeyModifierState();
        eventData.keyCode = convertVirtualKeyCode(wParam, lParam);

        pushEvent(Event{eventData});
      }
      break;
    }
    case WM_KEYUP:
    case WM_SYSKEYUP:
    {
      auto eventData = Event::KeyboardData();
      eventData.keyState = Event::ButtonState::Released;
      eventData.modifier = getKeyModifierState();
      eventData.keyCode = convertVirtualKeyCode(wParam, lParam);

      pushEvent(Event{eventData});
      break;
    }
    case WM_MOUSEWHEEL:
    {
      //The coordinate is relative to the upper-left corner of the screen. (This is different than the mouse button events.
      POINT position;
      position.x = static_cast<int32_t>(GET_X_LPARAM(lParam));
      position.y = static_cast<int32_t>(GET_Y_LPARAM(lParam));
      // Convert position from screen coordinates to window coordinates.
      ScreenToClient(m_windowHandle, &position);

      int16_t delta = static_cast<int16_t>(GET_WHEEL_DELTA_WPARAM(wParam));

      auto eventData = Event::MouseWheelData();
      eventData.wheelOrientation = Input::Mouse::Wheel::Vertical;
      eventData.deltaScroll = static_cast<float>(delta) / WHEEL_DELTA;
      eventData.modifier = getKeyModifierState();
      eventData.x = position.x;
      eventData.y = position.y;

      pushEvent(Event{eventData});
      break;
    }
    case WM_MOUSEHWHEEL:
    {
      //The coordinate is relative to the upper-left corner of the screen. (This is different than the mouse button events.
      POINT position;
      position.x = GET_X_LPARAM(lParam);
      position.y = GET_Y_LPARAM(lParam);
      // Convert position from screen coordinates to window coordinates.
      ScreenToClient(m_windowHandle, &position);

      int16_t delta = static_cast<int16_t>(GET_WHEEL_DELTA_WPARAM(wParam));

      auto eventData = Event::MouseWheelData();
      eventData.wheelOrientation = Input::Mouse::Wheel::Horizontal;
      eventData.deltaScroll = -static_cast<float>(delta) / WHEEL_DELTA;
      eventData.modifier = getKeyModifierState();
      eventData.x = position.x;
      eventData.y = position.y;

      pushEvent(Event{eventData});
      break;
    }
      // Mouse Button Down
    case WM_LBUTTONDBLCLK:
    case WM_LBUTTONDOWN:
    case WM_MBUTTONDBLCLK:
    case WM_MBUTTONDOWN:
    case WM_RBUTTONDBLCLK:
    case WM_RBUTTONDOWN:
    case WM_XBUTTONDBLCLK:
    case WM_XBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_MBUTTONUP:
    case WM_RBUTTONUP:
    case WM_XBUTTONUP:
    {
      //The coordinate is relative to the upper - left corner of the client area
      POINT cursorPos;
      cursorPos.x = GET_X_LPARAM(lParam);
      cursorPos.y = GET_Y_LPARAM(lParam);

      // convert the cursor position to screen coordinates
      ClientToScreen(m_windowHandle, &cursorPos);


      auto mouseButton = Input::Mouse::Button::Invalid;
      auto isDoubleClick = false;
      auto isMouseUp = false;
      switch (msg)
      {
        case WM_LBUTTONDBLCLK:
        {
          isDoubleClick = true;
          mouseButton = Input::Mouse::Button::Left;
          break;
        }
        case WM_LBUTTONUP:
        {
          isMouseUp = true;
          mouseButton = Input::Mouse::Button::Left;
          break;
        }
        case WM_LBUTTONDOWN:
        {
          mouseButton = Input::Mouse::Button::Left;
          break;
        }
        case WM_MBUTTONDBLCLK:
        {
          isDoubleClick = true;
          mouseButton = Input::Mouse::Button::Middle;
          break;
        }
        case WM_MBUTTONUP:
        {
          isMouseUp = true;
          mouseButton = Input::Mouse::Button::Middle;
          break;
        }
        case WM_MBUTTONDOWN:
        {
          mouseButton = Input::Mouse::Button::Middle;
          break;
        }
        case WM_RBUTTONDBLCLK:
        {
          isDoubleClick = true;
          mouseButton = Input::Mouse::Button::Right;
          break;
        }
        case WM_RBUTTONUP:
        {
          isMouseUp = true;
          mouseButton = Input::Mouse::Button::Right;
          break;
        }
        case WM_RBUTTONDOWN:
        {
          mouseButton = Input::Mouse::Button::Right;
          break;
        }
        case WM_XBUTTONDBLCLK:
        {
          isDoubleClick = true;
          mouseButton = (HIWORD(wParam) & XBUTTON1) ? Input::Mouse::Button::XButton1 : Input::Mouse::Button::XButton2;
          break;
        }
        case WM_XBUTTONUP:
        {
          isMouseUp = true;
          mouseButton = (HIWORD(wParam) & XBUTTON1) ? Input::Mouse::Button::XButton1 : Input::Mouse::Button::XButton2;
          break;
        }
        case WM_XBUTTONDOWN:
        {
          mouseButton = (HIWORD(wParam) & XBUTTON1) ? Input::Mouse::Button::XButton1 : Input::Mouse::Button::XButton2;
          break;
        }
        default:
          std::abort();
      }

      auto eventData = Event::MouseButtonData();
      eventData.button = mouseButton;
      eventData.buttonState = isMouseUp ? Event::ButtonState::Released : Event::ButtonState::Pressed;
      eventData.modifier = getKeyModifierState();
      eventData.x = cursorPos.x;
      eventData.y = cursorPos.y;

      pushEvent(Event{eventData});
      break;
    }


    case WM_MOUSELEAVE:
    {
      // Avoid this firing a second time in case the cursor is dragged outside
      if (m_mouseInside)
      {
        m_mouseInside = false;

        pushEvent(Event{Event::MouseLeaveData{}});
      }
      break;
    }
    case WM_MOUSEMOVE:
    {
      // The coordinate is relative to the upper - left corner of the client area.
      int32_t x = GET_X_LPARAM(lParam);
      int32_t y = GET_Y_LPARAM(lParam);

      RECT area;
      GetClientRect(m_windowHandle, &area);

      // Capture the mouse in case the user wants to drag it outside
      if ((wParam & (MK_LBUTTON | MK_MBUTTON | MK_RBUTTON | MK_XBUTTON1 | MK_XBUTTON2)) == 0)
      {
        // Only release the capture if we really have it
        if (GetCapture() == m_windowHandle)
        {
          ReleaseCapture();
        }
      }
      else if (GetCapture() != m_windowHandle)
      {
        // Set the capture to continue receiving mouse events
        SetCapture(m_windowHandle);
      }

      // If the cursor is outside the client area...
      if ((x < area.left) || (x > area.right) || (y < area.top) || (y > area.bottom))
      {
        // and it used to be inside, the mouse left it.
        if (m_mouseInside)
        {
          m_mouseInside = false;

          // No longer care for the mouse leaving the window
          setTracking(false);

          pushEvent(Event{Event::MouseLeaveData{}});
        }
      }
      else
      {
        // and vice-versa
        if (!m_mouseInside)
        {
          m_mouseInside = true;

          // Look for the mouse leaving the window
          setTracking(true);

          pushEvent(Event{Event::MouseEnteredData{}});
        }
      }

      auto eventData = Event::MouseMovedData();
      eventData.x = x;
      eventData.y = y;

      pushEvent(Event{eventData});
      break;
    }
    case WM_DEVICECHANGE:
    {
      //// Some sort of device change has happened, update joystick connections
      //if ((wParam == DBT_DEVICEARRIVAL) || (wParam == DBT_DEVICEREMOVECOMPLETE))
      //{
      //  // Some sort of device change has happened, update joystick connections if it is a device interface
      //  DEV_BROADCAST_HDR* deviceBroadcastHeader = reinterpret_cast<DEV_BROADCAST_HDR*>(lParam);

      //  if (deviceBroadcastHeader && (deviceBroadcastHeader->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE))
      //    JoystickImpl::updateConnections();
      //}

      break;
    }
    //case WM_PAINT:
    //{
    //  ValidateRect(hWnd, NULL);
    //}
    //  break;
    default:
    {
      result = DefWindowProc(m_windowHandle, msg, wParam, lParam);
      break;
    }
  }

  return result;
}

LRESULT windowClassProcDispatcher(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
  static thread_local std::unordered_map<HWND, Win32Window*> hwndMap = {};
  LRESULT result = 0;

  switch (msg)
  {

    case WM_NCCREATE:
    {
      auto createStruct = reinterpret_cast<CREATESTRUCT*>(lparam);
      auto window = reinterpret_cast<Win32Window*>(createStruct->lpCreateParams);
      window->m_windowHandle = hwnd;
      hwndMap.emplace(hwnd, window);
      SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)window);
      result = window->processEvent(msg, wparam, lparam);
    }
    break;
    default:
    {
      if (!hwndMap.empty())
      {
        //auto window = reinterpret_cast<Win32Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        // What is better GetWindowLongPtr of the unordered_map lookup? The map can continually grow if we continuously add windows and never remove them...
        auto pair = hwndMap.find(hwnd);
        auto window = pair->second;
        if (window)
        {
          result = window->processEvent(msg, wparam, lparam);

          if (msg == WM_DESTROY)
          {
            hwndMap.erase(hwnd);
          }

        }
      }
    }
    break;
  }

  return result;
}

} // namespace ApplicationCore::Window::Win32
