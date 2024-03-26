include_guard()

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(PersonalWorks_BASIC_BUILD_MODE "Disable most of the nice to have build features." OFF)

# https://cmake.org/cmake/help/latest/module/GenerateExportHeader.html
# used for the function generate_export_header( someLib) that will generate a header file somelib_export.h for exporting symbol from a dynamic library.
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

include(FetchContent)
include(ProcessorCount)
ProcessorCount(PROCESSOR_COUNT)
include(GenerateExportHeader)
include(CMake/BuildTypes.cmake)
include(CMake/BuildOutput.cmake)
include(CMake/Targets.cmake)


set(GIT_SHA "Unknown" CACHE STRING "SHA this build was generated from")
string(SUBSTRING "${GIT_SHA}" 0 8 GIT_SHORT_SHA)

if(MSVC)
  add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/MP>)
endif()

# Enhance error reporting and compiler messages
if(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
  if(WIN32)
    # On Windows cuda nvcc uses cl and not clang
    add_compile_options($<$<COMPILE_LANGUAGE:C>:-fcolor-diagnostics> $<$<COMPILE_LANGUAGE:CXX>:-fcolor-diagnostics>)
  else()
    add_compile_options(-fcolor-diagnostics)
  endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  if(WIN32)
    # On Windows cuda nvcc uses cl and not gcc
    add_compile_options($<$<COMPILE_LANGUAGE:C>:-fdiagnostics-color=always>
      $<$<COMPILE_LANGUAGE:CXX>:-fdiagnostics-color=always>)
  else()
    add_compile_options(-fdiagnostics-color=always)
  endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC" AND MSVC_VERSION GREATER 1900)
  add_compile_options(/diagnostics:column)
else()
  message(STATUS "No colored compiler diagnostic set for '${CMAKE_CXX_COMPILER_ID}' compiler.")
endif()

if(MSVC)
  if("$ENV{VSCMD_VER}" STREQUAL "")
    message(WARNING "Not in a developper environment run cmake from a Visual Studio Developper terminal.")
  endif()
endif()
