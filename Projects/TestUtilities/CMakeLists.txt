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

set(moduleFiles 
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

target_include_directories(${targetName}
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}"
)

add_target_sources(${targetName} 
   PRIVATEFILES ${privateFiles}
   MODULEFILES ${moduleFiles}
   PUBLICFILES ${publicFiles}
   HEADERUNITFILES ${headerUnitFiles}
   INTERFACEFILES ${interfaceFiles}
   TESTFILES ${testFiles}
   BENCHMARKFILES ${benchmarkFiles}
   )

set(interfaceLibs
  GTest::gmock 
  GTest::gtest
  benchmark::benchmark
)

add_target_link_libraries(${targetName}
  INTERFACELIBS ${interfaceLibs}
)