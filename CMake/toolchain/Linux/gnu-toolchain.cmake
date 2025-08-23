# GNU GCC toolchain
include_guard(GLOBAL)

set(CMAKE_SYSTEM_NAME Linux CACHE STRING "")

include(${CMAKE_CURRENT_LIST_DIR}/../common-toolchain.cmake)

set(CMAKE_CXX_COMPILER "gcc++")
set(CMAKE_C_COMPILER "gcc")

link_libraries(stdc++exp)

include(${CMAKE_CURRENT_LIST_DIR}/../vcpkg-toolchain.cmake)
