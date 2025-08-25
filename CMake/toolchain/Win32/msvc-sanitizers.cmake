include_guard()

if(BUILD_SANITIZER_ADDRESS)
  string(FIND "$ENV{PATH}" "$ENV{VSINSTALLDIR}" index_of_vs_install_dir)
  if("${index_of_vs_install_dir}" STREQUAL "-1")
    message(
      SEND_ERROR
      "Using MSVC sanitizers requires setting the MSVC environment before building the project. Please manually open the MSVC command prompt and rebuild the project."
    )
  endif()

  string(APPEND DEFAULT_C_FLAGS " /fsanitize=address /Zi")
  string(APPEND DEFAULT_CXX_FLAGS " /fsanitize=address /Zi")

endif()

if(BUILD_SANITIZER_LEAK OR BUILD_SANITIZER_UNDEFINED OR BUILD_SANITIZER_THREAD OR BUILD_SANITIZER_MEMORY)
  message(WARNING "MSVC only supports address sanitizer")
endif()
