
get_filename_component(targetName ${CMAKE_CURRENT_LIST_DIR} NAME)

set(BUILD_KIND_IS_DEBUG 0)
set(BUILD_KIND_IS_RELEASE 0)
set(BUILD_KIND_IS_SHIPPING 0)

if(BUILD_KIND STREQUAL DEBUG)
set(BUILD_KIND_IS_DEBUG 1)
elseif(BUILD_KIND STREQUAL RELEASE)
set(BUILD_KIND_IS_RELEASE 1)
elseif(BUILD_KIND STREQUAL SHIPPING)
set(BUILD_KIND_IS_SHIPPING 1)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL Windows)
  list(APPEND platformInterfaceSources
  )
  list(APPEND platformPublicSources 
    ${targetName}/Win32/StringUtils.h
    ${targetName}/Win32/Win32Utility.h
    ${targetName}/Win32/WindowsHeader.h
  )
  list(APPEND platformPrivateSources 
    ${targetName}/Win32/StringUtils.cpp
  )
  list(APPEND platformPublicCxxModules
  )
  list(APPEND platformPrivateCxxModules
  )

elseif(CMAKE_SYSTEM_NAME STREQUAL Linux)
  list(APPEND platformInterfaceSources
  )
  list(APPEND platformPublicSources 
  )
  list(APPEND platformPrivateSources 
  )
  list(APPEND platformPublicCxxModules
  )
  list(APPEND platformPrivateCxxModules
  )

endif()

list(APPEND interfaceSources
  ${platformInterfaceSources}
)
list(APPEND publicSources
  ${targetName}/DelegateTypes/EventDispatcher.h
  ${targetName}/DelegateTypes/EventQueue.h
  ${targetName}/AppFrameTimer.h
  ${targetName}/Assert.h
  ${targetName}/Config.h
  ${targetName}/ConfigGenerated.h.in
  ${targetName}/Endian.h
  ${targetName}/EnumClassFlags.h
  ${targetName}/NonCopyable.h
  ${targetName}/PathUtils.h
  ${targetName}/Pimpl.h
  ${targetName}/Overloaded.h
  ${targetName}/Exception.h
  ${targetName}/BuildConfig/BuildConfig.hunit.h
  ${targetName}/BuildConfig/BuildConfigArchitecture.hunit.h
  ${targetName}/BuildConfig/BuildConfigCompiler.hunit.h
  ${targetName}/BuildConfig/BuildConfigPlatform.hunit.h
  ${platformPublicSources}
)
list(APPEND privateSources
  ${targetName}/DelegateTypes/EventDispatcher.cpp
  ${targetName}/DelegateTypes/EventQueue.cpp
  ${targetName}/Endian.cpp
  ${platformPrivateSources}
)
list(APPEND publicCxxModules
  ${targetName}/BuildConfig/BuildConfig.ixx
  ${targetName}/Delegates/Delegate.ixx
  ${targetName}/Delegates/DelegateMulticast.ixx
  ${targetName}/Core.ixx
  ${targetName}/CoreConcepts.ixx
  ${targetName}/CoreSingleton.ixx
  ${targetName}/CoreUtility.ixx
  ${targetName}/ErasedStorage.ixx
  ${targetName}/StaticFunction.ixx
  ${platformPublicCxxModules}
)
list(APPEND privateCxxModules
  ${platformPrivateCxxModules}
)
list(APPEND testsSources
  ${targetName}/BuildConfig/BuildConfig.test.cpp
  ${targetName}/StaticFunction.test.cpp
  ${targetName}/Delegates/Delegate.test.cpp
  ${targetName}/Delegates/DelegateCommon.test.h
  ${targetName}/Delegates/DelegateMulticast.test.cpp
  ${targetName}/DelegateTypes/EventDispatcher.test.cpp
  ${targetName}/DelegateTypes/EventQueue.test.cpp
  ${targetName}/ErasedStorage.test.cpp
  ${targetName}/Pimpl.test.cpp
  ${targetName}/EnumClassFlags.test.cpp
  ${targetName}/Exception.test.cpp
)
list(APPEND benchamrksSources
  ${targetName}/Delegates/DelegateStdFunction.h
  ${targetName}/Core.benchmark.cpp
  ${targetName}/Delegates/Delegate.benchmark.cpp
  ${targetName}/Delegates/DelegateCommon.benchmark.h
  ${targetName}/Delegates/DelegateCompare.benchmark.cpp
  ${targetName}/Delegates/DelegateMulticast.benchmark.cpp
  ${targetName}/ErasedStorage.benchmark.cpp
  ${targetName}/StaticFunction.benchmark.cpp
)

list(APPEND interfaceLibraries )
list(APPEND publicLibraries )
list(APPEND privateLibraries
  PersonalWorks::PersonalWorks_options
  PersonalWorks::PersonalWorks_warnings
  PersonalWorks::PersonalWorks_sanitizers
  PersonalWorks::Config
  PersonalWorks::TestUtilities
  fmt::fmt
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
  )

  target_sources(${targetNameTest}
    INTERFACE ${interfaceSources}
    PUBLIC ${publicSources}
    PRIVATE ${privateSources} ${testsSources} CoreMain.test.cpp
    PUBLIC FILE_SET publicCxxModules TYPE CXX_MODULES FILES ${publicCxxModules}
    PRIVATE FILE_SET privateCxxModules TYPE CXX_MODULES FILES ${privateCxxModules}
  )

  # see https://github.com/google/googletest/issues/2157
  target_link_libraries(${targetNameTest}
    INTERFACE ${interfaceLibraries}
    PUBLIC ${publicLibraries}
    PRIVATE
      ${privateLibraries}
      PersonalWorks::TestUtilities
      fmt::fmt
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
  )

  target_sources(${targetNameBenchmark}
    INTERFACE ${interfaceSources}
    PUBLIC ${publicSources}
    PRIVATE ${privateSources} ${benchamrksSources} CoreMain.benchmark.cpp
    PUBLIC FILE_SET publicCxxModules TYPE CXX_MODULES FILES ${publicCxxModules}
    PRIVATE FILE_SET privateCxxModules TYPE CXX_MODULES FILES ${privateCxxModules}
  )

  target_link_libraries(${targetNameBenchmark}
    INTERFACE ${interfaceLibraries}
    PUBLIC ${publicLibraries}
    PRIVATE
      ${privateLibraries}
      PersonalWorks::TestUtilities
      fmt::fmt
  )
endif()
