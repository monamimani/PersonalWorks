

get_filename_component(targetName ${CMAKE_CURRENT_LIST_DIR} NAME)

#if(BUILD_KIND STREQUAL "Dev")
#  set(BUILD_KIND_DEV ON)
#elseif(BUILD_KIND STREQUAL "QA")
#  set(BUILD_KIND_QA ON)
#elseif(BUILD_KIND STREQUAL "Profile")
#  set(BUILD_KIND_PROFILE ON)
#elseif(BUILD_KIND STREQUAL "Shipping")
#  set(BUILD_KIND_SHIPPING ON)
#endif()

configure_file("${targetName}/Config.h.in" "${targetName}/Config.h" ESCAPE_QUOTES)

add_library(${targetName} INTERFACE)
add_library(${PROJECT_NAME}::${targetName} ALIAS ${targetName})

target_include_directories(${targetName}
  INTERFACE
  "${CMAKE_CURRENT_BINARY_DIR}"
)
