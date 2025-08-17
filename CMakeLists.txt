cmake_minimum_required(VERSION 3.28 FATAL_ERROR)

include(CMakePrintHelpers)

include(CheckCXXCompilerFlag)


include(CMake/PreventInSourceBuilds.cmake)
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

include(CMakeProjectOptions.cmake)
PersonalWorks_declare_options()
PersonalWorks_global_options()
PersonalWorks_target_options()

include(CMake/VulkanDependencies.cmake)

# If this project is consumed by another, don't setup tests.
if(PROJECT_IS_TOP_LEVEL AND PersonalWorks_BUILD_TESTS)
  enable_testing()
  include(CMake/TestDependencies.cmake)

  if(NOT PROCESSOR_COUNT EQUAL 0)
    set(CTEST_BUILD_FLAGS -j${PROCESSOR_COUNT})
    set(CTEST_TEST_ARGS ${CTEST_TEST_ARGS} PARALLEL_LEVEL ${PROCESSOR_COUNT})
  endif()
endif()

add_subdirectory(Projects/CodeIncubator)
add_subdirectory(Projects/CppExploration)
#add_subdirectory(Projects/CodeExercises)
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
