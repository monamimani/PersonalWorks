// clang-format off
// Cheeky but need to be before the headers so that classes can use FRIEND_TEST without including gtest_prod.h
// and that the whole universe depends on gtest.
#include "gtest/gtest.h"
// clang-format on

#include "Core/DelegateTypes/EventQueue.h"

namespace Core
{
constexpr uint32_t TEST_INT = 42;

enum class EventTypes
{
  EventTestA,
  EventTestB,
};

struct EventTestA
{
  uint32_t value = TEST_INT;
};

struct EventTestB
{
  uint32_t value = 151;
};

void foo([[maybe_unused]] const EventTestA& eventData)
{
}

struct Bar
{
  int value[128] = {};

  void barFct([[maybe_unused]]const EventTestB& eventData)
  {
  }

  void barFctConst([[maybe_unused]]const EventTestB& eventData) const
  {
  }
};

constexpr auto lambdaTest = [](const EventTestA&) {};
} // namespace Core

namespace std
{

// template <>
// struct hash<Core::EventTypes>
//{
//  size_t operator()(const Core::EventTypes& eventTypes) const
//  {
//    return static_cast<std::size_t>(eventTypes);
//  }
//};

template <>
struct hash<Core::EventTestA>
{
  size_t operator()(const Core::EventTestA&) const
  {
    return hash<Core::EventTypes>()(Core::EventTypes::EventTestA);
  }
};

template <>
struct hash<Core::EventTestB>
{
  size_t operator()(const Core::EventTestB&) const
  {
    return hash<Core::EventTypes>()(Core::EventTypes::EventTestB);
  }
};
} // namespace std

namespace Core
{

//class EventQueueTest : public ::testing::Test
//{
//protected:
//  using EventQueueType = Core::EventQueue;
//
//  void SetUp() override {}
//
//  void TearDown() override {}
//
//  static const auto& getEventList(const EventQueueType& eventQueue)
//  {
//    return eventQueue.m_eventList;
//  }
//
//  static const auto& getEventMulticastDelegateMap(const EventQueueType& eventQueue)
//  {
//    return eventQueue.m_eventMulticastDelegateMap;
//  }
//
//  void onEventTestA(const EventTestA&)
//  {
//      eventTestAProcessed =true;
//  }
//
//  EventQueueType eventQueue;
//
//  bool eventTestAProcessed = false;
//
//  FRIEND_TEST(EventQueueTest, processEvents);
//};
//
//class EventQueueTestMultithreadTest : public EventQueueTest
//{
//};
//
//TEST_F(EventQueueTest, DefaultConstructorDestructor)
//{
//  ASSERT_EQ(getEventList(eventQueue).size(), 0);
//  ASSERT_EQ(getEventMulticastDelegateMap(eventQueue).size(), 0);
//}
//TEST_F(EventQueueTest, CopyConstructorDestructor)
//{
//  // auto handleRAII = eventDispatcher.append(Event_Types::TestEventKind, &foo);
//  // auto eventDispatcherCopy{eventDispatcher};
//  // ASSERT_FALSE(eventDispatcherCopy.isEmpty());
//  // ASSERT_EQ(getEventMap(eventDispatcherCopy).size(), 1);
//  // ASSERT_FALSE(eventDispatcher.isEmpty());
//  // ASSERT_EQ(getEventMap(eventDispatcher).size(), 1);
//}
//TEST_F(EventQueueTest, MoveConstructorDestructor)
//{
//  // auto handleRAII = eventDispatcher.append(Event_Types::TestEventKind, &foo);
//  // auto eventDispatcherMoved{std::move(eventDispatcher)};
//  // ASSERT_FALSE(eventDispatcherMoved.isEmpty());
//  // ASSERT_EQ(getEventMap(eventDispatcherMoved).size(), 1);
//  // ASSERT_TRUE(eventDispatcher.isEmpty());
//  // ASSERT_EQ(getEventMap(eventDispatcher).size(), 0);
//}
//
//
//TEST_F(EventQueueTest, addEvent)
//{
//    eventQueue.addEvent(EventTypes::EventTestA, EventTestA{});
//    ASSERT_EQ(getEventList(eventQueue).size(), 1);
//    eventQueue.addEvent(EventTypes::EventTestB, EventTestB{});
//    ASSERT_EQ(getEventList(eventQueue).size(), 2);
//}
//
//
//
//TEST_F(EventQueueTest, addListener)
//{
//  eventQueue.addListener(EventTypes::EventTestA, &foo);
//  ASSERT_EQ(getEventList(eventQueue).size(), 0);
//  ASSERT_EQ(getEventMulticastDelegateMap(eventQueue).size(), 1);
//
//  eventQueue.addListener(EventTypes::EventTestB, []([[maybe_unused]] const EventTestB& eventData) { });
//   ASSERT_EQ(getEventList(eventQueue).size(), 0);
//   ASSERT_EQ(getEventMulticastDelegateMap(eventQueue).size(), 2);
//
//  eventQueue.addListener(EventTypes::EventTestB, [bar = Bar()](const EventTestB& eventData) mutable { bar.barFct(eventData);});
//  ASSERT_EQ(getEventList(eventQueue).size(), 0);
//  //ASSERT_EQ(getEventMulticastDelegateMap(eventQueue).size(), 3);
//
//  eventQueue.addListener(EventTypes::EventTestB, [bar = Bar()](const EventTestB& eventData) { bar.barFctConst(eventData);});
//  //ASSERT_EQ(getEventMulticastDelegateMap(eventQueue).size(), 4);
//
//  auto bar = Bar();
//  eventQueue.addListener(EventTypes::EventTestB, &bar, &Bar::barFctConst);
//  //ASSERT_EQ(getEventMulticastDelegateMap(eventQueue).size(), 5);
//
//  eventQueue.addEvent(EventTypes::EventTestA, EventTestA{});
//  eventQueue.addEvent(EventTypes::EventTestB, EventTestB{});
//  eventQueue.processEvents();
//
//  // auto handleRAII = eventDispatcher.append(Event_Types::TestEventKind, &foo);
//  // auto eventDispatcherMoved{std::move(eventDispatcher)};
//  // ASSERT_FALSE(eventDispatcherMoved.isEmpty());
//  // ASSERT_EQ(getEventMap(eventDispatcherMoved).size(), 1);
//  // ASSERT_TRUE(eventDispatcher.isEmpty());
//  // ASSERT_EQ(getEventMap(eventDispatcher).size(), 0);
//}
//
//TEST_F(EventQueueTest, processEvents)
//{
//    eventQueue.addListener(EventTypes::EventTestA, this, &EventQueueTest::onEventTestA);
//    ASSERT_EQ(getEventMulticastDelegateMap(eventQueue).size(), 1);
//
//    eventQueue.addEvent(EventTypes::EventTestA, EventTestA{});
//    ASSERT_EQ(getEventList(eventQueue).size(), 1);
//
//    ASSERT_EQ(eventTestAProcessed, false);
//    eventQueue.processEvents();
//    ASSERT_EQ(getEventList(eventQueue).size(), 0);
//
//    ASSERT_EQ(eventTestAProcessed, true);
//}

} // namespace Core