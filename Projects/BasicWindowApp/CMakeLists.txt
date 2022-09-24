# file(GLOB_RECURSE sources CONFIGURE_DEPENDS "*.cpp" "*.cxx" "*.ixx")
# file(GLOB_RECURSE headers CONFIGURE_DEPENDS "*.h" "*.hpp")

# Optionally glob, but only for CMake 3.12 or later: file(GLOB HEADER_LIST
# CONFIGURE_DEPENDS "${ModernCMakeExample_SOURCE_DIR}/include/modern/*.hpp")

get_filename_component(targetName ${CMAKE_CURRENT_LIST_DIR} NAME)

set(interfaceFiles 
)

set(publicFiles 
)

set(privateFiles 
)

set(testFiles 
  ${targetName}/BasicWindowApp.test.cpp
)


set(benchmarkFiles 
)

add_executable(${targetName}   ${targetName}/main.cpp)

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
    Core
    ApplicationCore
)

if(BUILD_TESTS)
  set(targetNameTests ${targetName}Tests )
  add_executable(${targetNameTests})

  target_sources(${targetNameTests}
    PRIVATE
      ${privateFiles}
      ${testFiles}
    PUBLIC
      ${publicFiles}
    INTERFACE
      ${interfaceFiles}
  )

  target_include_directories(${targetName}
      PUBLIC
          "${CMAKE_CURRENT_SOURCE_DIR}"
  )

  target_link_libraries(${targetNameTests}
  PRIVATE 
    GTest::gmock 
    GTest::gtest
    GTest::gtest_main
  )

  gtest_discover_tests(${targetNameTests}
    WORKING_DIRECTORY ${PROJECT_DIR}
    PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_DIR}"
    DISCOVERY_MODE PRE_TEST
  )

  set_target_properties(${targetNameTests} PROPERTIES FOLDER Tests)
endif()