include_guard()

set(BUILD_TYPES "Debug" "Release" "RelWithDebInfo")

set(BUILD_KIND_LIST "Dev" "QA" "Profile" "Shipping")
set(BUILD_KIND "Dev" CACHE STRING "The kind of configuration for the build.")
set_property(CACHE BUILD_KIND PROPERTY STRINGS ${BUILD_KIND_LIST})

if(NOT BUILD_KIND IN_LIST BUILD_KIND_LIST)
  message(FATAL_ERROR "BUILD_KIND is ${BUILD_KIND} and it must be one of ${BUILD_KIND_LIST}")
endif()

get_property(BUILDING_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)

if(NOT BUILDING_MULTI_CONFIG)
  # Set a default build type if none was specified
  if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "Setting build type to 'RelWithDebInfo' as none was specified.")
    set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Choose the type of build." FORCE)

    # Set the possible values of build type for cmake-gui, ccmake
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS ${BUILD_TYPES})
  endif()

  if(NOT CMAKE_BUILD_TYPE IN_LIST BUILD_TYPES)
    message(FATAL_ERROR "CMAKE_BUILD_TYPE is ${CMAKE_BUILD_TYPE} and it must be one of ${BUILD_TYPES}")
  endif()

else()
  set(CMAKE_CONFIGURATION_TYPES ${BUILD_TYPES})
  set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING
    "Reset the configurations to what we need"
    FORCE)

  # Can't validate Multi Config because it should be done at generator time.
  # if(NOT $<CONFIG:cfgs> IN_LIST BUILD_TYPES)
  # message(FATAL_ERROR "CMAKE_CONFIGURATION_TYPES is $<CONFIG:cfgs> and it must be one of ${BUILD_TYPES}")
  # endif()
endif()