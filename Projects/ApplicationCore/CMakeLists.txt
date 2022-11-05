# file(GLOB_RECURSE sources CONFIGURE_DEPENDS "*.cpp" "*.cxx" "*.ixx")
# file(GLOB_RECURSE headers CONFIGURE_DEPENDS "*.h" "*.hpp")

# Optionally glob, but only for CMake 3.12 or later: file(GLOB HEADER_LIST
# CONFIGURE_DEPENDS "${ModernCMakeExample_SOURCE_DIR}/include/modern/*.hpp")

get_filename_component(targetName ${CMAKE_CURRENT_LIST_DIR} NAME)

if(CMAKE_SYSTEM_NAME STREQUAL Windows)

  set(platform_publicFiles 
  ${targetName}/Application/Win32/Win32Application.h
  ${targetName}/Application/Win32/Win32ApplicationDesc.h
  ${targetName}/Application/Win32/Win32ApplicationMainLoop.h
  ${targetName}/Application/Win32/Win32MessageLoop.h
  ${targetName}/Input/Win32/Win32Input.h
  ${targetName}/Window/Win32/Win32EventQueue.h
  ${targetName}/Window/Win32/Win32Window.h
  ${targetName}/Window/WinRT/WinRTWindow.h

  )

  set(platform_privateFiles 
  ${targetName}/Application/Win32/Win32Application.cpp
  ${targetName}/Application/Win32/Win32ApplicationMainLoop.cpp
  ${targetName}/Application/Win32/Win32MessageLoop.cpp
  ${targetName}/Input/Win32/Win32Input.cpp
  ${targetName}/Window/Win32/Win32EventQueue.cpp
  ${targetName}/Window/Win32/Win32Window.cpp
  ${targetName}/Window/WinRT/WinRTWindow.cpp
  )

elseif (CMAKE_SYSTEM_NAME STREQUAL Linux)

  set(platform_publicFiles 
  )

  set(platform_privateFiles 
  )

endif()

set(interfaceFiles 
)

set(publicFiles 
  ${targetName}/Application/Application.h
  ${targetName}/Application/ApplicationEventMediator.h
  ${targetName}/Input/Keyboard.h
  ${targetName}/Input/Mouse.h
  ${targetName}/Window/Event.h
  ${targetName}/Window/Window.h
  ${targetName}/Window/WindowDesc.h
  ${targetName}/Window/WindowImpl.h
  ${platform_publicFiles}
)

set(privateFiles 
  ${targetName}/Application/Application.cpp
  ${targetName}/Application/ApplicationEventMediator.cpp
  ${targetName}/Input/Keyboard.cpp
  ${targetName}/Input/Mouse.cpp
  ${targetName}/Window/Event.cpp
  ${targetName}/Window/Window.cpp
  ${targetName}/Window/WindowImpl.cpp
  ${platform_privateFiles}
)

set(testFiles 
  ${targetName}/Application/Application.test.cpp
)


set(benchmarkFiles 
)

add_library(${targetName})

target_sources(${targetName}
  PRIVATE
    ${privateFiles}
  PUBLIC
    ${publicFiles}
  INTERFACE
    ${interfaceFiles}
)

target_include_directories(${targetName}
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(${targetName}
  PRIVATE
  PUBLIC
    Core
)

if(BUILD_TESTS)
  add_test_target(
    TARGET ${targetName} 
    TESTFILES ${testFiles}
    PRIVATEFILES ${privateFiles}
    PUBLICFILES ${publicFiles}
    INTERFACEFILES ${interfaceFiles}
    LINKLIBS Core)
endif()

if(BUILD_BENCHMARKS)
  add_benchmark_target(
    TARGET ${targetName} 
    BENCHMARKFILES ${benchmarkFiles}
    PRIVATEFILES ${privateFiles}
    PUBLICFILES ${publicFiles}
    INTERFACEFILES ${interfaceFiles}
    LINKLIBS Core)
endif()