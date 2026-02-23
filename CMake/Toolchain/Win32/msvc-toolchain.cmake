# MSVC toolchain
include_guard(GLOBAL)

cmake_minimum_required(VERSION 3.28 FATAL_ERROR)

set(CMAKE_SYSTEM_NAME Windows CACHE STRING "")


if("$ENV{VSCMD_VER}" STREQUAL "")
  message(FATAL_ERROR "Not in a developer environment, run cmake from a Visual Studio Developer terminal.")
endif()

include(${CMAKE_CURRENT_LIST_DIR}/../common-toolchain.cmake)

set(CMAKE_CXX_COMPILER "cl")
set(CMAKE_C_COMPILER "cl")

string(APPEND DEFAULT_C_FLAGS "/MP /utf-8 /diagnostics:caret")
string(APPEND DEFAULT_CXX_FLAGS "/MP /utf-8 /diagnostics:caret")

#I should set the dynamicdeoptimization in release mode. not in shipping builds
#add_compile_options("/dynamicdeopt")

include(${CMAKE_CURRENT_LIST_DIR}/../../Features/SanitizersMsvc.cmake)
set(SANITIZER_FLAGS_LIST "")
get_sanitizers_flags_msvc(SANITIZER_FLAGS_LIST)

set(SANITIZER_FLAGS "")
list(JOIN SANITIZER_FLAGS_LIST " " SANITIZER_FLAGS)

#include(CMakePrintHelpers)
#cmake_print_variables(CMAKE_CURRENT_LIST_DIR)
#cmake_print_variables(SANITIZER_FLAGS)
#cmake_print_variables(BUILD_SANITIZERS)

set(VCPKG_TARGET_TRIPLET "x64-windows-sanitizers" CACHE STRING "Vcpkg target triplet")

set(CMAKE_C_FLAGS_INIT "${DEFAULT_C_FLAGS} ${SANITIZER_FLAGS}")
set(CMAKE_CXX_FLAGS_INIT "${DEFAULT_CXX_FLAGS} ${SANITIZER_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS_INIT "${DEFAULT_SHARED_LINKER_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS_INIT "${DEFAULT_EXE_LINKER_FLAGS}")

#set(VCPKG_VERBOSE ON)
include(${CMAKE_CURRENT_LIST_DIR}/../vcpkg-toolchain.cmake)
