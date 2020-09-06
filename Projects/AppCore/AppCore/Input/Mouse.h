#pragma once

namespace AppCore::Input
{
/// \brief Mouse realtime states
class Mouse
{
public:
  enum class Button
  {
    Left,     //!< The left mouse button
    Right,    //!< The right mouse button
    Middle,   //!< The middle (wheel) mouse button
    XButton1, //!< The first extra mouse button
    XButton2, //!< The second extra mouse button
    Count
  };

  enum class Wheel
  {
    Vertical,  //!< The vertical mouse wheel
    Horizontal //!< The horizontal mouse wheel
  };

private:
};
} // namespace AppCore::Input