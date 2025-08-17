include_guard()

if(EXISTS "${CMAKE_SOURCE_DIR}/vcpkg.json")
    # This toolchain file is meant to be used with vcpkg.
    # We need to include the vcpkg toolchain file.
    # The path to vcpkg is needed.
    # We check the environment variable VCPKG_ROOT, or assume it's a submodule.
    if(DEFINED ENV{VCPKG_ROOT})
      set(VCPKG_PATH "$ENV{VCPKG_ROOT}")
    elseif(IS_DIRECTORY "${CMAKE_SOURCE_DIR}/vcpkg")
      set(VCPKG_PATH "${CMAKE_SOURCE_DIR}/vcpkg")
    else()
      message(FATAL_ERROR "vcpkg.json found, but VCPKG_ROOT is not set and vcpkg submodule not found.")
    endif()

    set(VCPKG_TOOLCHAIN "${VCPKG_PATH}/scripts/buildsystems/vcpkg.cmake")
    if(NOT EXISTS "${VCPKG_TOOLCHAIN}")
      message(FATAL_ERROR "vcpkg toolchain not found at ${VCPKG_TOOLCHAIN}")
    endif()
    include("${VCPKG_TOOLCHAIN}")
endif()

# Common settings for all toolchains
set(CMAKE_CXX_STANDARD 23 CACHE STRING "The C++ standard to use")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)

set(CMAKE_COLOR_DIAGNOSTICS ON)
set(CMAKE_COLOR_MAKEFILE ON)

# Organize unrelated targets to clean IDE hierarchy.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "Thirdparty")
