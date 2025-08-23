include_guard()

include(CMake/Features/UnityBuild.cmake)
include(CMake/Features/CodeHardening.cmake)

function(add_target_project_options TARGET_NAME)
  set(options)
  set(oneValueArgs)
  set(multiValueArgs)

  cmake_parse_arguments(PARSE_ARGV 1 ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" )

  if(ARG_PREFIX_UNPARSED_ARGUMENTS)
    message(FATAL_ERROR "Bad arguments: ${ARG_PREFIX_UNPARSED_ARGUMENTS}")
  endif()

  if(ARG_KEYWORDS_MISSING_VALUES)
    message(FATAL_ERROR "Missing values for keywords: ${ARG_KEYWORDS_MISSING_VALUES}")
  endif()

  if(NOT TARGET ${TARGET_NAME})
    message(FATAL_ERROR "Target ${TARGET_NAME} does not exist. Please create the target before calling this function.")
  endif()


  target_compile_features(${TARGET_NAME} INTERFACE cxx_std_${CMAKE_CXX_STANDARD})

  if(PersonalWorks_ENABLE_UNITY_BUILD)
    add_unity_build_to_target(${TARGET_NAME})
  endif()

  if(PersonalWorks_ENABLE_TARGET_CODE_HARDENING)
    add_target_code_hardening(${TARGET_NAME})
  endif()

endfunction()
