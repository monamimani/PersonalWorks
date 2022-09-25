cmake_minimum_required(VERSION 3.22)

include(CMakePrintHelpers)

get_filename_component(WorkspaceName ${CMAKE_SOURCE_DIR} NAME)
string(REPLACE " " "_" WorkspaceName ${WorkspaceName})

project(${WorkspaceName})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR}")

cmake_print_variables(CMAKE_MODULE_PATH)

include(ProcessorCount)
ProcessorCount(PROCESSOR_COUNT)


if(NOT PROCESSOR_COUNT EQUAL 0)
  set(CTEST_BUILD_FLAGS -j${PROCESSOR_COUNT})
  set(CTEST_TEST_ARGS ${CTEST_TEST_ARGS} PARALLEL_LEVEL ${PROCESSOR_COUNT})
endif()

include(CMakeDependentOption)
include(CMake/StaticAnalizers.cmake)
include(CMake/ClangFormat.cmake)
include(CMake/Sanitizers.cmake)

# Prevent in-source builds.
if ( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} )
message( FATAL_ERROR “In-source builds not allowed! Create a build directory and run CMake from there. ” )
endif()

include(CMake/DefaultProjectSettings.cmake)


option(BUILD_TESTS "Builds the tests" ON)
CMAKE_DEPENDENT_OPTION(BUILD_TESTS_COVERAGE "Build the project with code coverage support for tests" OFF "BUILD_TESTS" OFF)

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

  if(MSVC)
      set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  endif()
endif()

add_subdirectory(Projects/Core)
add_subdirectory(Projects/ApplicationCore)
add_subdirectory(Projects/BasicWindowApp)
add_subdirectory(Tests)