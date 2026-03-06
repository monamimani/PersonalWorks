cmake_minimum_required(VERSION 3.28 FATAL_ERROR)

include("${CMAKE_CURRENT_LIST_DIR}/configure-ports.cmake")

set(VCPKG_TARGET_ARCHITECTURE x64)
set(VCPKG_CRT_LINKAGE dynamic)
set(VCPKG_LIBRARY_LINKAGE dynamic)

set(VCPKG_CMAKE_SYSTEM_NAME Linux CACHE STRING "")

set(VCPKG_ENV_PASSTHROUGH BUILD_SANITIZERS CC CXX)
set(BUILD_SANITIZERS "$ENV{BUILD_SANITIZERS}")

if(DEFINED ENV{CC})
  set(VCPKG_C_COMPILER "$ENV{CC}")
endif()
if(DEFINED ENV{CXX})
  set(VCPKG_CXX_COMPILER "$ENV{CXX}")
endif()

include("${CMAKE_CURRENT_LIST_DIR}/../../CMake/Features/SanitizersLinux.cmake")
set(SANITIZER_FLAGS_LIST "")
get_sanitizers_flags_linux(SANITIZER_FLAGS_LIST)

set(SANITIZER_FLAGS "")
list(JOIN SANITIZER_FLAGS_LIST " " SANITIZER_FLAGS)

# include(CMakePrintHelpers)
# cmake_print_variables(CMAKE_CURRENT_LIST_DIR)
# cmake_print_variables(SANITIZER_FLAGS)
# cmake_print_variables(BUILD_SANITIZERS)

string(APPEND VCPKG_C_FLAGS "${SANITIZER_FLAGS}")
string(APPEND VCPKG_CXX_FLAGS "${SANITIZER_FLAGS}")
