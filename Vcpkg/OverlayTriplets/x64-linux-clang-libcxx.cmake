cmake_minimum_required(VERSION 3.28 FATAL_ERROR)

include("${CMAKE_CURRENT_LIST_DIR}/configure-ports.cmake")

set(VCPKG_TARGET_ARCHITECTURE x64)
set(VCPKG_CRT_LINKAGE dynamic)
set(VCPKG_LIBRARY_LINKAGE static)

set(VCPKG_CMAKE_SYSTEM_NAME Linux)

# set(VCPKG_VERBOSE ON)
set(VCPKG_CHAINLOAD_TOOLCHAIN_FILE ${CMAKE_CURRENT_LIST_DIR}/../ChainloadToolchains/linux-clang-libcxx.chainload.cmake)
