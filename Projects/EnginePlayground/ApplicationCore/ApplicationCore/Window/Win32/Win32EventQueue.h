#pragma once

#include <queue>

#include "ApplicationCore/Window/Event.h"

namespace ApplicationCore::Window::Win32
{

class Win32EventQueue
{

  void pushEvent(const Event& event) { eventQueue.push(event); }

private:
  std::queue<Event> eventQueue;
};

} // namespace ApplicationCore::Window::Win32