#pragma once

#include <algorithm>
#include <any>
#include <concepts>
#include <ranges>
#include <unordered_map>

namespace Core
{
// clang-format off
template <typename T>
concept Hashable = requires(T a)
{
    { std::hash<T>{}(a) } -> std::convertible_to<std::size_t>;
};

template <typename Function_T, typename ReturnType, typename... Args >
concept DelegateConcept = std::invocable<Function_T, Args...> && std::same_as<ReturnType, std::invoke_result_t<Function_T, Args...>>;
// clang-format on

class EventQueue
{

  // template <typename FunctionType>
  // struct DelegateWrapper;
  /*
  struct DelegateWrapper
  {

    auto getLambdaWrapper() {}
  };

  using DelegateWrapper_T = std::function<void(const std::any&)>;

  using MulticastDelegate_T = std::vector<DelegateWrapper_T>;
  using MulticastDelegateMap_T = std::unordered_map<std::size_t, MulticastDelegate_T>;
  using EventPair_T = std::pair<std::size_t, std::any>;

  template <typename>
  struct FunctionTraits;

  template <typename T>
  struct FunctionTraits : public FunctionTraits<decltype(&T::operator())>
  {
  };

  template <typename ReturnType, typename... Args>
  struct FunctionTraits<ReturnType(Args...)>
  {
    using FunctionSignature = ReturnType(Args...);
    using FunctionPointer = ReturnType (*)(Args...);
    using FunctionReference = ReturnType (&)(Args...);

    using ResultType = ReturnType;

    using ArgTuple = std::tuple<Args...>;
    static constexpr auto Arity = sizeof...(Args);
    template <std::size_t N>
    using NthArgType = typename std::tuple_element<N, ArgTuple>::type;
    using FirstArgType = NthArgType<0>;
    using LastArgType = NthArgType<Arity - 1>;
  };

  template <typename ReturnType, typename... Args>
  struct FunctionTraits<ReturnType (*)(Args...)> : public FunctionTraits<ReturnType(Args...)>
  {
  };

  template <typename ClassType, typename ReturnType, typename... Args>
  struct FunctionTraits<ReturnType (ClassType::*)(Args...)> : public FunctionTraits<ReturnType(Args...)>
  {
    using OwnerType = ClassType&;
  };

  template <typename ClassType, typename ReturnType, typename... Args>
  struct FunctionTraits<ReturnType (ClassType::*)(Args...) const> : public FunctionTraits<ReturnType(Args...)>
  {
    using OwnerType = const ClassType&;
  };

  template <typename ClassType, typename ReturnType, typename... Args>
  struct FunctionTraits<ReturnType (ClassType::*)(Args...) volatile> : public FunctionTraits<ReturnType(Args...)>
  {
    using OwnerType = volatile ClassType&;
  };

  template <typename ClassType, typename ReturnType, typename... Args>
  struct FunctionTraits<ReturnType (ClassType::*)(Args...) const volatile> : public FunctionTraits<ReturnType(Args...)>
  {
    using OwnerType = const volatile ClassType&;
  };

  template <typename FunctionType>
  struct FunctionTraits<std::function<FunctionType>> : public FunctionTraits<FunctionType>
  {
  };

  // template <typename ReturnType, typename ClassType>
  // struct FunctionTraits<std::mem_fn<ReturnType (ClassType::*)>> : public FunctionTraits<ReturnType (ClassType*)>
  //{
  //};

  // template <typename ReturnType, typename ClassType,, typename... Args>
  // struct FunctionTraits<std::mem_fn<ReturnType (ClassType::*)(Args...)>> : public FunctionTraits<ReturnType (ClassType*, Args...)>
  //{
  //};

  // template <typename ReturnType, typename ClassType,, typename... Args>
  // struct FunctionTraits<std::mem_fn<ReturnType (ClassType::*)(Args...) const>> : public FunctionTraits<ReturnType (const ClassType*, Args...)>
  //{
  //};

  // template <typename ReturnType, typename ClassType,, typename... Args>
  // struct FunctionTraits<std::mem_fn<ReturnType (ClassType::*)(Args...) volatile>> : public FunctionTraits<ReturnType (volatile ClassType*, Args...)>
  //{
  //};

  // template <typename ReturnType, typename ClassType,, typename... Args>
  // struct FunctionTraits<std::mem_fn<ReturnType (ClassType::*)(Args...) const volatile>> : public FunctionTraits<ReturnType (const volatile ClassType*, Args...)>
  //{
  //};

public:
  class DelegateHandleRAII
  {
  };

  void addEvent(const Hashable auto& event)
  {
    addEvent(event, event);
  }

  template <Hashable Event_T, typename EventData_T>
  void addEvent(const Event_T& eventType, const EventData_T& eventData)
  {
    std::lock_guard<std::mutex> lockGuard(m_mutex);
    m_eventList.push_back(std::make_pair(std::hash<Event_T>()(eventType), std::any(eventData)));
  }

  template <Hashable EventType, typename FunctionType>
  requires DelegateConcept<FunctionType, typename FunctionTraits<FunctionType>::ResultType, typename FunctionTraits<FunctionType>::FirstArgType> DelegateHandleRAII addListener(const EventType& eventType, const FunctionType& fct)
  {
    static_assert(FunctionTraits<FunctionType>::Arity == 1, "Support only 1 argument at the moment");
    using ReturnType = FunctionTraits<FunctionType>::ResultType;
    using ArgType = FunctionTraits<FunctionType>::FirstArgType;

    //auto delegateWrapper = [fct]() {
    //  return [delegate = fct](const std::any& anyEventData) mutable {
    //    auto* eventData = std::any_cast<std::decay_t<ArgType>>(&anyEventData);
    //    delegate(*eventData);
    //  };
    //};

     auto delegateWrapper = [delegate = fct](const std::any& anyEventData) mutable {
      auto* eventData = std::any_cast<std::decay_t<ArgType>>(&anyEventData);
      delegate(*eventData);
    };

    return addListenerImpl(eventType, delegateWrapper);
  }

  template <Hashable EventType, typename ObjectType, typename FunctionType>
  DelegateHandleRAII addListener(const EventType& eventType, ObjectType* obj, const FunctionType& fct)
  {
    static_assert(FunctionTraits<FunctionType>::Arity == 1, "Support only 1 argument at the moment");
    using ReturnType = FunctionTraits<FunctionType>::ResultType;
    using ArgType = FunctionTraits<FunctionType>::FirstArgType;

    //auto delegateWrapper = [fct, obj]() {
    //  return [delegate = fct, obj](const std::any& anyEventData) mutable {
    //    auto* eventData = std::any_cast<std::decay_t<ArgType>>(&anyEventData);
    //    std::invoke(delegate, obj, *eventData);
    //  };
    //};

     auto delegateWrapper = [delegate = fct, obj](const std::any& anyEventData) mutable {
      auto* eventData = std::any_cast<std::decay_t<ArgType>>(&anyEventData);
      std::invoke(delegate, obj, *eventData);
    };

    return addListenerImpl(eventType, delegateWrapper);
  }

  template <Hashable EventType, typename FunctionType>
  DelegateHandleRAII addListenerImpl(const EventType& eventType, const FunctionType& delegateWrapper)
  {
    const auto& hasher = std::hash<EventType>();
    std::lock_guard<std::mutex> lockGuard(m_mutex);
    auto& multicastDelegate = m_eventMulticastDelegateMap[hasher(eventType)];
    multicastDelegate.push_back(delegateWrapper);
    return DelegateHandleRAII{}; // tmp
  }

  void processEvents()
  {

    // I have to keep the mutex the whole time because if a listener would unregister we might call into a dead delegate
    std::lock_guard<std::mutex> lockGuard(m_mutex);
    for (const auto& curEvent : m_eventList)
    {
      const auto& eventKey = curEvent.first;
      const auto& eventData = curEvent.second;

      if (auto it = m_eventMulticastDelegateMap.find(eventKey); it != m_eventMulticastDelegateMap.end())
      {
        const auto& multicastDelegate = it->second;
        std::ranges::for_each(multicastDelegate, [&eventData](const auto& delegateWrapper) { delegateWrapper(eventData); });
      }
    }

    m_eventList.clear();
  }

private:
  mutable std::mutex m_mutex;
  std::vector<EventPair_T> m_eventList;
  MulticastDelegateMap_T m_eventMulticastDelegateMap;

#ifdef FRIEND_TEST
  friend class EventQueueTest;
// FRIEND_TEST(EventQueueTest, AppendFreeFunction);
#endif
*/

};

} // namespace Core