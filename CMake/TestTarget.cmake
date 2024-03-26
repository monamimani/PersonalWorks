include_guard()


function(add_test_target TARGETNAME)
  set(options)
  set(oneValueArgs MAINFILE)
  set(multiValueArgs SOURCEFILES MODULEFILES)

  cmake_parse_arguments(PARSE_ARGV 1 ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" )

  #message(STATUS "function=${CMAKE_CURRENT_FUNCTION}")
  #message(STATUS "ARGV=${ARGV}")
  #message(STATUS "ARGN=${ARGN}")
  #cmake_print_variables(TARGETNAME)
  #cmake_print_variables(ARG_MAINFILE)
  #cmake_print_variables(ARG_SOURCEFILES)
  #cmake_print_variables(ARG_MODULEFILES)

  #cmake_print_variables(ARG_PREFIX_UNPARSED_ARGUMENTS)
  #cmake_print_variables(ARG_KEYWORDS_MISSING_VALUES)


if(NOT BUILD_TESTING OR NOT GTest_FOUND)
  return()
endif()

get_target_property("${TARGETNAME}Include" ${TARGETNAME} INCLUDE_DIRECTORIES)
get_target_property("${TARGETNAME}Sources" ${TARGETNAME} SOURCES)
get_target_property("${TARGETNAME}ModuleSets" ${TARGETNAME} CXX_MODULE_SETS)
get_target_property("${TARGETNAME}DefaultModuleSet" ${TARGETNAME} CXX_MODULE_SET)
get_target_property("${TARGETNAME}Libraires" ${TARGETNAME} LINK_LIBRARIES)


cmake_print_variables(${TARGETNAME}Sources)
cmake_print_variables(${TARGETNAME}ModuleSets)
cmake_print_variables(${TARGETNAME}DefaultModuleSet)

set(targetNameTests ${TARGETNAME}Tests)

add_executable(${targetNameTests})
add_executable(PersonalWorks::${targetNameTests} ALIAS ${targetNameTests})
set_target_properties(${targetNameTests} PROPERTIES FOLDER Tests)
gtest_discover_tests(${targetNameTests})

target_include_directories(${targetNameTests}
  PUBLIC
  ${${TARGETNAME}Include}
)

target_sources(${targetNameTests}
  PRIVATE
  ${${TARGETNAME}Sources}
  ${ARG_SOURCEFILES}
  ${ARG_MAINFILE}
)

target_sources(${targetNameTests}
  PRIVATE
    FILE_SET CXX_MODULES FILES ${${TARGETNAME}DefaultModuleSet}
    FILE_SET CXX_MODULES FILES ${ARG_MODULEFILES}
)

foreach(SET ${${TARGETNAME}ModuleSets})
  if(NOT (SET STREQUAL "CXX_MODULES"))
    target_sources(${targetNameTests}
      PRIVATE
        FILE_SET ${SET} CXX_MODULES
    )
  endif()
endforeach()

target_link_libraries(${targetNameTests}
  PRIVATE
  ${${TARGETNAME}Libraires}
  fmt::fmt
  TestUtilities
  $<$<NOT:$<BOOL:${ARG_MAINFILE}>>:GTest::gtest_main>
)

target_compile_definitions(${targetNameTests} PUBLIC TEST_GTEST)

# set_target_properties(${TARGETNAME}Tests PROPERTIES VS_DEBUGGER_COMMAND_ARGUMENTS "--gtest_filter=ErasedStorageTestStructF*")

endfunction()

function(add_benchmark_target TARGETNAME)
  set(options)
  set(oneValueArgs MAINFILE)
  set(multiValueArgs SOURCEFILES MODULEFILES)

  cmake_parse_arguments(PARSE_ARGV 1 ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" )

  #message(STATUS "function=${CMAKE_CURRENT_FUNCTION}")
  #message(STATUS "ARGV=${ARGV}")
  #message(STATUS "ARGN=${ARGN}")
  #cmake_print_variables(TARGETNAME)
  #cmake_print_variables(ARG_MAINFILE)
  #cmake_print_variables(ARG_SOURCEFILES)
  #cmake_print_variables(ARG_MODULEFILES)

  #cmake_print_variables(ARG_PREFIX_UNPARSED_ARGUMENTS)
  #cmake_print_variables(ARG_KEYWORDS_MISSING_VALUES)

if(NOT BUILD_TESTING OR NOT benchmark_FOUND)
  return()
endif()

get_target_property("${TARGETNAME}Include" ${TARGETNAME} INCLUDE_DIRECTORIES)
get_target_property("${TARGETNAME}Sources" ${TARGETNAME} SOURCES)
get_target_property("${TARGETNAME}ModuleSets" ${TARGETNAME} CXX_MODULE_SETS)
get_target_property("${TARGETNAME}DefaultModuleSet" ${TARGETNAME} CXX_MODULE_SET)
get_target_property("${TARGETNAME}Libraires" ${TARGETNAME} LINK_LIBRARIES)

set(targetNameBenchmark ${TARGETNAME}Benchmarks)

add_executable(${targetNameBenchmark})
add_executable(PersonalWorks::${targetNameBenchmark} ALIAS ${targetNameBenchmark})

set_target_properties(${targetNameBenchmark} PROPERTIES FOLDER Benchmarks)

target_include_directories(${targetNameBenchmark}
  PUBLIC
    ${${TARGETNAME}Include}
)

target_sources(${targetNameBenchmark}
  PRIVATE
    ${${TARGETNAME}Sources}
    ${ARG_SOURCEFILES}
    ${ARG_MAINFILE}
)

target_sources(${targetNameBenchmark}
  PRIVATE
    FILE_SET CXX_MODULES FILES ${${TARGETNAME}DefaultModuleSet}
    FILE_SET CXX_MODULES FILES ${ARG_MODULEFILES}
)

foreach(SET ${${TARGETNAME}ModuleSets})
  if(NOT (SET STREQUAL "CXX_MODULES"))
    target_sources(${targetNameBenchmark}
      PRIVATE
        FILE_SET ${SET} CXX_MODULES
    )
  endif()
endforeach()

target_link_libraries(${targetNameBenchmark}
  PRIVATE
  ${${targetName}Libraires}
  fmt::fmt
  TestUtilities
  $<$<NOT:$<BOOL:${ARG_MAINFILE}>>:benchmark::benchmark_main>
)

  #set_target_properties(${targetNameBenchmarks} PROPERTIES VS_DEBUGGER_COMMAND_ARGUMENTS "--benchmark_out=${targetNameBenchmarks}.json --benchmark_out_format=json")

endfunction()
