# file(GLOB_RECURSE sources CONFIGURE_DEPENDS "*.cpp" "*.cxx" "*.ixx")
# file(GLOB_RECURSE headers CONFIGURE_DEPENDS "*.h" "*.hpp")

# Optionally glob, but only for CMake 3.12 or later: file(GLOB HEADER_LIST
# CONFIGURE_DEPENDS "${ModernCMakeExample_SOURCE_DIR}/include/modern/*.hpp")

get_filename_component(targetName ${CMAKE_CURRENT_LIST_DIR} NAME)


set(interfaceFiles 
)

set(headerUnitFiles 
)

set(publicFiles 
  ${targetName}/TestStruct.test.h
  ${targetName}/GoogleTest.h
  ${targetName}/GoogleBenchmark.h
  ${targetName}/TestStruct.benchmark.h
  ${targetName}/BasicTestsGenerator.h
  ${targetName}/BasicTestsFriend.h
)

set(publicModuleFiles 
)

set(privateFiles 
  ${targetName}/TestStruct.test.cpp
  ${targetName}/TestStruct.benchmark.cpp
  ${targetName}/BasicTestsGenerator.cpp
)

set(testFiles 
  ${targetName}/BasicTests.test.cpp
)

set(benchmarkFiles 
)

add_library(${targetName})

if(CMAKE_VERSION GREATER_EQUAL "3.26.0" )
  target_sources(${targetName}
    PRIVATE
      ${privateFiles}
    PUBLIC
     FILE_SET CXX_MODULES FILES ${publicModuleFiles}
    #PUBLIC
     #FILE_SET CXX_MODULE_HEADER_UNITS FILES ${headerUnitFiles}
    PUBLIC
      ${publicFiles}
    INTERFACE
      ${interfaceFiles}
  )
else()
  target_sources(${targetName}
    INTERFACE
      ${privateFiles}
      ${publicModuleFiles}
      ${publicFiles}
      ${interfaceFiles}
      ${headerUnitFiles}
  )
endif()


target_include_directories(${targetName}
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(${targetName}
  PRIVATE
  PUBLIC
  INTERFACE
    GTest::gmock 
    GTest::gtest
    benchmark::benchmark
)


if(BUILD_TESTS)
 add_test_target(
   TARGET ${targetName} 
   TESTFILES ${testFiles}
   PRIVATEFILES ${privateFiles}
   MODULEFILES ${moduleFiles}
   HEADERUNITFILES ${headerUnitFiles}
   PUBLICFILES ${publicFiles}
   INTERFACEFILES ${interfaceFiles})
endif()

if(BUILD_BENCHMARKS)
endif()