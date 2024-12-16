include_guard()

# find_package(Vulkan COMPONENTS dxc SPIRV-Tools REQUIRED)
find_package(VulkanHeaders CONFIG REQUIRED)

if(NOT VulkanHeaders_FOUND)
  message(FATAL_ERROR "VulkanHeaders not found.")
endif()

get_target_property(VulkanHeaders_INCLUDE_DIRS Vulkan::Headers INTERFACE_INCLUDE_DIRECTORIES)

if(MSVC)
  find_package(VulkanLoader CONFIG REQUIRED)

  if(NOT VulkanLoader_FOUND)
    message(FATAL_ERROR "VulkanLoader not found.")
  endif()
endif(MSVC)

add_library(VulkanHppModule INTERFACE)
add_library("Vulkan::CppModule" ALIAS "VulkanHppModule")
target_compile_definitions(VulkanHppModule INTERFACE VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1 VULKAN_HPP_NO_STRUCT_CONSTRUCTORS)

get_target_property(VulkanHppModuleType VulkanHppModule TYPE)

# target_compile_features("VulkanHppModule" INTERFACE cxx_std_23)

if(NOT ${VulkanHppModuleType} STREQUAL "INTERFACE_LIBRARY")
  target_include_directories(VulkanHppModule
    PRIVATE
    ${VulkanHeaders_INCLUDE_DIRS}
  )
endif()

if(NOT ${VulkanHppModuleType} STREQUAL "INTERFACE_LIBRARY")
    target_sources(VulkanHppModule
      PUBLIC
      FILE_SET vulkanhpp_module_file_set
      TYPE CXX_MODULES
      BASE_DIRS ${VulkanHeaders_INCLUDE_DIRS}
      FILES ${VulkanHeaders_INCLUDE_DIRS}/vulkan/vulkan.cppm
    )
endif()

target_link_libraries(VulkanHppModule
  INTERFACE
  Vulkan::Headers
  PersonalWorks::PersonalWorks_options
  PersonalWorks::PersonalWorks_warnings
  PersonalWorks::PersonalWorks_sanitizers
)

if(MSVC)
  if(${VulkanHppModuleType} STREQUAL "INTERFACE_LIBRARY")
      target_link_libraries(VulkanHppModule INTERFACE Vulkan::Loader)
  else()
    target_link_libraries(VulkanHppModule PRIVATE Vulkan::Loader)
  endif()
endif(MSVC)
