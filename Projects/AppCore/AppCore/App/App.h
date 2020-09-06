#pragma once

//#include "Utility/Timer.h"

class StepTimer;

#include <string_view>
#include <variant>
#include <atomic>

namespace AppCore::App
{
class App
{
public:
  explicit App(const std::variant<std::string_view, std::wstring_view>& applicationName);
  virtual ~App() = default;

  virtual int run() = 0;


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

private:
  virtual void update(const StepTimer& timer) = 0;
  virtual void render(const StepTimer& timer) = 0;

  std::string m_applicationName;
  std::atomic<bool> m_running = true;

  //StepTimer m_timer;
};

} // namespace AppCore::App
