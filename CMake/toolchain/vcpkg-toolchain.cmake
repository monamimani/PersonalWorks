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

    set(VCPKG_TOOLCHAIN_PATH "${VCPKG_PATH}/scripts/buildsystems/vcpkg.cmake")
    file(REAL_PATH ${VCPKG_TOOLCHAIN_PATH} VCPKG_TOOLCHAIN_PATH EXPAND_TILDE)
    if(NOT EXISTS "${VCPKG_TOOLCHAIN_PATH}")
      message(FATAL_ERROR "vcpkg toolchain not found at ${VCPKG_TOOLCHAIN_PATH}")
    endif()

    #set(VCPKG_VERBOSE ON)
    include("${VCPKG_TOOLCHAIN_PATH}")
endif()
