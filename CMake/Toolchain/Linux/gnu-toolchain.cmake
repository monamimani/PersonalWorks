# GNU GCC toolchain
include_guard(GLOBAL)

cmake_minimum_required(VERSION 3.28 FATAL_ERROR)

set(CMAKE_SYSTEM_NAME Linux CACHE STRING "")

include(${CMAKE_CURRENT_LIST_DIR}/../common-toolchain.cmake)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_C_COMPILER "gcc")

string(APPEND DEFAULT_C_FLAGS "-Wno-builtin-macro-redefined")
string(APPEND DEFAULT_CXX_FLAGS "-Wno-builtin-macro-redefined")

link_libraries(stdc++exp)

include(${CMAKE_CURRENT_LIST_DIR}/../../Features/SanitizersLinux.cmake)
set(SANITIZER_FLAGS_LIST "")
get_sanitizers_flags_linux(SANITIZER_FLAGS_LIST)

set(SANITIZER_FLAGS "")
list(JOIN SANITIZER_FLAGS_LIST " " SANITIZER_FLAGS)

set(VCPKG_TARGET_TRIPLET "x64-linux-sanitizers" CACHE STRING "Vcpkg target triplet")

set(CMAKE_C_FLAGS_INIT "${DEFAULT_C_FLAGS} ${SANITIZER_FLAGS}")
set(CMAKE_CXX_FLAGS_INIT "${DEFAULT_CXX_FLAGS} ${SANITIZER_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS_INIT "${DEFAULT_SHARED_LINKER_FLAGS} ${SANITIZER_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS_INIT "${DEFAULT_EXE_LINKER_FLAGS} ${SANITIZER_FLAGS}")

include(${CMAKE_CURRENT_LIST_DIR}/../vcpkg-toolchain.cmake)
