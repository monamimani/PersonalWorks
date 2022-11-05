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

set(publicFiles 
  ${targetName}/BuildConfig/BuildConfig.hunit.h
  ${targetName}/BuildConfig/BuildConfigArchitecture.hunit.h
  ${targetName}/BuildConfig/BuildConfigCompiler.hunit.h
  ${targetName}/BuildConfig/BuildConfigPlatform.hunit.h
  ${targetName}/Delegates/DelegateStdFunction.h
  ${targetName}/DelegateTypes/EventDispatcher.h
  ${targetName}/DelegateTypes/EventQueue.h
  ${targetName}/AppFrameTimer.h
  ${targetName}/Assert.h
  ${targetName}/Config.h
  ${targetName}/Endian.h
  ${targetName}/Expected.hunit.h
  ${targetName}/NonCopyable.h
  ${targetName}/PathUtils.h
  ${targetName}/Pimpl.h
  ${platform_publicFiles}
)

set(privateFiles 
  ${targetName}/BuildConfig/BuildConfig.ixx
  ${targetName}/Delegates/Delegate.ixx
  ${targetName}/Delegates/DelegateMulticast.ixx
  ${targetName}/DelegateTypes/EventDispatcher.cpp
  ${targetName}/DelegateTypes/EventQueue.cpp
  ${targetName}/Core.ixx
  ${targetName}/CoreConcepts.ixx
  ${targetName}/CoreSingleton.ixx
  ${targetName}/CoreUtility.ixx
  ${targetName}/Endian.cpp
  ${targetName}/ErasedStorage.ixx
  ${platform_privateFiles}
)

set(testFiles 
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

if(BUILD_TESTS)
  add_test_target(
    TARGET ${targetName} 
    TESTFILES ${testFiles}
    PRIVATEFILES ${privateFiles}
    PUBLICFILES ${publicFiles}
    INTERFACEFILES ${interfaceFiles}
    MAINFILE CoreMain.test.cpp)
endif()

if(BUILD_BENCHMARKS)
  add_benchmark_target(
    TARGET ${targetName} 
    BENCHMARKFILES ${benchmarkFiles}
    PRIVATEFILES ${privateFiles}
    PUBLICFILES ${publicFiles}
    INTERFACEFILES ${interfaceFiles}
    MAINFILE CoreMain.benchmark.cpp)
endif()