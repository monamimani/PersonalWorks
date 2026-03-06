include_guard()

function(add_target_sanitizers TARGET_NAME)

  if(NOT TARGET ${TARGET_NAME})
    message(FATAL_ERROR "Target ${TARGET_NAME} does not exist. Please create the target before calling this function.")
  endif()

  set(SANITIZER_FLAGS_LIST "")

  if(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang|GNU")
    include(CMake/Features/SanitizersLinux.cmake)
    get_sanitizers_flags_linux(SANITIZER_FLAGS_LIST)
  endif()

  if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    include(CMake/Features/SanitizersMsvc.cmake)
    get_sanitizers_flags_msvc(SANITIZER_FLAGS_LIST)
  endif()

  #cmake_print_variables(SANITIZER_FLAGS_LIST)

  if(NOT SANITIZER_FLAGS_LIST STREQUAL "")
    target_compile_options(${TARGET_NAME} INTERFACE ${SANITIZER_FLAGS_LIST})
  endif()

endfunction()
