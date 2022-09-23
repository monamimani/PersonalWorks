#pragma once

#include <atomic>
#include <memory>
#include <string_view>
#include <variant>

#include "Core/AppFrameTimer.h"
#include "Core/NonCopyable.h"

#include "ApplicationCore/Application/ApplicationEventMediator.h"

namespace ApplicationCore
{

// clang-format off
template <typename Application_T>
concept Application_C = requires(Application_T& app)
{
  {app.initialize()} -> std::convertible_to<bool>;
  {app.update()} -> std::convertible_to<void>;
  {app.shutdown()} -> std::convertible_to<void>;
};
// clang-format on


class Application : Core::NonCopyable
{
public:
  struct AppContext
  {
    Core::AppFrameTimer::AppFrameTimeContext m_appFrameTimeContext = {};
  };

  explicit Application(const std::variant<std::string_view, std::wstring_view>& applicationName);
  virtual ~Application() = default;

  virtual bool initialize() { return true;}
  virtual void update() = 0;
  virtual void shutdown() {}

  template <typename T, typename... Args>
  static auto CreateApplication(Args&&... args)
  {
    return std::make_unique<T>(std::forward<Args>(args)...);
  }

protected:
  [[nodiscard]] std::string getApplicationName() const { return m_applicationName; }

  bool isRunning() const { return m_running; }

  void setRunning(bool running) { m_running = running; }

  Core::AppFrameTimer m_timer;

private:
  std::string m_applicationName;
  std::atomic<bool> m_running = true;

  ApplicationEventMediator m_appEventMediator;
};

} // namespace ApplicationCore
