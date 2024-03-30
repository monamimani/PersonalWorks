
#include "TestUtilities/GoogleTest.h"

#include "Core/DelegateTypes/EventDispatcher.h"

namespace Core
{
    constexpr uint32_t TEST_INT = 42;

enum class EventTypes
{
  TestEventKind,
};

struct EventTest
{
    uint32_t value = TEST_INT;
};

void foo([[maybe_unused]] const EventTest& eventData) {}

struct Bar
{
  int value[128] = {};
};

constexpr auto lambdaTest = [](const EventTest&) {};

//class CoreEventDispatcherTest : public ::testing::Test
//{
//protected:
//  using EventDispatcherType = Core::EventDispatcher<EventTypes, void(const EventTest&)>;
//
//  void SetUp() override {}
//
//  void TearDown() override {}
//
//  static const auto& getEventMap(const EventDispatcherType& dispatcher)
//  {
//    return dispatcher.m_eventDelegateMap;
//  }
//
//  EventDispatcherType m_eventDispatcher;
//};
//
//TEST_F(CoreEventDispatcherTest, DefaultConstructorDestructor)
//{
//
//  ASSERT_TRUE(m_eventDispatcher.isEmpty());
//  ASSERT_EQ(getEventMap(eventDispatcher).size(), 0);
//}

//TEST_F(CoreEventDispatcherTest, CopyConstructorDestructor)
//{
//  auto handleRAII = eventDispatcher.append(Event_Types::TestEventKind, &foo);
//
//  auto eventDispatcherCopy{eventDispatcher};
//  ASSERT_FALSE(eventDispatcherCopy.isEmpty());
//  ASSERT_EQ(getEventMap(eventDispatcherCopy).size(), 1);
//
//  ASSERT_FALSE(eventDispatcher.isEmpty());
//  ASSERT_EQ(getEventMap(eventDispatcher).size(), 1);
//}
//
//TEST_F(CoreEventDispatcherTest, MoveConstructorDestructor)
//{
//
//  auto handleRAII = eventDispatcher.append(Event_Types::TestEventKind, &foo);
//
//  auto eventDispatcherMoved{std::move(eventDispatcher)};
//  ASSERT_FALSE(eventDispatcherMoved.isEmpty());
//  ASSERT_EQ(getEventMap(eventDispatcherMoved).size(), 1);
//
//  ASSERT_TRUE(eventDispatcher.isEmpty());
//  ASSERT_EQ(getEventMap(eventDispatcher).size(), 0);
//}
//
//TEST_F(CoreEventDispatcherTest, AppendFreeFunction)
//{
//  auto handleRAII = eventDispatcher.append(Event_Types::TestEventKind, &foo);
//
//  ASSERT_FALSE(eventDispatcher.isEmpty());
//  ASSERT_EQ(getEventMap(eventDispatcher).size(), 1);
//}
//
//TEST_F(CoreEventDispatcherTest, AppendLambda)
//{
//  auto handleRAII = eventDispatcher.append(Event_Types::TestEventKind, [](const EventTest&) {});
//
//  ASSERT_EQ(getEventMap(eventDispatcher).size(), 1);
//  ASSERT_FALSE(eventDispatcher.isEmpty());
//}
//
//TEST_F(CoreEventDispatcherTest, AppendLambda1)
//{
//
//  Bar bar;
//  auto handleRAII = eventDispatcher.append(Event_Types::TestEventKind, [bar](const EventTest&) {});
//  ASSERT_FALSE(eventDispatcher.isEmpty());
//  ASSERT_EQ(getEventMap(eventDispatcher).size(), 1);
//}
//
//TEST_F(CoreEventDispatcherTest, Prepend)
//{
//  auto handleRAII = eventDispatcher.prepend(Event_Types::TestEventKind, &foo);
//  ASSERT_FALSE(eventDispatcher.isEmpty());
//  ASSERT_EQ(getEventMap(eventDispatcher).size(), 1);
//}
//
//TEST_F(CoreEventDispatcherTest, InsertBefore)
//{
//  Core::MulticastDelegate<void()> delegateList;
//
//  auto handleRAII = eventDispatcher.append(Event_Types::TestEventKind, &foo);
//
//  auto handleRAII2 = eventDispatcher.insertBefore(Event_Types::TestEventKind, handleRAII, lambdaTest);
//
//  ASSERT_FALSE(eventDispatcher.isEmpty());
//  ASSERT_EQ(getEventMap(eventDispatcher).size(), 1);
//}
//
//TEST_F(CoreEventDispatcherTest, Remove)
//{
//  auto handleRAII = eventDispatcher.append(Event_Types::TestEventKind, &foo);
//  eventDispatcher.remove(Event_Types::TestEventKind, handleRAII);
//
//  const auto& multicastDelegate = getEventMap(eventDispatcher).at(Event_Types::TestEventKind);
//  ASSERT_TRUE(multicastDelegate.isEmpty());
//}
//
//TEST_F(CoreEventDispatcherTest, Swap)
//{
//  auto handleRAII = eventDispatcher.append(Event_Types::TestEventKind, &foo);
//
//  EventDispatcherType eventDispatcherB;
//
//  swap(eventDispatcher, eventDispatcherB);
//  ASSERT_TRUE(eventDispatcher.isEmpty());
//  ASSERT_EQ(getEventMap(eventDispatcher).size(), 0);
//  ASSERT_FALSE(eventDispatcherB.isEmpty());
//  ASSERT_EQ(getEventMap(eventDispatcherB).size(), 1);
//}
//
//TEST_F(CoreEventDispatcherTest, Broadcast)
//{
//    auto testResult = testing::AssertionFailure();
//    auto handleRAII = eventDispatcher.append(Event_Types::TestEventKind, [&testResult](const EventTest& eventData) {
//        ASSERT_EQ(eventData.value, TEST_INT);
//        testResult = testing::AssertionSuccess();
//        });
//
//    eventDispatcher.Broadcast(Event_Types::TestEventKind, EventTest{});
//
//    ASSERT_TRUE(testResult);
//}
//
//TEST_F(CoreEventDispatcherTest, FunctionCallOperator)
//{
//    auto testResult = testing::AssertionFailure();
//    auto handleRAII = eventDispatcher.append(Event_Types::TestEventKind, [&testResult](const EventTest&) { testResult = testing::AssertionSuccess(); });
//
//    eventDispatcher(Event_Types::TestEventKind, EventTest{});
//
//    ASSERT_TRUE(testResult);
//}

} // namespace Core