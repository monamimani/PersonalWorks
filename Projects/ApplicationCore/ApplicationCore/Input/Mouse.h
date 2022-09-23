#pragma once

namespace ApplicationCore::Input
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
    Count,
    Invalid
  };

  enum class Wheel
  {
    Vertical,  //!< The vertical mouse wheel
    Horizontal //!< The horizontal mouse wheel
  };

  struct Position
  {
    int x = 0;
    int y = 0;
  };

private:
};
} // namespace ApplicationCore::Input