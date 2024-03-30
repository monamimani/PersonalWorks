
get_filename_component(targetName ${CMAKE_CURRENT_LIST_DIR} NAME)

list(APPEND interfaceSources
)
list(APPEND publicSources
  ${targetName}/Config.h
  ${targetName}/ConfigGenerated.h.in
  ${targetName}/TestStruct.test.h
  ${targetName}/GoogleTest.h
  ${targetName}/GoogleBenchmark.h
  ${targetName}/TestStruct.benchmark.h
  ${targetName}/BasicTestsGenerator.h
  ${targetName}/TestsFriend.h
)
list(APPEND privateSources
  ${targetName}/TestStruct.test.cpp
  ${targetName}/TestStruct.benchmark.cpp
  ${targetName}/BasicTestsGenerator.cpp
  ${targetName}/TestsFriend.cpp
)
list(APPEND publicCxxModules
)
list(APPEND privateCxxModules
)
list(APPEND testsSources
  ${targetName}/BasicTests.test.cpp
)
list(APPEND benchamrksSources
)

list(APPEND interfaceLibraries )
list(APPEND publicLibraries )
list(APPEND privateLibraries
  PersonalWorks::Config
  GTest::gtest
  #GTest::gmock 
  benchmark::benchmark
  PersonalWorks::PersonalWorks_options
  PersonalWorks::PersonalWorks_warnings
  PersonalWorks::PersonalWorks_sanitizers
)

add_library(${targetName})
add_library(PersonalWorks::${targetName} ALIAS ${targetName})

configure_file(${targetName}/ConfigGenerated.h.in ${targetName}/ConfigGenerated.h)

generate_export_header(${targetName} EXPORT_FILE_NAME ${targetName}/${targetName}Exports.h)

if(NOT BUILD_SHARED_LIBS)
  string(TOUPPER ${targetName} targetNameUpper)
  target_compile_definitions(${targetName} PUBLIC ${targetNameUpper}_STATIC_DEFINE)
endif()


target_include_directories(${targetName}
  PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_BINARY_DIR}"
)

target_sources(${targetName}
  INTERFACE ${interfaceSources}
  PUBLIC ${publicSources}
  PRIVATE ${privateSources}
  PUBLIC FILE_SET publicCxxModules TYPE CXX_MODULES FILES ${publicCxxModules}
  PRIVATE FILE_SET privateCxxModules TYPE CXX_MODULES FILES ${privateCxxModules}
)

target_link_libraries(${targetName}
  INTERFACE ${interfaceLibraries}
  PUBLIC ${publicLibraries}
  PRIVATE ${privateLibraries}
)


if(BUILD_TESTING AND GTest_FOUND AND testsSources)
  set(targetNameTest ${targetName}Tests)
  add_executable(${targetNameTest})
  add_executable(PersonalWorks::${targetNameTest} ALIAS ${targetNameTest})
  set_target_properties(${targetNameTest} PROPERTIES FOLDER Tests)

  target_include_directories(${targetNameTest}
    PUBLIC
      "${CMAKE_CURRENT_SOURCE_DIR}"
      "${CMAKE_CURRENT_BINARY_DIR}"
  )

  target_sources(${targetNameTest}
    INTERFACE ${interfaceSources}
    PUBLIC ${publicSources}
    PRIVATE ${privateSources} ${testsSources}
    PUBLIC FILE_SET publicCxxModules TYPE CXX_MODULES FILES ${publicCxxModules}
    PRIVATE FILE_SET privateCxxModules TYPE CXX_MODULES FILES ${privateCxxModules}
  )

  # see https://github.com/google/googletest/issues/2157
  target_link_libraries(${targetNameTest}
    INTERFACE ${interfaceLibraries}
    PUBLIC ${publicLibraries}
    PRIVATE
      ${privateLibraries}
      fmt::fmt
      GTest::gtest_main
  )
  gtest_discover_tests(${targetNameTest})

  # Provide a simple smoke test to make sure that the CLI works and can display a --help message
  # add_test(NAME cli.has_help COMMAND SampleApp --help)

  # Provide a test to verify that the version being reported from the application
  # matches the version given to CMake. This will be important once you package
  # your program. Real world shows that this is the kind of simple mistake that is easy
  # to make, but also easy to test for.
  # add_test(NAME cli.version_matches COMMAND SampleApp --version)
  # set_tests_properties(cli.version_matches PROPERTIES PASS_REGULAR_EXPRESSION "${PROJECT_VERSION}")
endif()

if(BUILD_TESTING AND benchmark_FOUND AND benchamrksSources)
  set(targetNameBenchmark ${targetName}Benchmarks)
  add_executable(${targetNameBenchmark})
  add_executable(PersonalWorks::${targetNameBenchmark} ALIAS ${targetNameBenchmark})
  set_target_properties(${targetNameBenchmark} PROPERTIES FOLDER Benchmarks)

  target_include_directories(${targetNameBenchmark}
    PUBLIC
      "${CMAKE_CURRENT_SOURCE_DIR}"
      "${CMAKE_CURRENT_BINARY_DIR}"
  )

  target_sources(${targetNameBenchmark}
    INTERFACE ${interfaceSources}
    PUBLIC ${publicSources}
    PRIVATE ${privateSources} ${benchamrksSources}
    PUBLIC FILE_SET publicCxxModules TYPE CXX_MODULES FILES ${publicCxxModules}
    PRIVATE FILE_SET privateCxxModules TYPE CXX_MODULES FILES ${privateCxxModules}
  )

  target_link_libraries(${targetNameBenchmark}
    INTERFACE ${interfaceLibraries}
    PUBLIC ${publicLibraries}
    PRIVATE
      ${privateLibraries}
      fmt::fmt
      benchmark::benchmark_main
  )
endif()
