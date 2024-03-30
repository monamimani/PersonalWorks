#pragma once

#include <queue>

#include "Core/NonCopyable.h"
#include "ApplicationCore/Window/Event.h"

namespace ApplicationCore::Window
{
using Pixel = uint32_t;
struct Size
{
  Size() = default;

  Size(Pixel _width, Pixel _height)
      : width{_width}
      , height{_height}
  {
  }

  bool operator==(const Size&) const = default;
  bool operator!=(const Size&) const = default;

  Pixel width = 0;
  Pixel height = 0;
};

class WindowImpl : Core::NonCopyable
{
public:
  virtual ~WindowImpl() = default;

  /// \brief Get the client size of the window
  /// \return The size of the window in pixel;
  virtual Size getSize() const = 0;

  /// \brief Grab or release the mouse cursor by preventing it from leaving the window.
  /// \param isGrabbed True grabs the cursor, False set the cursor free.
  virtual void setGrabCursor(bool isGrabbed) = 0;

  /// \brief Enable or disable automatic key-repeat
  /// \param enabled 
  virtual void setKeyRepeatedEnabled(bool enabled) = 0;

  /// \brief Show or hide the mouse cursor
  /// \param visible 
  virtual void setMouseCursorVisible(bool visible) = 0;

protected:
  WindowImpl() = default;

  /// \brief Push en event on the event queue.
  /// This function is used by derived classes to push a newly received event in the event queue.
  /// \param event Event to push.
  void pushEvent(const Event& event)
  {
    m_events.push(event);
  }

private:
  std::queue<Event> m_events; //!< Queue containing received events.
};

} // namespace ApplicationCore::Window