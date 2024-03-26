include_guard()

find_package(Vulkan COMPONENTS dxc SPIRV-Tools REQUIRED)

if (NOT Vulkan_FOUND)
  message(FATAL_ERROR "Vulkan not found.")
endif()

add_library(VulkanHppModule INTERFACE)
#target_sources(VulkanHppModule PRIVATE
#  FILE_SET CXX_MODULES
#  BASE_DIRS ${Vulkan_INCLUDE_DIR} 
#  FILES ${Vulkan_INCLUDE_DIR}/vulkan/vulkan.cppm
#)
target_compile_definitions(VulkanHppModule INTERFACE VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1 VULKAN_HPP_NO_STRUCT_CONSTRUCTORS) 

target_link_libraries(VulkanHppModule INTERFACE Vulkan::Vulkan)
