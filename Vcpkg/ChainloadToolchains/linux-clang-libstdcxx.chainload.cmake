include_guard()

set(CMAKE_SYSTEM_NAME Linux CACHE STRING "")

set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")

# Optional: Ensure lld is used if available to match project settings
set(CMAKE_LINKER "ld.lld")
