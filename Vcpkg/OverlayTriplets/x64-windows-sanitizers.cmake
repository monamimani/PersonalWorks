set(VCPKG_TARGET_ARCHITECTURE x64)
set(VCPKG_CRT_LINKAGE dynamic)
set(VCPKG_LIBRARY_LINKAGE dynamic)

set(VCPKG_ENV_PASSTHROUGH BUILD_SANITIZERS)
set(BUILD_SANITIZERS "$ENV{BUILD_SANITIZERS}")
include("${CMAKE_CURRENT_LIST_DIR}/../../CMake/Features/SanitizersMsvc.cmake")


include(CMakePrintHelpers)
cmake_print_variables(BUILD_SANITIZERS)
cmake_print_variables(SANITIZERS_LIST)
cmake_print_variables(SANITIZER_FLAGS)

string(APPEND VCPKG_C_FLAGS "${SANITIZER_FLAGS}")
string(APPEND VCPKG_CXX_FLAGS "${SANITIZER_FLAGS}")
