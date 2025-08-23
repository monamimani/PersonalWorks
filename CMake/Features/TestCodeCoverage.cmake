include_guard()

include(CMakeDependentOption)

cmake_dependent_option(PersonalWorks_ENABLE_TEST_CODE_COVERAGE "Enable coverage reporting" OFF "NOT PersonalWorks_BASIC_BUILD_MODE" OFF)

function(configure_test_code_coverage)
  message(STATUS "Enabling code coverage")
  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # Add compiler flags for code coverage
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
  elseif(MSVC)
    message(STATUS "Configuring OpenCppCoverage for MSVC")
    find_program(OPEN_CPP_COVERAGE_EXE OpenCppCoverage)
    if(OPEN_CPP_COVERAGE_EXE)
      message(STATUS "OpenCppCoverage found at: ${OPEN_CPP_COVERAGE_EXE}")
      # Define a custom target to run OpenCppCoverage
      add_custom_target(run_coverage
        COMMAND ${OPEN_CPP_COVERAGE_EXE}
        ARGS "--export_type=html" "--output_dir=${CMAKE_BINARY_DIR}/coverage_report"
             "--sources=${CMAKE_SOURCE_DIR}" "--excluded_sources=${CMAKE_BINARY_DIR}"
             "--excluded_sources=vcpkg_installed" "--excluded_sources=_deps"
             "--excluded_sources=Tests" "--excluded_sources=Benchmarks"
             "--excluded_sources=FuzzTests"
             "--cover_children" "--working_directory=${CMAKE_BINARY_DIR}"
             "-- ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target ALL_BUILD --config $<CONFIG>"
        COMMENT "Generating code coverage report with OpenCppCoverage"
        USES_TERMINAL
      )
      message(STATUS "To generate coverage report, run: cmake --build . --target run_coverage")
      message(STATUS "Report will be in: ${CMAKE_BINARY_DIR}/coverage_report/index.html")
    else()
      message(WARNING "OpenCppCoverage.exe not found. Please install it or add it to your PATH.")
    endif()
  endif()
endfunction()
