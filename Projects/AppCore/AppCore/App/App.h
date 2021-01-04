#pragma once

#include "Core/AppFrameTimer.h"

#include <atomic>
#include <string_view>
#include <variant>

namespace AppCore::App
{
class App
{
public:

  struct AppContext
  {
    Core::AppFrameTimer::AppFrameTimeContext m_appFrameTimeContext = {};
  };

  explicit App(const std::variant<std::string_view, std::wstring_view>& applicationName);
  virtual ~App() = default;

  virtual void init() {}
  virtual int run() = 0;
  virtual void shutdown() {}

protected:
  virtual void beforeMainLoop() = 0;
  virtual void afterMainLoop() = 0;

  [[nodiscard]] std::string getApplicationName() const
  {
    return m_applicationName;
  }

  bool isRunning() const
  {
    return m_running;
  }

    Core::AppFrameTimer m_timer;

private:

  std::string m_applicationName;
  std::atomic<bool> m_running = true;


};

} // namespace AppCore::App
