if(benchmark_target_included)
    return()
endif()
set(benchmark_target_included true)

function(add_benchmark_target)
  set(options)
  set(oneValueArgs TARGET MAINFILE)
  set(multiValueArgs BENCHMARKFILES MODULEFILES HEADERUNITFILES PRIVATEFILES PUBLICFILES INTERFACEFILES LINKLIBS)

  cmake_parse_arguments(PARSE_ARGV 0 ADD_BENCHMARK_TARGET "${options}" "${oneValueArgs}" "${multiValueArgs}" )

#  message(STATUS "ARGV=${ARGV}")
#  message(STATUS "ARGN=${ARGN}")
#  message(STATUS "TARGET=${ADD_BENCHMARK_TARGET_TARGET}")
#  message(STATUS "MAINFILE=${ADD_BENCHMARK_TARGET_MAINFILE}")
#  message(STATUS "BENCHMARKFILES=${ADD_BENCHMARK_TARGET_BENCHMARKFILES}")
#  message(STATUS "MODULEFILES=${ADD_BENCHMARK_TARGET_MODULEFILES}")
#  message(STATUS "PRIVATEFILES=${ADD_BENCHMARK_TARGET_PRIVATEFILES}")
#  message(STATUS "PUBLICFILES=${ADD_BENCHMARK_TARGET_PUBLICFILES}")
#  message(STATUS "INTERFACEFILES=${ADD_BENCHMARK_TARGET_INTERFACEFILES}")
#  message(STATUS "LINKLIBS=${ADD_BENCHMARK_TARGET_LINKLIBS}")
#  message(STATUS "UNPARSED_ARGUMENTS=${ADD_BENCHMARK_TARGET_PREFIX_UNPARSED_ARGUMENTS}")
#  message(STATUS "KEYWORDS_MISSING_VALUES=${ADD_BENCHMARK_TARGET_KEYWORDS_MISSING_VALUES}")

  set(targetNameBenchmarks ${ADD_BENCHMARK_TARGET_TARGET}Benchmarks )
  add_executable(${targetNameBenchmarks})

  target_include_directories(${targetNameBenchmarks}
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}"
  )
  
  if(CMAKE_VERSION GREATER_EQUAL "3.26.0" )
    target_sources(${targetNameBenchmarks}
    PRIVATE
      ${ADD_BENCHMARK_TARGET_MAINFILE}
      ${ADD_BENCHMARK_TARGET_PRIVATEFILES}
      ${ADD_BENCHMARK_TARGET_BENCHMARKFILES}
    PUBLIC
      ${ADD_BENCHMARK_TARGET_PUBLICFILES}
    PUBLIC
      FILE_SET CXX_MODULES FILES ${ADD_BENCHMARK_TARGET_MODULEFILES}
    PUBLIC
      FILE_SET CXX_MODULE_HEADER_UNITS FILES ${ADD_BENCHMARK_TARGET_HEADERUNITFILES}
    INTERFACE
      ${ADD_BENCHMARK_TARGET_INTERFACEFILES}
    )
  else()
    target_sources(${targetNameBenchmarks}
      PRIVATE
        ${ADD_BENCHMARK_TARGET_MAINFILE}
        ${ADD_BENCHMARK_TARGET_PRIVATEFILES}
        ${ADD_BENCHMARK_TARGET_BENCHMARKFILES}
        ${ADD_BENCHMARK_TARGET_MODULEFILES}
      PUBLIC
        ${ADD_BENCHMARK_TARGET_PUBLICFILES}
        ${ADD_BENCHMARK_TARGET_HEADERUNITFILES}
      INTERFACE
        ${ADD_BENCHMARK_TARGET_INTERFACEFILES}
    )
  endif()

  target_compile_definitions(${targetNameBenchmarks} PUBLIC BENCHMARK_STATIC_DEFINE) # See https://stackoverflow.com/questions/73494386/lnk2001-linker-error-while-linking-google-benchmark-lib

  target_link_libraries(${targetNameBenchmarks}
    PRIVATE 
      $<$<NOT:$<BOOL:${ADD_BENCHMARK_TARGET_MAINFILE}>>:benchmark::benchmark_main>
      TestUtilities
      ${ADD_BENCHMARK_TARGET_LINKLIBS}
  )

  set_target_properties(${targetNameBenchmarks} PROPERTIES FOLDER Benchmarks)

  #set_target_properties(${targetNameBenchmarks} PROPERTIES VS_DEBUGGER_COMMAND_ARGUMENTS "--benchmark_out=${targetNameBenchmarks}.json --benchmark_out_format=json")

endfunction()