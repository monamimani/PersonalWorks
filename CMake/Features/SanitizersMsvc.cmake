include_guard()

# Allowed values are "none", "address", 
set(BUILD_SANITIZER_LIST "none" "address")
set(BUILD_SANITIZER "none" CACHE STRING "The kind of configuration for the build.")
set_property(CACHE BUILD_SANITIZER PROPERTY STRINGS ${BUILD_SANITIZER_LIST})
set(ENV{BUILD_SANITIZER} "${BUILD_SANITIZER}")

if(NOT BUILD_SANITIZER IN_LIST BUILD_SANITIZER_LIST)
  message(FATAL_ERROR "BUILD_SANITIZER is ${BUILD_SANITIZER} and it must be one of ${BUILD_SANITIZER_LIST}")
endif()

set(SANITIZER_FLAGS "")

if(address IN_LIST BUILD_SANITIZER)
   string(FIND "$ENV{PATH}" "$ENV{VSINSTALLDIR}" index_of_vs_install_dir)
  if("${index_of_vs_install_dir}" STREQUAL "-1")
    message(
      FATAL_ERROR
      "Using MSVC sanitizers requires setting the MSVC environment before building the project. Please manually open the MSVC command prompt and rebuild the project."
    )
  endif()

  string(APPEND SANITIZER_FLAGS " /fsanitize=address /Zi")
endif()

#option(BUILD_SANITIZER_ADDRESS "Enable Address Sanitizer" OFF)
#option(BUILD_SANITIZER_UNDEFINED "Enable Undefined Behavior Sanitizer" OFF)
#option(BUILD_SANITIZER_THREAD "Enable Thread Sanitizer" OFF)
#option(BUILD_SANITIZER_MEMORY "Enable Memory Sanitizer" OFF)
#option(BUILD_SANITIZER_LEAK "Enable Leak Sanitizer" OFF)


#set(SANITIZER_FLAGS "")

#if(BUILD_SANITIZER)
#  string(FIND "$ENV{PATH}" "$ENV{VSINSTALLDIR}" index_of_vs_install_dir)
#  if("${index_of_vs_install_dir}" STREQUAL "-1")
#    message(
#      FATAL_ERROR
#      "Using MSVC sanitizers requires setting the MSVC environment before building the project. Please manually open the MSVC command prompt and rebuild the project."
#    )
#  endif()
#
#  string(APPEND SANITIZER_FLAGS " /fsanitize=address /Zi")
#
#endif()
#
#if(BUILD_SANITIZER_LEAK OR BUILD_SANITIZER_UNDEFINED OR BUILD_SANITIZER_THREAD OR BUILD_SANITIZER_MEMORY)
#  message(WARNING "MSVC only supports address sanitizer")
#endif()
