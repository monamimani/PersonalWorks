include_guard()

include(CMakePrintHelpers)

set(CMAKE_CXX_STANDARD 23 CACHE STRING "The C++ standard to use for compilation")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_CXX_STANDARD LESS 20)
  message(FATAL_ERROR "C++20 or higher is required for modules support. Current C++ standard is ${CMAKE_CXX_STANDARD}.")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# https://cmake.org/cmake/help/latest/module/GenerateExportHeader.html
# used for the function generate_export_header( someLib) that will generate a header file somelib_export.h for exporting symbol from a dynamic library.
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)

set(CMAKE_COLOR_DIAGNOSTICS ON)
set(CMAKE_COLOR_MAKEFILE ON)
