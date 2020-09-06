#pragma once

#include "AppCore/Input/Keyboard.h"
#include "AppCore/Input/Mouse.h"

#include <cstdint>
#include <variant>

namespace AppCore::Window
{

/// \brief Define the system events and their parameters.
class Event
{
public:
  /// @brief State of a button press, keyboard, mouse, ...
  enum class ButtonState
  {
    Invalid = -1,
    Pressed = 0,
    Released,
    Count
  };

  struct KeyModifierState
  {
    bool ctrl = false;   //!< Is the control key pressed.
    bool alt = false;    //!< Is the alt key pressed.
    bool shift = false;  //!< Is the shift key pressed.
    bool system = false; //!< Is the system key pressed.
  };

  struct KeyboardEventData
  {
    ButtonState keyState = ButtonState::Invalid;
    Input::Keyboard::Key keyCode;
    KeyModifierState modifier;
  };

  struct TextEventData
  {
    uint32_t unicodeChar = 0; //!< UTF-32 Unicode value of teh character.
  };

  /// \brief Resize event parameters
  struct ResizeEventData
  {
    uint32_t width = 0;
    uint32_t height = 0;
  };

  struct CloseEventData
  {
  };

  struct FocusEventData
  {
    bool takeFocus = false;
  };

  struct MouseWheelEventData
  {
    Input::Mouse::Wheel wheelOrientation; //!< Which wheel this event data refer to, when mouse have multiple wheels.
    float deltaScroll;                    //!< Wheel scroll offset ( positive is up/left, negative is down/right).
    KeyModifierState modifier;            //!< state of the modifiers key.
    int32_t x;                            //!< x position of the mouse pointer relative to the left of the owner window.
    int32_t y;                            //!< y position of the mouse pointer relative to the top of the owner window.
  };

  struct MouseButtonEventData
  {
    Input::Mouse::Button button;                    //!< Which mouse button the data refer to.
    ButtonState buttonState = ButtonState::Invalid; //!< The state of the button, pressed or released.
    KeyModifierState modifier;                      //!< state of the modifiers key.
    int32_t x;                                      //!< x position of the mouse pointer relative to the left of the owner window.
    int32_t y;                                      //!< y position of the mouse pointer relative to the top of the owner window.
  };

  struct MouseLeaveEventData
  {
  };

  struct MouseEnteredEventData
  {
  };

  struct MouseMovedEventData
  {
    int32_t x; //!< x position of the mouse pointer relative to the left of the owner window.
    int32_t y; //!< y position of the mouse pointer relative to the top of the owner window.
  };

  template <typename T>
  Event(const T& eventData)
      : m_eventData{eventData}
  {
  }

private:
  using EventDataVariant = std::variant<KeyboardEventData,
                                        TextEventData,
                                        ResizeEventData,
                                        CloseEventData,
                                        FocusEventData,
                                        MouseWheelEventData,
                                        MouseButtonEventData,
                                        MouseEnteredEventData,
                                        MouseLeaveEventData,
                                        MouseMovedEventData>;
  EventDataVariant m_eventData;
};
} // namespace AppCore::Window