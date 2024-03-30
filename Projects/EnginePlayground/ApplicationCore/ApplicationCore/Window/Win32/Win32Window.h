#pragma once

#include <memory>

#include "ApplicationCore/Input/Keyboard.h"
#include "ApplicationCore/Application/Win32/Win32ApplicationDesc.h"
#include "ApplicationCore/Window/WindowDesc.h"
#include "ApplicationCore/Window/WindowImpl.h"
#include "Core/Win32/WindowsHeader.h"

namespace ApplicationCore::Window::Win32
{
struct BasicWin32WindowClass;

class Win32Window : public WindowImpl
{
public:
  explicit Win32Window(const WindowDesc& desc);
  ~Win32Window() override;

  void setGrabCursor(bool isGrabbed) override;
  void setKeyRepeatedEnabled(bool enabled) override;
  void setMouseCursorVisible(bool visible) override;

private:
  friend LRESULT CALLBACK windowClassProcDispatcher(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
  LRESULT CALLBACK processEvent(UINT msg, WPARAM wParam, LPARAM lParam);

  void cleanup();

  Size getSize() const override;

  /// \brief Convert a Win32 virtual key to a Keyboard key code.
  /// \param key The virutalkey to convert
  /// \param flags Additionnal flags
  /// \return
  Input::Keyboard::Key convertVirtualKeyCode(WPARAM key, LPARAM flags);

  /// \brief Grap or release the mouse cursor
  /// This function grab the cursor, limit the cursor to the window.
  /// \param isGrabbed True to grab and False to release the cursor
  void execGrabCursor(bool isGrabbed);

  /// \brief Enables or disables the tracking of the mouse cursor leaving the window.
  /// \param enabled
  void setTracking(bool enabled);

  WindowDesc m_desc;
  ApplicationCore::Win32::Win32ApplicationDesc m_nativeDesc;

  HWND m_windowHandle = {};
  std::shared_ptr<BasicWin32WindowClass> m_windowClass;

  bool m_keyRepeatEnabled = true; //!< Key-repeat state for keydown evets.
  bool m_cursorGrabbed = false;   //!< Is the mouse cursor grabbed?
  bool m_cursorVisible = true;    //!< Is the cursor visible?
  HCURSOR m_lastCursor;           //!< Last cursor used -- this data is not owned by the window and is required to be always valid
  bool m_resizing = false;        //!< Is a resize operation ongoing?
  bool m_mouseInside = false;     //!< Isthe mouse inside the window?
  uint16_t m_surrogate = 0;       //!< First half of the surrogate pair, in case we're receiving a Unicode character in two events.
  Size m_previousSize;            //< The previous window size handled.
};

} // namespace ApplicationCore::Window::Win32