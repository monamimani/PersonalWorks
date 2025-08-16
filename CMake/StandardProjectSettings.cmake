include_guard()

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Organize unrelated targets to clean IDE hierarchy.
set(DEPENDENCY_FOLDER "Thirdparty")

# This will also clean up the CMake ALL_BUILD, INSTALL, RUN_TESTS and ZERO_CHECK projects.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER ${DEPENDENCY_FOLDER})

# https://cmake.org/cmake/help/latest/module/GenerateExportHeader.html
# used for the function generate_export_header( someLib) that will generate a header file somelib_export.h for exporting symbol from a dynamic library.
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)

include(FetchContent)
include(ProcessorCount)
ProcessorCount(PROCESSOR_COUNT)
include(GenerateExportHeader)
include(CMake/Utilities.cmake)
include(CMake/BuildTypes.cmake)
include(CMake/BuildOutput.cmake)
include(CMake/Targets.cmake)

set(GIT_SHA "Unknown" CACHE STRING "SHA this build was generated from")
string(SUBSTRING "${GIT_SHA}" 0 8 GIT_SHORT_SHA)

if(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
  add_compile_options(-std=c++26)
  #add_compile_options(-D__cpp_concepts=202002)
  add_compile_options(-Wno-builtin-macro-redefined)
  link_libraries(stdc++exp) # for std::stacktrace
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  add_compile_options("-std=c++26")
  link_libraries(stdc++exp) # for std::stacktrace
endif()

if(MSVC)
  add_compile_options("/std:c++latest")
  #add_compile_options("/dynamicdeopt")
  add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/MP>)
  add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/utf-8>)
endif()

set(CMAKE_COLOR_DIAGNOSTICS ON)
set(CMAKE_COLOR_MAKEFILE ON)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC" AND MSVC_VERSION GREATER 1900)
  #add_compile_options(/diagnostics:column)
  add_compile_options(/diagnostics:caret)
endif()

if(MSVC)
  if("$ENV{VSCMD_VER}" STREQUAL "")
    message(WARNING "Not in a developper environment run cmake from a Visual Studio Developper terminal.")
  endif()
endif()
