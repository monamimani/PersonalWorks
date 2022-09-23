if(sanitizers_included)
    return()
endif()
set(sanitizers_included true)

# Link this 'library' to use the sanitizers
add_library(SanitizersConfiguration INTERFACE)

option(USE_ASAN "Enable the Address Sanitizers" OFF)
if(USE_ASAN)
  message("Enable Address Sanitizer")
  if(MSVC)
    target_compile_options(SanitizersConfiguration INTERFACE
            /fsanitize=address /Oy)
    target_link_libraries(SanitizersConfiguration INTERFACE
            /fsanitize=address)
  else()
    target_compile_options(SanitizersConfiguration INTERFACE
        -fsanitize=address -fno-omit-frame-pointer)
    target_link_libraries(SanitizersConfiguration INTERFACE
            -fsanitize=address)
  endif()
endif()

option(USE_TSAN "Enable the Thread Sanitizers" OFF)
if(USE_TSAN)
  message("Enable Thread Sanitizer")
  if(MSVC)
  else()
    target_compile_options(SanitizersConfiguration INTERFACE  -fsanitize=thread)
    target_link_libraries(SanitizersConfiguration INTERFACE
            -fsanitize=thread)
  endif()
endif()

option(USE_MSAN "Enable the Memory Sanitizers" OFF)
if(USE_MSAN)
  message("Enable Memory Sanitizer")
   if(MSVC)
  else()
    target_compile_options(SanitizersConfiguration INTERFACE
            -fsanitize=memory -fno-omit-frame-pointer)
    target_link_libraries(SanitizersConfiguration INTERFACE
            -fsanitize=memory)
  endif()
endif()

option(USE_UBSAN "Enable the Undefined Behavior Sanitizers" OFF)
if(USE_UBSAN)
  message("Enable Undefined Behavior Sanitizer")
  if(MSVC)
  else()
    target_compile_options(SanitizersConfiguration INTERFACE
            -fsanitize=undefined)
    target_link_libraries(SanitizersConfiguration INTERFACE
            -fsanitize=undefined)
  endif()
endif()