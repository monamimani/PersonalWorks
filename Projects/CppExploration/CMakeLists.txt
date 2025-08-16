
project(CppExploration
  LANGUAGES CXX C)

get_filename_component(targetName ${CMAKE_CURRENT_LIST_DIR} NAME)

list(APPEND interfaceSources
)
list(APPEND publicSources
)
list(APPEND privateSources
  StaticDispatch.cpp
)
list(APPEND publicCxxModules
)
list(APPEND privateCxxModules
)
list(APPEND testsSources
  LambdaIdioms.cpp
  CoroutineNigeltao.cpp
  CommonFilesystemTasks.cpp
)
list(APPEND benchamrksSources
)

list(APPEND interfaceLibraries )
list(APPEND publicLibraries )
list(APPEND privateLibraries
  PersonalWorks::Config
  PersonalWorks::Core
  PersonalWorks::PersonalWorks_options
  PersonalWorks::PersonalWorks_warnings
  PersonalWorks::PersonalWorks_sanitizers
)

#if((NOT GTest_FOUND) OR (NOT benchmark_FOUND))
#  message(FATAL_ERROR "Testing framework not found")
#endif()

if(PersonalWorks_BUILD_TESTS)
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
    Catch2::Catch2WithMain
)
catch_discover_tests(${targetNameTest})
endif()
# Provide a simple smoke test to make sure that the CLI works and can display a --help message
# add_test(NAME cli.has_help COMMAND SampleApp --help)

# Provide a test to verify that the version being reported from the application
# matches the version given to CMake. This will be important once you package
# your program. Real world shows that this is the kind of simple mistake that is easy
# to make, but also easy to test for.
# add_test(NAME cli.version_matches COMMAND SampleApp --version)
# set_tests_properties(cli.version_matches PROPERTIES PASS_REGULAR_EXPRESSION "${PROJECT_VERSION}")


#if(${benchamrksSources})
#  set(targetNameBenchmark ${targetName}Benchmarks)
#  add_executable(${targetNameBenchmark})
#  add_executable(PersonalWorks::${targetNameBenchmark} ALIAS ${targetNameBenchmark})
#  set_target_properties(${targetNameBenchmark} PROPERTIES FOLDER Benchmarks)
#
#  target_include_directories(${targetNameBenchmark}
#    PUBLIC
#      "${CMAKE_CURRENT_SOURCE_DIR}"
#      "${CMAKE_CURRENT_BINARY_DIR}"
#  )
#
#  target_sources(${targetNameBenchmark}
#    INTERFACE ${interfaceSources}
#    PUBLIC ${publicSources}
#    PRIVATE ${privateSources} ${benchamrksSources}
#    PUBLIC FILE_SET publicCxxModules TYPE CXX_MODULES FILES ${publicCxxModules}
#    PRIVATE FILE_SET privateCxxModules TYPE CXX_MODULES FILES ${privateCxxModules}
#  )
#
#  target_link_libraries(${targetNameBenchmark}
#    INTERFACE ${interfaceLibraries}
#    PUBLIC ${publicLibraries}
#    PRIVATE
#      ${privateLibraries}
#      Catch2::Catch2WithMain
#  )
#endif()
