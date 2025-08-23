# MSVC toolchain
include_guard(GLOBAL)

set(CMAKE_SYSTEM_NAME Windows CACHE STRING "")

include(${CMAKE_CURRENT_LIST_DIR}/../common-toolchain.cmake)

set(CMAKE_CXX_COMPILER "cl")
set(CMAKE_C_COMPILER "cl")

# MSVC specific settings
string(APPEND CMAKE_C_FLAGS_INIT " /MP /utf-8 /diagnostics:caret")
string(APPEND CMAKE_CXX_FLAGS_INIT " /MP /utf-8 /diagnostics:caret")

#I should set the dynamicdeoptimization in release mode. not in shipping builds
#add_compile_options("/dynamicdeopt")

if("$ENV{VSCMD_VER}" STREQUAL "")
  message(WARNING "Not in a developer environment, run cmake from a Visual Studio Developer terminal.")
endif()

include(${CMAKE_CURRENT_LIST_DIR}/../vcpkg-toolchain.cmake)
