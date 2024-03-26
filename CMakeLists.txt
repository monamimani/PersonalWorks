cmake_minimum_required(VERSION 3.28 FATAL_ERROR)

set(VCPKG_INSTALLED_DIR ${CMAKE_BINARY_DIR}/../../vcpkg_installed)

if(NOT DEFINED CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 23)
endif()

include(CMakePrintHelpers)
include(CMake/PreventInSourceBuilds.cmake)
include(CMake/ValidateVcpkgToolchain.cmake)
include(CMake/GetCPM.cmake)
include(CMake/VulkanDependencies.cmake)

cmake_print_variables(CMAKE_VERSION)
# cmake_print_variables(CMAKE_MODULE_PATH)
# set(CMAKE_VERBOSE_MAKEFILE ON)

# get_filename_component(WorkspaceName ${CMAKE_SOURCE_DIR} NAME)
# string(REPLACE " " "_" WorkspaceName ${WorkspaceName})
project(PersonalWorks
  VERSION 0.0.1
  DESCRIPTION ""
  HOMEPAGE_URL "https://github.com/monamimani/PersonalWorks"
  LANGUAGES CXX C)

include(CMakeProjectOptions.cmake)
PersonalWorks_declare_options()
PersonalWorks_global_options()
PersonalWorks_target_options()

find_package(fmt CONFIG REQUIRED)

# If this project is consumed by another, don't setup tests.
if(PROJECT_IS_TOP_LEVEL)
  include(CMake/TestDependencies.cmake)
  include(CMake/TestTarget.cmake)

  if(NOT PROCESSOR_COUNT EQUAL 0)
    set(CTEST_BUILD_FLAGS -j${PROCESSOR_COUNT})
    set(CTEST_TEST_ARGS ${CTEST_TEST_ARGS} PARALLEL_LEVEL ${PROCESSOR_COUNT})
  endif()
endif()



add_subdirectory(Projects/Config)
add_subdirectory(Projects/TestUtilities)
add_subdirectory(Projects/Core)
add_subdirectory(Projects/Gfx)
add_subdirectory(Projects/ApplicationCore)
add_subdirectory(Projects/BasicWindowApp)

# set the startup project for the "play" button in MSVC
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT CoreTests)

# foreach(ITEM ${PROJECT_CONTENTS})
#   if(IS_DIRECTORY ${ITEM})
#     file(RELATIVE_PATH FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/Projects/" ${ITEM})
#     add_subdirectory(Projects/${FOLDER})
#   endif()
# endforeach()
