 # file(GLOB_RECURSE sources CONFIGURE_DEPENDS "*.cpp" "*.cxx" "*.ixx")
# file(GLOB_RECURSE headers CONFIGURE_DEPENDS "*.h" "*.hpp")

# Optionally glob, but only for CMake 3.12 or later: file(GLOB HEADER_LIST
# CONFIGURE_DEPENDS "${ModernCMakeExample_SOURCE_DIR}/include/modern/*.hpp")

get_filename_component(targetName ${CMAKE_CURRENT_LIST_DIR} NAME)


set(interfaceFiles 
)

set(publicFiles 
  ${targetName}/TestStruct.test.h
  ${targetName}/GoogleTest.h
  ${targetName}/GoogleBenchmark.h
  ${targetName}/TestStruct.benchmark.h
)

set(privateFiles 
  ${targetName}/TestStruct.test.cpp
  ${targetName}/TestStruct.benchmark.cpp
)

set(testFiles 
)


set(benchmarkFiles 
)

add_library(${targetName} INTERFACE )

target_sources(${targetName}
  INTERFACE
    ${privateFiles}
    ${publicFiles}
    ${interfaceFiles}
)

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
endif()

if(BUILD_BENCHMARKS)
endif()