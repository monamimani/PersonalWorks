#pragma once

#include <concepts>
#include <unordered_map>

import DelegateMulticast;

namespace Core
{
// clang-format off
template <typename T>
concept Hashable = requires(T a)
{
  { std::hash<T>{}(a) } -> std::convertible_to<std::size_t>;
};
// clang-format on

template <typename T, typename U>
class EventDispatcher;

template <Hashable Event_T, typename ReturnType, typename... Args>
class EventDispatcher<Event_T, ReturnType(Args...)>
{
  /*
  using MulticastDelegate_T = MulticastDelegate<ReturnType(Args...)>;
  using Map_T = std::unordered_map<Event_T, MulticastDelegate_T>;

public:
  EventDispatcher() = default;
  ~EventDispatcher()
  {
    clear();
  }

  EventDispatcher(const EventDispatcher& other)
  {
    std::lock_guard<std::mutex> lockGuard(other.m_mutex);
    m_eventDelegateMap = Map_T{other.m_eventDelegateMap};
  }

  EventDispatcher(EventDispatcher&& other)
  {
    std::lock_guard<std::mutex> lockGuard(other.m_mutex);
    m_eventDelegateMap = std::move(other.m_eventDelegateMap);
    other.m_eventDelegateMap.clear();
  }

  EventDispatcher& operator=(const EventDispatcher& other)
  {
    if (this != &other)
    {
      auto newList = other;
      swap(newList);
    }

    return *this;
  }

  EventDispatcher& operator=(EventDispatcher&& other) noexcept
  {
    if (this != &other)
    {
      std::lock_guard<std::mutex> lockGuard(other.m_mutex);
      m_eventDelegateMap = std::move(other.m_eventDelegateMap);
      other.m_eventDelegateMap.clear();
    }

    return *this;
  }

  template <std::invocable<Args...> Invokable_T>
  [[nodiscard]] auto append(Event_T eventType, const Invokable_T& delegate)
  {
    std::lock_guard<std::mutex> lockGuard(m_mutex);
    MulticastDelegate_T& multicastDelegate = m_eventDelegateMap[eventType];
    return multicastDelegate.append(delegate);
  }

  template <std::invocable<Args...> Invokable_T>
  [[nodiscard]] auto prepend(Event_T eventType, const Invokable_T& delegate)
  {
    std::lock_guard<std::mutex> lockGuard(m_mutex);
    return m_eventDelegateMap[eventType].prepend(delegate);
  }

  template <std::invocable<Args...> Invokable_T>
  [[nodiscard]] auto insertBefore(Event_T eventType, const DelegateHandleRAII<ReturnType(Args...)>& handle, const Invokable_T& delegate)
  {
    std::lock_guard<std::mutex> lockGuard(m_mutex);
    return m_eventDelegateMap[eventType].insertBefore(handle, delegate);
  }

  void remove(Event_T eventType, const DelegateHandleRAII<ReturnType(Args...)>& delegateHandle)
  {
    std::lock_guard<std::mutex> lockGuard(m_mutex);
    auto it = m_eventDelegateMap.find(eventType);
    if (it != m_eventDelegateMap.end())
    {
      it->second.remove(delegateHandle);
    }
  }

  ReturnType operator()(Event_T eventType, const Args... args) const
  {
    return Broadcast(eventType, args...);
  }

  void Broadcast(Event_T eventType, const Args... args) const
  {
    MulticastDelegate_T multicastDelegate;
    {
      std::lock_guard<std::mutex> lockGuard(m_mutex);
      auto it = m_eventDelegateMap.find(eventType);
      if (it != m_eventDelegateMap.end())
      {
        multicastDelegate = it->second;
      }
    }

    if (!multicastDelegate.isEmpty())
    {
        multicastDelegate.Broadcast(args...);
    }
  }

  void swap(EventDispatcher& other) noexcept
  {
    using std::swap;
    std::lock_guard<std::mutex> lockGuard(m_mutex);
    swap(m_eventDelegateMap, other.m_eventDelegateMap);
  }

  friend void swap(EventDispatcher& lhs, EventDispatcher& rhs) noexcept
  {
    lhs.swap(rhs);
  }

  bool isEmpty() const
  {
    std::lock_guard<std::mutex> lockGuard(m_mutex);
    return m_eventDelegateMap.empty();
  }

  void clear()
  {
    std::lock_guard<std::mutex> lockGuard(m_mutex);
    m_eventDelegateMap.clear();
  }

  explicit operator bool() const
  {
    return !isEmpty();
  }

private:
  mutable std::mutex m_mutex;
  Map_T m_eventDelegateMap;

#ifdef FRIEND_TEST
  friend class CoreEventDispatcherTest;
  // FRIEND_TEST(CoreEventDispatcherTest, AppendFreeFunction);
#endif
*/
};

} // namespace Core