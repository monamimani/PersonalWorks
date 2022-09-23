
#add_executable(main.test.cpp)

#file(GLOB_RECURSE sources CONFIGURE_DEPENDS "*.test.cpp" "*.test.cxx" "*.test.ixx")
# file(GLOB_RECURSE headers CONFIGURE_DEPENDS "*.h" "*.hpp")

#target_link_libraries(App
#    PRIVATE 
#        main.test.cpp
#        GTest::gmock 
#        GTest::gtest
#        benchmark::benchmark
#)

#gtest_discover_tests(App)


add_library(TestsUtilities)

target_sources(TestsUtilities
  PRIVATE
    TestUtilities/TestStruct.benchmark.cpp
    TestUtilities/TestStruct.test.cpp
  PUBLIC
    TestUtilities/TestStruct.benchmark.h
    TestUtilities/TestStruct.test.h
    GoogleBenchmark.h
    GoogleTest.h
)

target_include_directories(TestsUtilities
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(TestsUtilities
    PUBLIC 
        GTest::gmock 
        GTest::gtest
        benchmark::benchmark
)
