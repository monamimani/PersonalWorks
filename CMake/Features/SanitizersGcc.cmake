include_guard()

option(BUILD_SANITIZER_ADDRESS "Enable Address Sanitizer" OFF)
option(BUILD_SANITIZER_UNDEFINED "Enable Undefined Behavior Sanitizer" OFF)
option(BUILD_SANITIZER_THREAD "Enable Thread Sanitizer" OFF)
option(BUILD_SANITIZER_MEMORY "Enable Memory Sanitizer" OFF)
option(BUILD_SANITIZER_LEAK "Enable Leak Sanitizer" OFF)

set(SANITIZERS "")

if(BUILD_SANITIZER_ADDRESS)
  list(APPEND SANITIZERS "address")
endif()

if(BUILD_SANITIZER_UNDEFINED)
  list(APPEND SANITIZERS "undefined")
endif()

if(BUILD_SANITIZER_THREAD)
  list(APPEND SANITIZERS "thread")
endif()

if(BUILD_SANITIZER_MEMORY)
  list(APPEND SANITIZERS "memory")
endif()

if(BUILD_SANITIZER_LEAK)
  list(APPEND SANITIZERS "leak")
endif()

if(SANITIZERS)
  list(JOIN SANITIZERS "," SANITIZERS_LIST)
  set(SANITIZER_FLAGS "-fsanitize=${SANITIZERS_LIST}")
endif()
