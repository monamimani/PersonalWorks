# LLVM/Clang toolchain
include_guard(GLOBAL)

set(CMAKE_SYSTEM_NAME Linux CACHE STRING "")

include(${CMAKE_CURRENT_LIST_DIR}/common-toolchain.cmake)

set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_C_COMPILER "clang")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-builtin-macro-redefined")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-builtin-macro-redefined")

link_libraries(stdc++exp)

set(CLANG_WARNINGS
    -Wall
    -Wextra # reasonable and standard
    -Wshadow # warn the user if a variable declaration shadows one from a parent context
    -Wnon-virtual-dtor # warn the user if a class with virtual functions has a non-virtual destructor. This helps

    # catch hard to track down memory errors
    -Wold-style-cast # warn for c-style casts
    -Wcast-align # warn for potential performance problem casts
    -Wunused # warn on anything being unused
    -Woverloaded-virtual # warn if you overload (not override) a virtual function
    -Wpedantic # warn if non-standard C++ is used
    -Wconversion # warn on type conversions that may lose data
    -Wsign-conversion # warn on sign conversions
    -Wnull-dereference # warn if a null dereference is detected
    -Wdouble-promotion # warn if float is implicit promoted to double
    -Wformat=2 # warn on security issues around functions that format output (ie printf)
    -Wimplicit-fallthrough # warn on statements that fallthrough without an explicit annotation
)

string(REPLACE ";" " " CLANG_WARNINGS_STR "${CLANG_WARNINGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CLANG_WARNINGS_STR}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CLANG_WARNINGS_STR}")


include(${CMAKE_CURRENT_LIST_DIR}/vcpkg-toolchain.cmake)


