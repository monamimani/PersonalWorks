# file(GLOB_RECURSE sources CONFIGURE_DEPENDS "*.cpp" "*.cxx" "*.ixx")
# file(GLOB_RECURSE headers CONFIGURE_DEPENDS "*.h" "*.hpp")

# Optionally glob, but only for CMake 3.12 or later: file(GLOB HEADER_LIST
# CONFIGURE_DEPENDS "${ModernCMakeExample_SOURCE_DIR}/include/modern/*.hpp")

get_filename_component(targetName ${CMAKE_CURRENT_LIST_DIR} NAME)

set(interfaceFiles 
)

set(publicFiles 
  ${targetName}/BuildConfig/BuildConfig.hunit.h
  ${targetName}/BuildConfig/BuildConfigArchitecture.hunit.h
  ${targetName}/BuildConfig/BuildConfigCompiler.hunit.h
  ${targetName}/BuildConfig/BuildConfigPlatform.hunit.h
  ${targetName}/Delegates/DelegateStdFunction.h
  ${targetName}/DelegateTypes/EventDispatcher.h
  ${targetName}/DelegateTypes/EventQueue.h
  ${targetName}/Win32/StringUtils.h
  ${targetName}/Win32/Win32Utility.h
  ${targetName}/Win32/WindowsHeader.h
  ${targetName}/AppFrameTimer.h
  ${targetName}/Assert.h
  ${targetName}/Config.h
  ${targetName}/Endian.h
  ${targetName}/Expected.hunit.h
  ${targetName}/NonCopyable.h
  ${targetName}/PathUtils.h
  ${targetName}/Pimpl.h
)

set(privateFiles 
  ${targetName}/BuildConfig/BuildConfig.ixx
  ${targetName}/Delegates/Delegate.ixx
  ${targetName}/Delegates/DelegateMulticast.ixx
  ${targetName}/DelegateTypes/EventDispatcher.cpp
  ${targetName}/DelegateTypes/EventQueue.cpp
  ${targetName}/Win32/StringUtils.cpp
  ${targetName}/Core.ixx
  ${targetName}/CoreConcepts.ixx
  ${targetName}/CoreSingleton.ixx
  ${targetName}/CoreUtility.ixx
  ${targetName}/Endian.cpp
  ${targetName}/ErasedStorage.ixx
)

set(testFiles 
  ${targetName}/CoreMain.test.cpp
  ${targetName}/TestUtilities/TestStruct.test.cpp
  ${targetName}/TestUtilities/TestStruct.test.h
  ${targetName}/GoogleTest.h
  ${targetName}/BuildConfig/BuildConfig.test.cpp
  ${targetName}/Delegates/Delegate.test.cpp
  ${targetName}/Delegates/DelegateCommon.test.h
  ${targetName}/Delegates/DelegateMulticast.test.cpp
  ${targetName}/Delegates/DelegateMulticastCommon.test.h
  ${targetName}/DelegateTypes/EventDispatcher.test.cpp
  ${targetName}/DelegateTypes/EventQueue.test.cpp
  ${targetName}/ErasedStorage.test.cpp
  ${targetName}/Pimpl.test.cpp
)


set(benchmarkFiles 
  ${targetName}/TestUtilities/TestStruct.benchmark.cpp
  ${targetName}/TestUtilities/TestStruct.benchmark.h
  ${targetName}/GoogleBenchmark.h
  ${targetName}/Core.benchmark.cpp
  ${targetName}/Delegates/Delegate.benchmark.cpp
  ${targetName}/Delegates/DelegateCommon.benchmark.h
  ${targetName}/Delegates/DelegateCompare.benchmark.cpp
  ${targetName}/Delegates/DelegateMulticast.benchmark.cpp
  ${targetName}/ErasedStorage.benchmark.cpp
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