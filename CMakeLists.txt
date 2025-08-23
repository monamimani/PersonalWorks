cmake_minimum_required(VERSION 3.28 FATAL_ERROR)

include(CMakePrintHelpers)

include(CheckCXXCompilerFlag)


include(CMake/BuildConfig/PreventInSourceBuilds.cmake)
include(CMake/Dependencies/GetCPM.cmake)

cmake_print_variables(CMAKE_VERSION)

#if(MSVC)
#  set_target_properties(LLVMInstallDir PROPERTIES VS_GLOBAL_LLVMInstallDir "C:/Program Files/LLVM/bin")
#endif()

# cmake_print_variables(CMAKE_MODULE_PATH)
#set(CMAKE_VERBOSE_MAKEFILE ON)

# get_filename_component(WorkspaceName ${CMAKE_SOURCE_DIR} NAME)
# string(REPLACE " " "_" WorkspaceName ${WorkspaceName})
project(PersonalWorks
  VERSION 0.0.1
  DESCRIPTION ""
  HOMEPAGE_URL "https://github.com/monamimani/PersonalWorks"
  LANGUAGES CXX C)

include(CMake/BuildConfig/ProjectSettings.cmake)

option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)
option(PersonalWorks_BASIC_BUILD_MODE "Disable most of the nice to have build features." OFF)
option(PersonalWorks_BUILD_TESTS "Builds the tests" ON)
option(PersonalWorks_BUILD_BENCHMARKS "Builds the benchmarks" ON)

if(PersonalWorks_BASIC_BUILD_MODE)
  message(AUTHOR_WARNING "Basic build mode enabled. All fancy build feature like checks, IPO/LTO are disabled.")
endif()


include(CMake/Features/CodeHardening.cmake)
if(PersonalWorks_ENABLE_GLOBAL_CODE_HARDENING OR PersonalWorks_ENABLE_TARGET_CODE_HARDENING)
  if(NOT SUPPORTS_UBSAN
    OR PersonalWorks_ENABLE_SANITIZER_UNDEFINED
    OR PersonalWorks_ENABLE_SANITIZER_ADDRESS
    OR PersonalWorks_ENABLE_SANITIZER_THREAD
    OR PersonalWorks_ENABLE_SANITIZER_LEAK)
    set(ENABLE_UBSAN_MIN_RUNTIME FALSE)
  else()
    set(ENABLE_UBSAN_MIN_RUNTIME TRUE)
  endif()
  configure_code_hardening(${ENABLE_UBSAN_MIN_RUNTIME})
endif()
if(PersonalWorks_ENABLE_GLOBAL_CODE_HARDENING)
  add_global_code_hardening()
endif()

include(CMake/Features/LibFuzzer.cmake)
configure_libfuzzer()

include(CMake/Features/ClangTidy.cmake)
if(PersonalWorks_ENABLE_CLANG_TIDY)
  configure_clang_tidy()
endif()

include(CMake/Features/CppCheck.cmake)
if(PersonalWorks_ENABLE_CPPCHECK)
  configure_cppcheck()
endif()

include(CMake/Features/CmakeCache.cmake)
if(PersonalWorks_ENABLE_CMAKE_CACHE)
  configure_cmake_cache()
endif()

include(CMake/Features/TestCodeCoverage.cmake)
if(PersonalWorks_ENABLE_TEST_CODE_COVERAGE)
  configure_test_code_coverage()
endif()

include(CMake/Features/Doxygen.cmake)
if(PersonalWorks_ENABLE_DOXYGEN)
  PersonalWorks_enable_doxygen("")
endif()

include(CMake/Features/InterproceduralOptimization.cmake)
if(PersonalWorks_ENABLE_IPO)
  configure_ipo()
endif()

add_library(PersonalWorks_sanitizers INTERFACE)
add_library(PersonalWorks::PersonalWorks_sanitizers ALIAS PersonalWorks_sanitizers)

include(CMake/Features/Sanitizers.cmake)
add_target_sanitizer(
  PersonalWorks_sanitizers,
  PersonalWorks_ENABLE_SANITIZER_ADDRESS,
  PersonalWorks_ENABLE_SANITIZER_LEAK,
  PersonalWorks_ENABLE_SANITIZER_UNDEFINED,
  PersonalWorks_ENABLE_SANITIZER_THREAD,
  PersonalWorks_ENABLE_SANITIZER_MEMORY)


add_library(PersonalWorks_options INTERFACE)
add_library(PersonalWorks::PersonalWorks_options ALIAS PersonalWorks_options)
include(CMake/Targets/AddTargetProjectOptions.cmake)
add_target_project_options(PersonalWorks_options)

add_library(PersonalWorks_warnings INTERFACE)
add_library(PersonalWorks::PersonalWorks_warnings ALIAS PersonalWorks_warnings)
include(CMake/Targets/AddTargetWarnings.cmake)
add_target_warnings(PersonalWorks_warnings OFF)

include(CMake/Dependencies/VulkanDependencies.cmake)

# If this project is consumed by another, don't setup tests.
if(PROJECT_IS_TOP_LEVEL AND PersonalWorks_BUILD_TESTS)
  enable_testing()
  include(CMake/Dependencies/TestDependencies.cmake)

  if(NOT PROCESSOR_COUNT EQUAL 0)
    set(CTEST_BUILD_FLAGS -j${PROCESSOR_COUNT})
    set(CTEST_TEST_ARGS ${CTEST_TEST_ARGS} PARALLEL_LEVEL ${PROCESSOR_COUNT})
  endif()
endif()

add_subdirectory(Projects/CodeIncubator)
add_subdirectory(Projects/CppExploration)
add_subdirectory(Projects/Config)
add_subdirectory(Projects/EnginePlayground)


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
