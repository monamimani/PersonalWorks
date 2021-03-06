#pragma once

#include <chrono>
#include <cstdint>
#include <ratio>

namespace Core
{
class AppFrameTimer
{
public:

   struct AppFrameTimeContext
  {
    std::chrono::nanoseconds m_deltaTime = {};
    uint64_t m_frameCount = 0;
  };

  AppFrameTimer() = default;

  auto getDeltaTime() const
  {
    return m_deltaTime;
  }

  // Get elapsed time since the previous Update call.
  double getDeltaTimeInSeconds() const
  {
    return std::chrono::duration<double>(m_deltaTime).count();
  }

  // Get total time since the start of the program.
  double getTotalSeconds() const
  {
    return std::chrono::duration<double>(m_totalTime).count();
  }

  // Get total number of updates since start of the program.
  uint64_t getFrameCount() const
  {
    return m_frameCount;
  }

  // Get the current framerate.
  uint32_t getFramesPerSecond() const
  {
    return m_framesPerSecond;
  }

  // Set whether to use fixed or variable timestep mode.
  void setFixedTimeStep(bool isFixedTimestep)
  {
    m_isFixedTimeStep = isFixedTimestep;
  }

  // Set how often to call Update when in fixed timestep mode.
  void setTargetDeltaTimeInSeconds(std::chrono::duration<double> targetDeltaTime)
  {
    m_targetDeltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(targetDeltaTime);
  }

  // After an intentional timing discontinuity (for instance a blocking IO operation)
  // call this to avoid having the fixed timestep logic attempt a set of catch-up
  // Update calls.
  void reset()
  {
    m_lastTime = std::chrono::high_resolution_clock::now();
    m_leftOverTime = std::chrono::nanoseconds{};
    m_framesPerSecond = 0;
    m_framesThisSecond = 0;
    m_secondsCounter = std::chrono::nanoseconds{};
  }

  AppFrameTimeContext getFrameTimeContext()
  {
    return {m_deltaTime, m_frameCount};
  }

  // Update m_timer state, calling the specified Update function the appropriate number of times.
  template <typename TUpdate>
  void tick(const TUpdate& update)
  {
    // Query the current time.
    auto currentTime = std::chrono::high_resolution_clock::now();

    auto timeDelta = currentTime - m_lastTime;

    m_lastTime = currentTime;
    m_secondsCounter += timeDelta;

    // Clamp excessively large time deltas (e.g. after paused in the debugger).
    if (timeDelta > m_maxDelta)
    {
      timeDelta = m_maxDelta;
    }

    uint64_t lastFrameCount = m_frameCount;

    if (m_isFixedTimeStep)
    {
      // Fixed timestep update logic

      // If the app is running very close to the target elapsed time (within 1/4 of a millisecond) just clamp
      // the clock to exactly match the target value. This prevents tiny and irrelevant errors
      // from accumulating over time. Without this clamping, a game that requested a 60 fps
      // fixed update, running with vsync enabled on a 59.94 NTSC display, would eventually
      // accumulate enough tiny errors that it would drop a frame. It is better to just round
      // small deviations down to zero to leave things running smoothly.
      if (std::chrono::abs(timeDelta - m_targetDeltaTime) < std::chrono::nanoseconds{250000})
      {
        timeDelta = m_targetDeltaTime;
      }

      m_leftOverTime += timeDelta;

      while (m_leftOverTime >= m_targetDeltaTime)
      {
        m_deltaTime = m_targetDeltaTime;
        m_totalTime += m_targetDeltaTime;
        m_leftOverTime -= m_targetDeltaTime;
        m_frameCount++;

        update();
      }
    }
    else
    {
      // Variable timestep update logic.
      m_deltaTime = timeDelta;
      m_totalTime += timeDelta;
      m_leftOverTime = std::chrono::nanoseconds{};
      m_frameCount++;

      update();
    }

    // Track the current framerate.
    if (m_frameCount != lastFrameCount)
    {
      m_framesThisSecond++;
    }

    if (m_secondsCounter >= std::chrono::seconds{1})
    {
      m_framesPerSecond = m_framesThisSecond;
      m_framesThisSecond = 0;
      m_secondsCounter %= std::chrono::seconds{1};
    }
  }

private:
  std::chrono::high_resolution_clock::time_point m_lastTime = {};
  std::chrono::nanoseconds m_maxDelta = std::chrono::high_resolution_clock::duration(100000000); //< Initialize max delta to 1/10 of a second.

  std::chrono::nanoseconds m_deltaTime = {};
  std::chrono::nanoseconds m_totalTime = {};
  std::chrono::nanoseconds m_leftOverTime = {};

  // Members for tracking the framerate.
  uint64_t m_frameCount = 0;
  uint32_t m_framesPerSecond = 0;
  uint32_t m_framesThisSecond = 0;
  std::chrono::nanoseconds m_secondsCounter = {};

  // Members for configuring fixed timestep mode.
  bool m_isFixedTimeStep = false;
  std::chrono::nanoseconds m_targetDeltaTime = std::chrono::seconds(1) / 60;
};
} // namespace Core