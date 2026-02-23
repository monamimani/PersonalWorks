include_guard()


# find_package(Vulkan COMPONENTS dxc SPIRV-Tools REQUIRED)
find_package(VulkanHeaders REQUIRED CONFIG)

if (NOT TARGET Vulkan::Headers)
    message(FATAL_ERROR "Vulkan::Headers target not defined")
endif()

get_target_property(VulkanHeaders_INCLUDE_DIRS Vulkan::Headers INTERFACE_INCLUDE_DIRECTORIES)

if(MSVC)
  find_package(VulkanLoader CONFIG REQUIRED)

  if(NOT VulkanLoader_FOUND)
    message(FATAL_ERROR "VulkanLoader not found.")
  endif()
endif(MSVC)

add_library(VulkanHppCppModule)
add_library("VulkanHpp::CppModule" ALIAS "VulkanHppCppModule")

target_compile_features(VulkanHppCppModule INTERFACE cxx_std_23)

target_compile_definitions(VulkanHppCppModule
  PUBLIC
    VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1
    VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
)

#target_include_directories(VulkanHppCppModule
#  PRIVATE
#    ${VulkanHeaders_INCLUDE_DIRS}
#)

find_path(VULKAN_HPP_CPPM_DIR
    NAMES "vulkan/vulkan.cppm"
    REQUIRED
    MESSAGE_FAIL_REASON "Could not locate vulkan.cppm in the include directories."
)

find_path(VULKAN_HPP_VIDEO_CPPM_DIR
    NAMES "vulkan/vulkan_video.cppm"
    REQUIRED
    MESSAGE_FAIL_REASON "Could not locate vulkan.cppm in the include directories."
)

target_sources(VulkanHppCppModule
  PUBLIC
  FILE_SET vulkanhpp_cppmodule_file_set
  TYPE CXX_MODULES
  FILES
  "${VULKAN_HPP_CPPM_DIR}/vulkan/vulkan.cppm"
  "${VULKAN_HPP_VIDEO_CPPM_DIR}/vulkan/vulkan_video.cppm"
)

target_link_libraries(VulkanHppCppModule
  PUBLIC
  Vulkan::Headers
  PRIVATE
  PersonalWorks::PersonalWorks_options
  #PersonalWorks::PersonalWorks_warnings
  PersonalWorks::PersonalWorks_sanitizers
)

if(MSVC)
    target_link_libraries(VulkanHppCppModule PRIVATE Vulkan::Loader)
endif(MSVC)
