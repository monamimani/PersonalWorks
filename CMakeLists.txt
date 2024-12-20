cmake_minimum_required(VERSION 3.28 FATAL_ERROR)

set(VCPKG_INSTALLED_DIR ${CMAKE_BINARY_DIR}/../../vcpkg_installed)

if(NOT DEFINED CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 23)
endif()

include(CheckCXXCompilerFlag)

include(CMakePrintHelpers)
include(CMake/PreventInSourceBuilds.cmake)
include(CMake/ValidateVcpkgToolchain.cmake)
include(CMake/GetCPM.cmake)

cmake_print_variables(CMAKE_VERSION)

#if(MSVC)
#  set_target_properties(LLVMInstallDir PROPERTIES VS_GLOBAL_LLVMInstallDir "C:/Program Files/LLVM/bin")
#endif()

# cmake_print_variables(CMAKE_MODULE_PATH)
# set(CMAKE_VERBOSE_MAKEFILE ON)

# get_filename_component(WorkspaceName ${CMAKE_SOURCE_DIR} NAME)
# string(REPLACE " " "_" WorkspaceName ${WorkspaceName})
project(PersonalWorks
  VERSION 0.0.1
  DESCRIPTION ""
  HOMEPAGE_URL "https://github.com/monamimani/PersonalWorks"
  LANGUAGES CXX C)

if(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
  add_compile_options(-std=c++2c)
  add_compile_options(-D__cpp_concepts=202002 -Wno-builtin-macro-redefined)
  add_link_options("-lstdc++exp")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  add_compile_options("-std=c++26")
endif()

if(MSVC)
  add_compile_options("/std:c++latest")
endif()

include(CMakeProjectOptions.cmake)
PersonalWorks_declare_options()
PersonalWorks_global_options()
PersonalWorks_target_options()

include(CMake/VulkanDependencies.cmake)

find_package(fmt CONFIG REQUIRED)

# If this project is consumed by another, don't setup tests.
if(PROJECT_IS_TOP_LEVEL AND PersonalWorks_BUILD_TESTS)
  enable_testing()
  include(CMake/TestDependencies.cmake)

  if(NOT PROCESSOR_COUNT EQUAL 0)
    set(CTEST_BUILD_FLAGS -j${PROCESSOR_COUNT})
    set(CTEST_TEST_ARGS ${CTEST_TEST_ARGS} PARALLEL_LEVEL ${PROCESSOR_COUNT})
  endif()
endif()

add_subdirectory(Projects/Config)
add_subdirectory(Projects/EnginePlayground)
add_subdirectory(Projects/CodeIncubator)
add_subdirectory(Projects/CppExploration)


# set the startup project for the "play" button in MSVC
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT CoreTests)

# foreach(ITEM ${PROJECT_CONTENTS})
# if(IS_DIRECTORY ${ITEM})
# file(RELATIVE_PATH FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/Projects/" ${ITEM})
# add_subdirectory(Projects/${FOLDER})
# endif()
# endforeach()

execute_process(
    COMMAND ${CMAKE_COMMAND} -E create_symlink
        ${CMAKE_BINARY_DIR}/compile_commands.json
        ${CMAKE_SOURCE_DIR}/compile_commands.json
)
