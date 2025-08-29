include_guard()

# Allowed values are "none", "address",
set(BUILD_SANITIZER_LIST "none" "address")
set(BUILD_SANITIZERS "none" CACHE STRING "The kind of configuration for the build.")
set_property(CACHE BUILD_SANITIZERS PROPERTY STRINGS ${BUILD_SANITIZER_LIST})
set(ENV{BUILD_SANITIZERS} "${BUILD_SANITIZERS}")

string(REPLACE "," ";" SANITIZERS_LIST "${BUILD_SANITIZERS}")
foreach(SANITIZER ${BUILD_SANITIZERS})
  if(NOT SANITIZER IN_LIST BUILD_SANITIZER_LIST)
    message(FATAL_ERROR "Invalid sanitizer '${SANITIZER}' specified in BUILD_SANITIZERS. Allowed values are: ${BUILD_SANITIZER_LIST}")
  endif()
endforeach()

set(SANITIZER_FLAGS "")

if("address" IN_LIST BUILD_SANITIZERS)
   string(FIND "$ENV{PATH}" "$ENV{VSINSTALLDIR}" index_of_vs_install_dir)
  if("${index_of_vs_install_dir}" STREQUAL "-1")
    message(
      FATAL_ERROR
      "Using MSVC sanitizers requires setting the MSVC environment before building the project. Please manually open the MSVC command prompt and rebuild the project."
    )
  endif()

  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "ProgramDatabase")
  string(APPEND SANITIZER_FLAGS " /fsanitize=address /Zi /INCREMENTAL:NO")
endif()
