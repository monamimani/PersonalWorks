# file(GLOB_RECURSE sources CONFIGURE_DEPENDS "*.cpp" "*.cxx" "*.ixx")
# file(GLOB_RECURSE headers CONFIGURE_DEPENDS "*.h" "*.hpp")

# Optionally glob, but only for CMake 3.12 or later: file(GLOB HEADER_LIST
# CONFIGURE_DEPENDS "${ModernCMakeExample_SOURCE_DIR}/include/modern/*.hpp")

get_filename_component(targetName ${CMAKE_CURRENT_LIST_DIR} NAME)

set(interfaceFiles 
)

set(publicFiles 
  ${targetName}/Application/Win32/Win32Application.h
  ${targetName}/Application/Win32/Win32ApplicationDesc.h
  ${targetName}/Application/Win32/Win32ApplicationMainLoop.h
  ${targetName}/Application/Win32/Win32MessageLoop.h
  ${targetName}/Application/Application.h
  ${targetName}/Application/ApplicationEventMediator.h
  ${targetName}/Input/Win32/Win32Input.h
  ${targetName}/Input/Keyboard.h
  ${targetName}/Input/Mouse.h
  ${targetName}/Window/Win32/Win32EventQueue.h
  ${targetName}/Window/Win32/Win32Window.h
  ${targetName}/Window/WinRT/WinRTWindow.h
  ${targetName}/Window/Event.h
  ${targetName}/Window/Window.h
  ${targetName}/Window/WindowDesc.h
  ${targetName}/Window/WindowImpl.h
)

set(privateFiles 
  ${targetName}/Application/Win32/Win32Application.cpp
  ${targetName}/Application/Win32/Win32ApplicationMainLoop.cpp
  ${targetName}/Application/Win32/Win32MessageLoop.cpp
  ${targetName}/Application/Application.cpp
  ${targetName}/Application/ApplicationEventMediator.cpp
  ${targetName}/Input/Win32/Win32Input.cpp
  ${targetName}/Input/Keyboard.cpp
  ${targetName}/Input/Mouse.cpp
  ${targetName}/Window/Win32/Win32EventQueue.cpp
  ${targetName}/Window/Win32/Win32Window.cpp
  ${targetName}/Window/WinRT/WinRTWindow.cpp
  ${targetName}/Window/Event.cpp
  ${targetName}/Window/Window.cpp
  ${targetName}/Window/WindowImpl.cpp
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
  set(targetNameTests ${targetName}Tests )
  add_executable(${targetNameTests} 
    ${testFiles}
  )
  set_target_properties(${targetNameTests} PROPERTIES FOLDER Tests)

  target_link_libraries(${targetNameTests}
  PRIVATE 
    ${targetName}
    GTest::gmock 
    GTest::gtest
    GTest::gtest_main
  )

  gtest_discover_tests(${targetNameTests}
    WORKING_DIRECTORY ${PROJECT_DIR}
    PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_DIR}"
    DISCOVERY_MODE PRE_TEST
  )


if(RUN_TESTS_AFTER_BUILD)
  add_custom_command(TARGET ${targetNameTests}
    # Run after all other rules within the target have been executed
    POST_BUILD
    COMMAND ${targetNameTests}
    COMMENT "Running Google Test"
    VERBATIM
  )
endif()

endif()