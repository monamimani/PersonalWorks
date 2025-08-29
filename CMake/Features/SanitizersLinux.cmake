include_guard()

# Allowed values
set(BUILD_SANITIZER_LIST "none" "address" "undefined" "thread" "memory" "leak")
set(BUILD_SANITIZERS "none" CACHE STRING "The kind of configuration for the build.")
set_property(CACHE BUILD_SANITIZERS PROPERTY STRINGS ${BUILD_SANITIZER_LIST})
set(ENV{BUILD_SANITIZERS} "${BUILD_SANITIZERS}")

# if(NOT BUILD_SANITIZERS IN_LIST BUILD_SANITIZER_LIST)
#   message(FATAL_ERROR "BUILD_SANITIZER is ${BUILD_SANITIZERS} and it must be from ${BUILD_SANITIZER_LIST}")
# endif()

string(REPLACE "," ";" SANITIZERS_LIST "${BUILD_SANITIZERS}")
foreach(SANITIZER ${BUILD_SANITIZERS})
  if(NOT SANITIZER IN_LIST BUILD_SANITIZER_LIST)
    message(FATAL_ERROR "Invalid sanitizer '${SANITIZER}' specified in BUILD_SANITIZERS. Allowed values are: ${BUILD_SANITIZER_LIST}")
  endif()
endforeach()

# Thread sanitizer is not compatible with address or leak sanitizers
if("thread" IN_LIST BUILD_SANITIZERS)
  if("address" IN_LIST BUILD_SANITIZERS OR "leak" IN_LIST BUILD_SANITIZERS)
    message(FATAL_ERROR "Thread sanitizer does not work with address or leak sanitizer enabled")
  endif()
endif()

# Memory sanitizer is not compatible with address, thread or leak sanitizers
if("memory" IN_LIST BUILD_SANITIZERS)
  message(WARNING "Memory sanitizer requires all the code (including libc++) to be MSan-instrumented otherwise it reports false positives")

  if("address" IN_LIST BUILD_SANITIZERS OR "thread" IN_LIST BUILD_SANITIZERS OR "leak" IN_LIST BUILD_SANITIZERS)
    message(FATAL_ERROR "Memory sanitizer does not work with address, ahread or leak sanitizer enabled")
  endif()
endif()

if(NOT "none" IN_LIST BUILD_SANITIZERS)
  list(JOIN BUILD_SANITIZERS "," SANITIZERS_LIST)
  string(APPEND SANITIZER_FLAGS "-fsanitize=${SANITIZERS_LIST} -g -fno-omit-frame-pointer")

  if(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
    if("undefined" IN_LIST BUILD_SANITIZERS)
    string(APPEND SANITIZER_FLAGS " -fno-sanitize-merge")
    endif()
  endif()
endif()
