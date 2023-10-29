 # file(GLOB_RECURSE sources CONFIGURE_DEPENDS "*.cpp" "*.cxx" "*.ixx")
# file(GLOB_RECURSE headers CONFIGURE_DEPENDS "*.h" "*.hpp")

# Optionally glob, but only for CMake 3.12 or later: file(GLOB HEADER_LIST
# CONFIGURE_DEPENDS "${ModernCMakeExample_SOURCE_DIR}/include/modern/*.hpp")


get_filename_component(targetName ${CMAKE_CURRENT_LIST_DIR} NAME)

if(CMAKE_SYSTEM_NAME STREQUAL Windows)

  set(platform_publicFiles 
  ${targetName}/Win32/StringUtils.h
  ${targetName}/Win32/Win32Utility.h
  ${targetName}/Win32/WindowsHeader.h
)

set(platform_privateFiles 
  ${targetName}/Win32/StringUtils.cpp
)

elseif (CMAKE_SYSTEM_NAME STREQUAL Linux)

  set(platform_publicFiles 
)

set(platform_privateFiles 
)

endif()

set(interfaceFiles 
)

set(headerUnitFiles 
)

set(publicFiles 
  ${targetName}/DelegateTypes/EventDispatcher.h
  ${targetName}/DelegateTypes/EventQueue.h
  ${targetName}/AppFrameTimer.h
  ${targetName}/Assert.h
  ${targetName}/Config.h
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
  ${platform_publicFiles}
)

set(moduleFiles 
  ${targetName}/BuildConfig/BuildConfig.ixx
  ${targetName}/Delegates/Delegate.ixx
  ${targetName}/Delegates/DelegateMulticast.ixx
  ${targetName}/Core.ixx
  ${targetName}/CoreConcepts.ixx
  ${targetName}/CoreSingleton.ixx
  ${targetName}/CoreUtility.ixx
  ${targetName}/ErasedStorage.ixx
  ${targetName}/StaticFunction.ixx
)

set(privateFiles 
  ${targetName}/DelegateTypes/EventDispatcher.cpp
  ${targetName}/DelegateTypes/EventQueue.cpp
  ${targetName}/Endian.cpp
  ${platform_privateFiles}
)

set(testFiles 
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
)

set(benchmarkFiles 
  ${targetName}/Delegates/DelegateStdFunction.h
  ${targetName}/Core.benchmark.cpp
  ${targetName}/Delegates/Delegate.benchmark.cpp
  ${targetName}/Delegates/DelegateCommon.benchmark.h
  ${targetName}/Delegates/DelegateCompare.benchmark.cpp
  ${targetName}/Delegates/DelegateMulticast.benchmark.cpp
  ${targetName}/ErasedStorage.benchmark.cpp
  ${targetName}/StaticFunction.benchmark.cpp
)

add_library(${targetName})

if(CMAKE_VERSION GREATER_EQUAL "3.26.0" )
  target_sources(${targetName}
    PRIVATE
      ${privateFiles}
    PUBLIC
     FILE_SET CXX_MODULES FILES ${moduleFiles}
    #PUBLIC
     #FILE_SET CXX_MODULE_HEADER_UNITS FILES ${headerUnitFiles}
    PUBLIC
      ${publicFiles}
    INTERFACE
      ${interfaceFiles}
  )
else()
  target_sources(${targetName}
    PRIVATE
      ${privateFiles}
    PRIVATE
      ${moduleFiles}
    PUBLIC
      ${publicFiles}
    PUBLIC
      ${headerUnitFiles}
    INTERFACE
      ${interfaceFiles}
  )
endif()

target_include_directories(${targetName}
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(${targetName}
  PRIVATE
    TestUtilities
  PUBLIC
)

if(BUILD_TESTS)
 add_test_target(
   TARGET ${targetName} 
   TESTFILES ${testFiles}
   PRIVATEFILES ${privateFiles}
   MODULEFILES ${moduleFiles}
   HEADERUNITFILES ${headerUnitFiles}
   PUBLICFILES ${publicFiles}
   INTERFACEFILES ${interfaceFiles}
   MAINFILE CoreMain.test.cpp)

  #set_target_properties(${targetName}Tests PROPERTIES VS_DEBUGGER_COMMAND_ARGUMENTS "--gtest_filter=ErasedStorageTestStructF*")
endif()

if(BUILD_BENCHMARKS)
  add_benchmark_target(
    TARGET ${targetName} 
    BENCHMARKFILES ${benchmarkFiles}
    PRIVATEFILES ${privateFiles}
    MODULEFILES ${moduleFiles}
    HEADERUNITFILES ${headerUnitFiles}
    PUBLICFILES ${publicFiles}
    INTERFACEFILES ${interfaceFiles}
    MAINFILE CoreMain.benchmark.cpp)
endif()