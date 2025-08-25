# MSVC toolchain
include_guard(GLOBAL)

set(CMAKE_SYSTEM_NAME Windows CACHE STRING "")

include(${CMAKE_CURRENT_LIST_DIR}/../common-toolchain.cmake)

set(CMAKE_CXX_COMPILER "cl")
set(CMAKE_C_COMPILER "cl")

string(APPEND DEFAULT_C_FLAGS " /MP /utf-8 /diagnostics:caret")
string(APPEND DEFAULT_CXX_FLAGS " /MP /utf-8 /diagnostics:caret")

#I should set the dynamicdeoptimization in release mode. not in shipping builds
#add_compile_options("/dynamicdeopt")

if("$ENV{VSCMD_VER}" STREQUAL "")
  message(WARNING "Not in a developer environment, run cmake from a Visual Studio Developer terminal.")
endif()

include(${CMAKE_CURRENT_LIST_DIR}/msvc-sanitizers.cmake)

set(CMAKE_C_FLAGS_INIT "${DEFAULT_C_FLAGS}")
set(CMAKE_CXX_FLAGS_INIT "${DEFAULT_CXX_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS_INIT "${DEFAULT_SHARED_LINKER_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS_INIT "${DEFAULT_EXE_LINKER_FLAGS}")

include(${CMAKE_CURRENT_LIST_DIR}/../vcpkg-toolchain.cmake)
