#pragma once

#include <system_error>

#include "ApplicationCore/Application/Application.h"
#include "ApplicationCore/Application/Win32/Win32MessageLoop.h"

namespace ApplicationCore::Win32
{
class Win32ApplicationMainLoop
{
public:
  // static int runMainLoop(Application& application, Win32MessageLoop& messageLoop)
  template <ApplicationC ApplicationT>
  static int runMainLoop(ApplicationT& application, Win32MessageLoop& messageLoop)
  {
    int32_t returnValue = EXIT_SUCCESS;
    if (application.initialize())
    {
      bool isContinue = true;

      // std::tie(isContinue, returnValue) = messageLoop.pump();
      // for (; isContinue; std::tie(isContinue, returnValue) = messageLoop.pump())
      //{
      //  application.update();
      //}

        //std::get<0> return the value of isContinue in the tuple returned by std::tie
      while (std::get<0>(std::tie(isContinue, returnValue) = messageLoop.pump()))
      {
        application.update();
      }
    }
    else
    {
      ::PostQuitMessage(-1);
    }

    application.shutdown();

    return returnValue;
  }

private:
};
} // namespace ApplicationCore::Win32
