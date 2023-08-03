cmake_minimum_required(VERSION 3.24)

set(VCPKG_INSTALLED_DIR  ${CMAKE_BINARY_DIR}/../vcpkg_installed)

#set(CMAKE_VERBOSE_MAKEFILE ON)

include(CMakePrintHelpers)
# cmake_print_variables(CMAKE_MODULE_PATH)

get_filename_component(WorkspaceName ${CMAKE_SOURCE_DIR} NAME)
string(REPLACE " " "_" WorkspaceName ${WorkspaceName})

file(REAL_PATH ${CMAKE_TOOLCHAIN_FILE} CMAKE_TOOLCHAIN_FILE EXPAND_TILDE)
if(NOT EXISTS  ${CMAKE_TOOLCHAIN_FILE})
    message(FATAL_ERROR "CMAKE_TOOLCHAIN_FILE does not exist: CMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}")
endif()

project(${WorkspaceName})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR}")

include(ProcessorCount)
ProcessorCount(PROCESSOR_COUNT)


if(NOT PROCESSOR_COUNT EQUAL 0)
  set(CTEST_BUILD_FLAGS -j${PROCESSOR_COUNT})
  set(CTEST_TEST_ARGS ${CTEST_TEST_ARGS} PARALLEL_LEVEL ${PROCESSOR_COUNT})
endif()

include(CMakeDependentOption)
include(CMake/PreventInSourceBuilds.cmake)
include(CMake/StaticAnalizers.cmake)
include(CMake/ClangFormat.cmake)
include(CMake/Sanitizers.cmake)
include(CMake/TestTarget.cmake)
include(CMake/BenchmarkTarget.cmake)



include(CMake/DefaultProjectSettings.cmake)


if(CMAKE_VERSION GREATER_EQUAL "3.26.0" )
  # CMAKE experimental C++ module
  set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "2182bf5c-ef0d-489a-91da-49dbc3090d2a")
  set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP 1)
  set(CMAKE_CXX_SCAN_FOR_MODULES 1)

  if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  
    string(CONCAT CMAKE_EXPERIMENTAL_CXX_SCANDEP_SOURCE
      "<CMAKE_CXX_COMPILER> <DEFINES> <INCLUDES> <FLAGS> <SOURCE>"
      " -MT <DYNDEP_FILE> -MD -MF <DEP_FILE>"
      " ${flags_to_scan_deps} -fdep-file=<DYNDEP_FILE> -fdep-output=<OBJECT>"
      )

    set(CMAKE_EXPERIMENTAL_CXX_MODULE_MAP_FORMAT "gcc")
    set(CMAKE_EXPERIMENTAL_CXX_MODULE_MAP_FLAG
      "${compiler_flags_for_module_map} -fmodule-mapper=<MODULE_MAP_FILE>")
  endif()

  if (CMAKE_SYSTEM_NAME STREQUAL Linux)
    add_compile_options( -fmodules-ts )
    add_link_options( -fmodules-ts )
  endif()

endif()

option(BUILD_TESTS "Builds the tests" ON)
CMAKE_DEPENDENT_OPTION(BUILD_TESTS_COVERAGE "Build the project with code coverage support for tests" OFF "BUILD_TESTS" OFF)
CMAKE_DEPENDENT_OPTION(RUN_TESTS_AFTER_BUILD "Run the tests after the build" OFF "BUILD_TESTS" OFF)
#CMAKE_DEPENDENT_OPTION(RUN_TESTS_AFTER_BUILD "Run the tests after the build" ON "BUILD_TESTS" OFF)

option(BUILD_BENCHMARKS "Builds the benchmarks" ON)

option(BUILD_DOCUMENTATION "Build the documentation of the Beyond game engine" OFF)
if (BUILD_DOCUMENTATION)
  include("doxygen")
endif()

# Docs only available if this is the main app
#find_package(Doxygen)
#if(Doxygen_FOUND)
#  add_subdirectory(docs)
#else()
#  message(STATUS "Doxygen not found, not building docs")
#endif()


# FetchContent added in CMake 3.11, downloads during the configure step
# FetchContent_MakeAvailable was not added until CMake 3.14
include(FetchContent)
# Formatting library, adds fmt::fmt Always use the full git hash, not the tag,
# safer and faster to recompile
#FetchContent_Declare(
#  fmtlib
#  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
#  GIT_TAG 8.0.1)
#FetchContent_MakeAvailable(fmtlib)


find_package(GTest CONFIG REQUIRED)
find_package(benchmark CONFIG REQUIRED)

find_package(fmt CONFIG REQUIRED)
link_libraries(fmt::fmt)


# macro(get_dir_list result curdir)
#     file(GLOB children ${curdir}/*)
#     set(dirlist "")
#     foreach(child ${children})
#         if(IS_DIRECTORY ${child})
#             list(APPEND dirlist ${child})
#         endif()
#     endforeach()
#     set(${result} ${dirlist})
# ENDMACRO()

# get_dir_list(ProjectDirList Projects)

# foreach(subdir ${ProjectDirList})
#   add_subdirectory(${ProjectDirList})
# endforeach()

# Testing only available if this is the main app. Note this needs to be done
# in the main CMakeLists since it calls enable_testing, which must be in the
# main CMakeLists.
if (BUILD_TESTS)
  enable_testing()
  include(GoogleTest)

  if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
      set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  endif()
endif()

add_subdirectory(Projects/Core)
add_subdirectory(Projects/TestUtilities)
add_subdirectory(Projects/ApplicationCore)
add_subdirectory(Projects/BasicWindowApp)
