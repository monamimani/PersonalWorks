 # file(GLOB_RECURSE sources CONFIGURE_DEPENDS "*.cpp" "*.cxx" "*.ixx")
# file(GLOB_RECURSE headers CONFIGURE_DEPENDS "*.h" "*.hpp")

# Optionally glob, but only for CMake 3.12 or later: file(GLOB HEADER_LIST
# CONFIGURE_DEPENDS "${ModernCMakeExample_SOURCE_DIR}/include/modern/*.hpp")

get_filename_component(targetName ${CMAKE_CURRENT_LIST_DIR} NAME)

if(CMAKE_SYSTEM_NAME STREQUAL Windows)

  set(platform_publicFiles 
  )

  set(platform_privateFiles 
  )

elseif (CMAKE_SYSTEM_NAME STREQUAL Linux)

  set(platform_publicFiles 
  )

  set(platform_privateFiles 
  )

endif()

set(interfaceFiles 
)

set(publicFiles 
  ${platform_publicFiles}
)

set(privateFiles 
  ${platform_privateFiles}
)

set(testFiles 
)


set(benchmarkFiles 
)

add_library(${targetName})

target_sources(${targetName}
  PRIVATE
    ${privateFiles}
  PUBLIC
    ${publicFiles}
  INTERFACE
    ${interfaceFiles}
)

target_include_directories(${targetName}
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(${targetName}
  PRIVATE
  PUBLIC
)

#source_group(
#  TREE "${PROJECT_SOURCE_DIR}/include"
#  PREFIX "Header Files"
#  FILES ${HEADER_LIST})

if(BUILD_TESTS)
  set(targetNameTests ${targetName}Tests )
  add_executable(${targetNameTests} 
    ${testFiles}
  )
  set_target_properties(${targetNameTests} PROPERTIES FOLDER Tests)

  target_link_libraries(${targetNameTests}
  PRIVATE 
    ${targetName}
    GTest::gmock 
    GTest::gtest
  )

  gtest_discover_tests(${targetNameTests}
    WORKING_DIRECTORY ${PROJECT_DIR}
    PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_DIR}"
    DISCOVERY_MODE PRE_TEST
  )


if(RUN_TESTS_AFTER_BUILD)
  add_custom_command(TARGET ${targetNameTests}
    # Run after all other rules within the target have been executed
    POST_BUILD
    COMMAND ${targetNameTests}
    COMMENT "Running Google Test"
    VERBATIM
  )
endif()

endif()