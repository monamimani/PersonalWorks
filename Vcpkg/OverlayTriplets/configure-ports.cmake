include_guard()


if(PORT STREQUAL "vulkan-headers")
    list(APPEND VCPKG_CMAKE_CONFIGURE_OPTIONS "-DVULKAN_HEADERS_ENABLE_MODULE=ON")

endif()
