include_guard()

include(CMakeDependentOption)

cmake_dependent_option(PersonalWorks_ENABLE_UNITY_BUILD "Enable unity builds" OFF "NOT PersonalWorks_BASIC_BUILD_MODE" OFF)

function(add_unity_build_to_target TARGET_NAME)

  set_target_properties(${TARGET_NAME} PROPERTIES UNITY_BUILD ${PersonalWorks_ENABLE_UNITY_BUILD})

endfunction()
