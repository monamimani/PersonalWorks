#pragma once

#include <concepts>

#include "ApplicationCore/Window/Event.h"
#include "Core/NonCopyable.h"

namespace ApplicationCore
{

class EventHandlerRAII : Core::NonCopyable
{
};

class ApplicationEventMediator
{
public:
//   [[nodiscard]] Core::DelegateHandleRAII<void(int)> registerEventHandler(std::invocable<int> auto function)
//   {
//       return m_delegates.append(function);
//   }

// private:
//   Core::MulticastDelegate<void(int)> m_delegates;
  // std::vector<std::invocable> m_handlers
};

static bool foo(int)
{
  return true;
}

struct Bar
{
  int val = 52;

  void Add(int a)
  {
      val+=a;
  }
};

static void test()
{
  //ApplicationEventMediator mediator;

  //auto handle1 = mediator.registerEventHandler(&foo);
  //auto handle2 = mediator.registerEventHandler([](int) { });

  //Bar bar;
  //auto handle3 = mediator.registerEventHandler([bar](int a) { bar.val + a; });
  //auto handle4 = mediator.registerEventHandler(std::bind(&Bar::Add, bar, std::placeholders::_1));
  //auto handle5 = mediator.registerEventHandler([bar](int a) mutable { bar.Add(a); });
}

} // namespace ApplicationCore