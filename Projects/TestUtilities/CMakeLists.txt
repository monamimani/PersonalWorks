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
  ${targetName}/BasicTests.h
)

set(moduleFiles 
)

set(privateFiles 
  ${targetName}/TestStruct.test.cpp
  ${targetName}/TestStruct.benchmark.cpp
  ${targetName}/BasicTests.cpp
)

set(testFiles 
  ${targetName}/BasicTests.test.cpp
)

set(benchmarkFiles 
)

add_library(${targetName} INTERFACE )

target_sources(${targetName}
  INTERFACE
    ${privateFiles}
    ${moduleFiles}
    ${publicFiles}
    ${interfaceFiles}
)

if(CMAKE_VERSION GREATER_EQUAL "3.26.0" )
  target_sources(${targetName}
    PRIVATE
      ${privateFiles}
    PUBLIC
     FILE_SET CXX_MODULES FILES ${moduleFiles}
    PUBLIC
     FILE_SET CXX_MODULE_HEADER_UNITS FILES ${headerUnitFiles}
    PUBLIC
      ${publicFiles}
    INTERFACE
      ${interfaceFiles}
  )
  target_sources(${targetName}
  INTERFACE
    ${privateFiles}
    ${moduleFiles}
    ${publicFiles}
    ${interfaceFiles}
   INTERFACE
    FILE_SET CXX_MODULES FILES ${moduleFiles}
   INTERFACE
    FILE_SET CXX_MODULE_HEADER_UNITS FILES ${headerUnitFiles}
)
else()
  target_sources(${targetName}
    INTERFACE
      ${privateFiles}
      ${moduleFiles}
      ${publicFiles}
      ${interfaceFiles}
      ${headerUnitFiles}
  )
endif()


target_include_directories(${targetName}
    INTERFACE
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