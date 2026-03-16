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

list(APPEND TC_BASE_C_FLAGS /MP /utf-8 /diagnostics:caret)
list(APPEND TC_BASE_CXX_FLAGS /MP /utf-8 /diagnostics:caret)

#I should set the dynamicdeoptimization in release mode. not in shipping builds
#add_compile_options("/dynamicdeopt")

include(${CMAKE_CURRENT_LIST_DIR}/../../Features/SanitizersMsvc.cmake)
set(SANITIZER_FLAGS_LIST "")
get_sanitizers_flags_msvc(SANITIZER_FLAGS_LIST)

list(APPEND TC_BASE_C_FLAGS ${SANITIZER_FLAGS_LIST})
list(APPEND TC_BASE_CXX_FLAGS ${SANITIZER_FLAGS_LIST})

#include(CMakePrintHelpers)
#cmake_print_variables(CMAKE_CURRENT_LIST_DIR)
#cmake_print_variables(SANITIZER_FLAGS)
#cmake_print_variables(BUILD_SANITIZERS)

set(VCPKG_TARGET_TRIPLET "x64-windows-sanitizers" CACHE STRING "Vcpkg target triplet")

list(JOIN TC_BASE_C_FLAGS " " TC_BASE_C_FLAGS_STRING)
set(CMAKE_C_FLAGS_INIT "${CMAKE_C_FLAGS_INIT} ${TC_BASE_C_FLAGS_STRING}")

list(JOIN TC_BASE_CXX_FLAGS " " TC_BASE_CXX_FLAGS_STRING)
set(CMAKE_CXX_FLAGS_INIT "${CMAKE_CXX_FLAGS_INIT} ${TC_BASE_CXX_FLAGS_STRING}")

list(JOIN TC_BASE_SHARED_LINKER_FLAGS " " TC_BASE_SHARED_LINKER_FLAGS_STRING)
set(CMAKE_SHARED_LINKER_FLAGS_INIT "${CMAKE_SHARED_LINKER_FLAGS_INIT} ${TC_BASE_SHARED_LINKER_FLAGS_STRING}")

list(JOIN TC_BASE_EXE_LINKER_FLAGS " " TC_BASE_EXE_LINKER_FLAGS_STRING)
set(CMAKE_EXE_LINKER_FLAGS_INIT "${CMAKE_EXE_LINKER_FLAGS_INIT} ${TC_BASE_EXE_LINKER_FLAGS_STRING}")

list(JOIN TC_BASE_MODULE_LINKER_FLAGS " " TC_BASE_MODULE_LINKER_FLAGS_STRING)
set(CMAKE_MODULE_LINKER_FLAGS_INIT "${CMAKE_MODULE_LINKER_FLAGS_INIT} ${TC_BASE_MODULE_LINKER_FLAGS_STRING}")

#set(VCPKG_VERBOSE ON)
include(${CMAKE_CURRENT_LIST_DIR}/../vcpkg-toolchain.cmake)
