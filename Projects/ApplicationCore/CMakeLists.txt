# file(GLOB_RECURSE sources CONFIGURE_DEPENDS "*.cpp" "*.cxx" "*.ixx")
# file(GLOB_RECURSE headers CONFIGURE_DEPENDS "*.h" "*.hpp")

# Optionally glob, but only for CMake 3.12 or later: file(GLOB HEADER_LIST
# CONFIGURE_DEPENDS "${ModernCMakeExample_SOURCE_DIR}/include/modern/*.hpp")

set(HEADERS ApplicationCore/Application/Win32/Win32Application.h
            ApplicationCore/Application/Win32/Win32ApplicationDesc.h
            ApplicationCore/Application/Win32/Win32ApplicationMainLoop.h
            ApplicationCore/Application/Win32/Win32MessageLoop.h
            ApplicationCore/Application/Application.h
            ApplicationCore/Application/ApplicationEventMediator.h
            ApplicationCore/Input/Win32/Win32Input.h
            ApplicationCore/Input/Keyboard.h
            ApplicationCore/Input/Mouse.h
            ApplicationCore/Window/Win32/Win32EventQueue.h
            ApplicationCore/Window/Win32/Win32Window.h
            ApplicationCore/Window/WinRT/WinRTWindow.h
            ApplicationCore/Window/Event.h
            ApplicationCore/Window/Window.h
            ApplicationCore/Window/WindowDesc.h
            ApplicationCore/Window/WindowImpl.h
    )

set(SOURCES ApplicationCore/Application/Win32/Win32Application.cpp
            ApplicationCore/Application/Win32/Win32ApplicationMainLoop.cpp
            ApplicationCore/Application/Win32/Win32MessageLoop.cpp
            ApplicationCore/Application/Application.cpp
            ApplicationCore/Application/ApplicationEventMediator.cpp
            ApplicationCore/Input/Win32/Win32Input.cpp
            ApplicationCore/Input/Keyboard.cpp
            ApplicationCore/Input/Mouse.cpp
            ApplicationCore/Window/Win32/Win32EventQueue.cpp
            ApplicationCore/Window/Win32/Win32Window.cpp
            ApplicationCore/Window/WinRT/WinRTWindow.cpp
            ApplicationCore/Window/Event.cpp
            ApplicationCore/Window/Window.cpp
            ApplicationCore/Window/WindowImpl.cpp

    )

set(TESTS_SOURCES ApplicationCore/Application/Application.test.cpp)

add_library(ApplicationCore)

target_sources(ApplicationCore
  PRIVATE
    ${SOURCES}
    ${TESTS_SOURCES}
  PUBLIC
    ${HEADERS}
)

target_include_directories(ApplicationCore
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(ApplicationCore
    PUBLIC
        Core
    PRIVATE 
        GTest::gmock 
        GTest::gtest
        benchmark::benchmark
)

#source_group(
#  TREE "${PROJECT_SOURCE_DIR}/include"
#  PREFIX "Header Files"
#  FILES ${HEADER_LIST})

# enable_testing()
# find_package(GTest REQUIRED)

if(BUILD_TESTING)
    gtest_discover_tests(App)
endif()