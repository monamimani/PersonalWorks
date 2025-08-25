include_guard()

set(SANITIZERS "")

if(BUILD_SANITIZER_ADDRESS)
  list(APPEND SANITIZERS "address")
endif()

if(BUILD_SANITIZER_LEAK)
  list(APPEND SANITIZERS "leak")
endif()

if(BUILD_SANITIZER_UNDEFINED)
  list(APPEND SANITIZERS "undefined")
endif()

if(BUILD_SANITIZER_THREAD)
  if("address" IN_LIST SANITIZERS OR "leak" IN_LIST SANITIZERS)
    message(WARNING "Thread sanitizer does not work with Address and Leak sanitizer enabled")
  else()
    list(APPEND SANITIZERS "thread")
  endif()
endif()

if(BUILD_SANITIZER_MEMORY)
  message(WARNING
    "Memory sanitizer requires all the code (including libc++) to be MSan-instrumented otherwise it reports false positives"
  )

  if("address" IN_LIST SANITIZERS OR "thread" IN_LIST SANITIZERS OR "leak" IN_LIST SANITIZERS)
    message(WARNING "Memory sanitizer does not work with Address, Thread or Leak sanitizer enabled")
  else()
    list(APPEND SANITIZERS "memory")
  endif()
endif()

if(SANITIZERS)
  list(JOIN SANITIZERS "," LIST_OF_SANITIZERS)

  string(APPEND DEFAULT_C_FLAGS " -fsanitize=${LIST_OF_SANITIZERS}")
  string(APPEND DEFAULT_CXX_FLAGS " -fsanitize=${LIST_OF_SANITIZERS}")
  string(APPEND DEFAULT_SHARED_LINKER_FLAGS " -fsanitize=${LIST_OF_SANITIZERS}")
  string(APPEND DEFAULT_EXE_LINKER_FLAGS " -fsanitize=${LIST_OF_SANITIZERS}")
endif()
