if(test_target_included)
    return()
endif()
set(test_target_included true)

function(add_test_target)
  set(options)
  set(oneValueArgs TARGET MAINFILE)
  set(multiValueArgs TESTFILES MODULEFILES PRIVATEFILES PUBLICFILES INTERFACEFILES LINKLIBS)

  cmake_parse_arguments(PARSE_ARGV 0 ADD_TEST_TARGET "${options}" "${oneValueArgs}" "${multiValueArgs}" )

#  message(STATUS "ARGV=${ARGV}")
#  message(STATUS "ARGN=${ARGN}")
#  message(STATUS "TARGET=${ADD_TEST_TARGET_TARGET}")
#  message(STATUS "MAINFILE=${ADD_TEST_TARGET_MAINFILE}")
#  message(STATUS "TESTFILES=${ADD_TEST_TARGET_TESTFILES}")
#  message(STATUS "MODULEFILES=${ADD_TEST_TARGET_MODULEFILES}")
#  message(STATUS "PRIVATEFILES=${ADD_TEST_TARGET_PRIVATEFILES}")
#  message(STATUS "PUBLICFILES=${ADD_TEST_TARGET_PUBLICFILES}")
#  message(STATUS "INTERFACEFILES=${ADD_TEST_TARGET_INTERFACEFILES}")
#  message(STATUS "LINKLIBS=${ADD_TEST_TARGET_LINKLIBS}")
#  message(STATUS "UNPARSED_ARGUMENTS=${ADD_TEST_TARGET_PREFIX_UNPARSED_ARGUMENTS}")
#  message(STATUS "KEYWORDS_MISSING_VALUES=${ADD_TEST_TARGET_KEYWORDS_MISSING_VALUES}")

  set(targetNameTests ${ADD_TEST_TARGET_TARGET}Tests )
  add_executable(${targetNameTests})

  target_include_directories(${targetNameTests}
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}"
  )

  target_sources(${targetNameTests}
    PRIVATE
      ${ADD_TEST_TARGET_MAINFILE}
      ${ADD_TEST_TARGET_PRIVATEFILES}
      ${ADD_TEST_TARGET_TESTFILES}
    PUBLIC
      ${ADD_TEST_TARGET_PUBLICFILES}
    INTERFACE
      ${ADD_TEST_TARGET_INTERFACEFILES}
  )

  target_link_libraries(${targetNameTests}
    PRIVATE 
      $<$<NOT:$<BOOL:${ADD_TEST_TARGET_MAINFILE}>>:GTest::gtest_main>
      TestUtilities
      ${ADD_TEST_TARGET_LINKLIBS}
  )

  gtest_discover_tests(${targetNameTests}
#    WORKING_DIRECTORY ${BINARY_OUT_DIR}
#    PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${BINARY_OUT_DIR}"
#    XML_OUTPUT_DIR ${BINARY_OUT_DIR}/Tests
    DISCOVERY_MODE PRE_TEST
  )

  set_target_properties(${targetNameTests} PROPERTIES FOLDER Tests)


  if(RUN_TESTS_AFTER_BUILD)
    add_custom_command(TARGET ${targetNameTests}
      # Run after all other rules within the target have been executed
      POST_BUILD
      COMMAND ${targetNameTests}
      COMMENT "Running Google Test"
      VERBATIM
    )
  endif()

endfunction()