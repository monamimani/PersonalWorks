include_guard()

#find_package(Vulkan COMPONENTS dxc SPIRV-Tools REQUIRED)
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


if(MSVC AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 19.41)
  add_library(VulkanHppModule)

  target_compile_definitions(VulkanHppModule INTERFACE VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1 VULKAN_HPP_NO_STRUCT_CONSTRUCTORS)

  target_include_directories(VulkanHppModule
      PRIVATE
          ${VulkanHeaders_INCLUDE_DIRS}
  )

  target_sources(VulkanHppModule
    PUBLIC
      FILE_SET vulkanhpp_module_file_set
      TYPE CXX_MODULES
      BASE_DIRS ${VulkanHeaders_INCLUDE_DIRS}
      FILES ${VulkanHeaders_INCLUDE_DIRS}/vulkan/vulkan.cppm
  )

  target_link_libraries(VulkanHppModule
    PRIVATE
      PersonalWorks::PersonalWorks_options
      PersonalWorks::PersonalWorks_warnings
      PersonalWorks::PersonalWorks_sanitizers
  )

  if(MSVC)
    target_link_libraries(VulkanHppModule PRIVATE Vulkan::Loader)
  endif(MSVC)

else()
  add_library(VulkanHppModule INTERFACE)

  target_compile_definitions(VulkanHppModule INTERFACE VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1 VULKAN_HPP_NO_STRUCT_CONSTRUCTORS)

  target_link_libraries(VulkanHppModule INTERFACE Vulkan::Headers)

  target_link_libraries(VulkanHppModule
    INTERFACE
      PersonalWorks::PersonalWorks_options
      PersonalWorks::PersonalWorks_warnings
      PersonalWorks::PersonalWorks_sanitizers
  )

  if(MSVC)
    target_link_libraries(VulkanHppModule INTERFACE Vulkan::Loader)
  endif(MSVC)

endif()

