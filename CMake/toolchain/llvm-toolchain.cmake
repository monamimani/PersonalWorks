# LLVM/Clang toolchain
include_guard(GLOBAL)

set(CMAKE_SYSTEM_NAME Linux CACHE STRING "")

include(${CMAKE_CURRENT_LIST_DIR}/common-toolchain.cmake)

set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_C_COMPILER "clang")

string(APPEND CMAKE_C_FLAGS_INIT " -Wno-builtin-macro-redefined")
string(APPEND CMAKE_CXX_FLAGS_INIT " -Wno-builtin-macro-redefined")

link_libraries(stdc++exp)

include(${CMAKE_CURRENT_LIST_DIR}/vcpkg-toolchain.cmake)


