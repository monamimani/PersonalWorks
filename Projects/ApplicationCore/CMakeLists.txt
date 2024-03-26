
get_filename_component(targetName ${CMAKE_CURRENT_LIST_DIR} NAME)

if(CMAKE_SYSTEM_NAME STREQUAL Windows)
  list(APPEND platformInterfaceSources
  )
  list(APPEND platformPublicSources 
    ${targetName}/Application/Win32/Win32Application.h
    ${targetName}/Application/Win32/Win32ApplicationDesc.h
    ${targetName}/Application/Win32/Win32ApplicationMainLoop.h
    ${targetName}/Application/Win32/Win32MessageLoop.h
    ${targetName}/Input/Win32/Win32Input.h
    ${targetName}/Window/Win32/Win32EventQueue.h
    ${targetName}/Window/Win32/Win32Window.h
    ${targetName}/Window/WinRT/WinRTWindow.h
  )
  list(APPEND platformPrivateSources  
    ${targetName}/Application/Win32/Win32Application.cpp
    ${targetName}/Application/Win32/Win32ApplicationMainLoop.cpp
    ${targetName}/Application/Win32/Win32MessageLoop.cpp
    ${targetName}/Input/Win32/Win32Input.cpp
    ${targetName}/Window/Win32/Win32EventQueue.cpp
    ${targetName}/Window/Win32/Win32Window.cpp
    ${targetName}/Window/WinRT/WinRTWindow.cpp
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
  ${targetName}/Application/Application.h
  ${targetName}/Application/ApplicationEventMediator.h
  ${targetName}/Input/Keyboard.h
  ${targetName}/Input/Mouse.h
  ${targetName}/Window/Event.h
  ${targetName}/Window/Window.h
  ${targetName}/Window/WindowDesc.h
  ${targetName}/Window/WindowImpl.h
  ${platformPublicSources}
)
list(APPEND privateSources
  ${targetName}/Application/Application.cpp
  ${targetName}/Application/ApplicationEventMediator.cpp
  ${targetName}/Input/Keyboard.cpp
  ${targetName}/Input/Mouse.cpp
  ${targetName}/Window/Event.cpp
  ${targetName}/Window/Window.cpp
  ${targetName}/Window/WindowImpl.cpp
  ${platformPrivateSources}
)
list(APPEND publicCxxModules
    ${platformPublicCxxModules}
)
list(APPEND privateCxxModules
  ${platformPrivateCxxModules}
)
list(APPEND testsSources
  ${targetName}/Application/Application.test.cpp

)
list(APPEND benchamrksSources
)

list(APPEND interfaceLibraries )
list(APPEND publicLibraries )
list(APPEND privateLibraries
  PersonalWorks::PersonalWorks_options
  PersonalWorks::PersonalWorks_warnings
  PersonalWorks::PersonalWorks_sanitizers
  PersonalWorks::Config
  PersonalWorks::Core
  fmt::fmt
)


add_library(${targetName})
add_library(PersonalWorks::${targetName} ALIAS ${targetName})

target_include_directories(${targetName}
  PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}"
)

generate_export_header(${targetName} EXPORT_FILE_NAME ${targetName}/${targetName}Exports.h)

if(NOT BUILD_SHARED_LIBS)
  string(TOUPPER ${targetName} targetNameUpper)
  target_compile_definitions(${targetName} PUBLIC ${targetNameUpper}_STATIC_DEFINE)
endif()

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
      PersonalWorks::TestUtilities
      fmt::fmt
      GTest::gtest_main
  )
  gtest_discover_tests(${targetNameTest})

  #set_target_properties(${targetName}Tests PROPERTIES VS_DEBUGGER_COMMAND_ARGUMENTS "--gtest_filter=ErasedStorageTestStructF*")


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
    PRIVATE ${privateSources} ${benchamrksSources}
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
      benchmark::benchmark_main
  )
endif()

