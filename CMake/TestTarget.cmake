if(test_target_included)
    return()
endif()
set(test_target_included true)

function(add_test_target)
  set(options)
  set(oneValueArgs TARGET MAINFILE)
  set(multiValueArgs TESTFILES MODULEFILES HEADERUNITFILES PRIVATEFILES PUBLICFILES INTERFACEFILES LINKLIBS)

  cmake_parse_arguments(PARSE_ARGV 0 ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" )

#  message(STATUS "ARGV=${ARGV}")
#  message(STATUS "ARGN=${ARGN}")
#  message(STATUS "TARGET=${ARG_TARGET}")
#  message(STATUS "MAINFILE=${ARG_MAINFILE}")
#  message(STATUS "TESTFILES=${ARG_TESTFILES}")
#  message(STATUS "MODULEFILES=${ARG_MODULEFILES}")
#  message(STATUS "PRIVATEFILES=${ARG_PRIVATEFILES}")
#  message(STATUS "PUBLICFILES=${ARG_PUBLICFILES}")
#  message(STATUS "INTERFACEFILES=${ARG_INTERFACEFILES}")
#  message(STATUS "LINKLIBS=${ARG_LINKLIBS}")
#  message(STATUS "UNPARSED_ARGUMENTS=${ARG_PREFIX_UNPARSED_ARGUMENTS}")
#  message(STATUS "KEYWORDS_MISSING_VALUES=${ARG_KEYWORDS_MISSING_VALUES}")

  set(targetNameTests ${ARG_TARGET}Tests )
  add_executable(${targetNameTests})

  target_include_directories(${targetNameTests}
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}"
  )

#  get_target_property(TARGET_SOURCES ${ARG_TARGET} SOURCES)
#  message(STATUS "${ARG_TARGET} sources=${TARGET_SOURCES}")
#
#  get_target_property(TARGET_HEADER_SET ${ARG_TARGET} HEADER_SET)
#  message(STATUS "${ARG_TARGET} headerSet=${TARGET_HEADER_SET}")
#
#  get_target_property(TARGET_CXX_MODULE_SET ${ARG_TARGET} CXX_MODULE_SET)
#  message(STATUS "${ARG_TARGET} cxxModuleSet=${TARGET_CXX_MODULE_SET}")
#
#  get_target_property(TARGET_LINK_LIBRARIES ${ARG_TARGET} LINK_LIBRARIES)
#  message(STATUS "${ARG_TARGET} linkLibraries=${TARGET_LINK_LIBRARIES}")
#
#  get_target_property(TARGET_INTERFACE_LINK_LIBRARIES ${ARG_TARGET} INTERFACE_LINK_LIBRARIES)
#  message(STATUS "${ARG_TARGET} interfaceLinkLibraries=${TARGET_INTERFACE_LINK_LIBRARIES}")

  if(CMAKE_VERSION GREATER_EQUAL "3.26.0" )
    target_sources(${targetNameTests}
      PRIVATE
        ${ARG_MAINFILE}
        ${ARG_PRIVATEFILES}
        ${ARG_TESTFILES}
      PUBLIC
        ${ARG_PUBLICFILES}
      PUBLIC 
        FILE_SET CXX_MODULES FILES ${ARG_MODULEFILES}
      #PUBLIC 
        #FILE_SET CXX_MODULE_HEADER_UNITS FILES ${ARG_HEADERUNITFILES}
      INTERFACE
        ${ARG_INTERFACEFILES}
    )
  else()
    target_sources(${targetNameTests}
      PRIVATE
        ${ARG_MAINFILE}
        ${ARG_PRIVATEFILES}
        ${ARG_MODULEFILES}
        ${ARG_TESTFILES}
      PUBLIC
        ${ARG_PUBLICFILES}
        ${ARG_HEADERUNITFILES}
      INTERFACE
        ${ARG_INTERFACEFILES}
    )
  endif()

  target_link_libraries(${targetNameTests}
    PRIVATE 
      $<$<NOT:$<BOOL:${ARG_MAINFILE}>>:GTest::gtest_main>
      TestUtilities
      SanitizersConfiguration
      ${ARG_LINKLIBS}
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

function(_make_test_target_name resultTargetNameTests targetName)
  set(${resultTargetNameTests} ${${targetName}}Tests)
  return(PROPAGATE ${resultTargetNameTests})
endfunction()

function(add_test_target_sources TARGETNAME)
  set(options)
  set(oneValueArgs MAINFILE)
  set(multiValueArgs TESTFILES MODULEFILES HEADERUNITFILES PRIVATEFILES PUBLICFILES INTERFACEFILES)

  cmake_parse_arguments(PARSE_ARGV 0 ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" )

#  message(STATUS "ARGV=${ARGV}")
#  message(STATUS "ARGN=${ARGN}")
#  message(STATUS "TARGET=${TARGETNAME}")
#  message(STATUS "MAINFILE=${ARG_MAINFILE}")
#  message(STATUS "TESTFILES=${ARG_TESTFILES}")
#  message(STATUS "MODULEFILES=${ARG_MODULEFILES}")
#  message(STATUS "HEADERUNITFILES=${ARG_HEADERUNITFILES}")
#  message(STATUS "PRIVATEFILES=${ARG_PRIVATEFILES}")
#  message(STATUS "PUBLICFILES=${ARG_PUBLICFILES}")
#  message(STATUS "INTERFACEFILES=${ARG_INTERFACEFILES}")
#  message(STATUS "UNPARSED_ARGUMENTS=${ARG_PREFIX_UNPARSED_ARGUMENTS}")
#  message(STATUS "KEYWORDS_MISSING_VALUES=${ARG_KEYWORDS_MISSING_VALUES}")

  _make_test_target_name(targetNameTests TARGETNAME)

  add_executable(${targetNameTests})

  target_include_directories(${targetNameTests}
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}"
  )

  define_property(
   TARGET 
   PROPERTY HAS_TEST_MAIN_FILE
   BRIEF_DOCS "The target has a user provided main file."
   )
   set_target_properties(${targetNameTests} PROPERTIES HAS_TEST_MAIN_FILE FALSE)

   if(${ARG_MAINFILE})
    set_target_properties(${targetNameTests} PROPERTIES HAS_TEST_MAIN_FILE TRUE)
   endif()

  if(CMAKE_VERSION GREATER_EQUAL "3.26.0" )
    target_sources(${targetNameTests}
      PRIVATE
        ${ARG_MAINFILE}
        ${ARG_PRIVATEFILES}
        ${ARG_TESTFILES}
      PUBLIC
        ${ARG_PUBLICFILES}
      PUBLIC 
        FILE_SET CXX_MODULES FILES ${ARG_MODULEFILES}
      #PUBLIC 
        #FILE_SET CXX_MODULE_HEADER_UNITS FILES ${ARG_HEADERUNITFILES}
      INTERFACE
        ${ARG_INTERFACEFILES}
    )
  else()
    target_sources(${targetNameTests}
      PRIVATE
        ${ARG_MAINFILE}
        ${ARG_PRIVATEFILES}
        ${ARG_MODULEFILES}
        ${ARG_TESTFILES}
      PUBLIC
        ${ARG_PUBLICFILES}
        ${ARG_HEADERUNITFILES}
      INTERFACE
        ${ARG_INTERFACEFILES}
    )
  endif()

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

function(add_test_target_link_libraries TARGETNAME)
  set(options)
  set(oneValueArgs)
  set(multiValueArgs TESTLIBS PRIVATELIBS PUBLICLIBS INTERFACELIBS)

  cmake_parse_arguments(PARSE_ARGV 1 ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" )

  #message(STATUS "function=${CMAKE_CURRENT_FUNCTION}")
  #message(STATUS "ARGV=${ARGV}")
  #message(STATUS "ARGN=${ARGN}")
  #message(STATUS "TARGETNAME=${TARGETNAME}")
  #message(STATUS "TESTLIBS=${ARG_TESTLIBS}")
  #message(STATUS "PRIVATELIBS=${ARG_PRIVATELIBS}")
  #message(STATUS "PUBLICLIBS=${ARG_PUBLICLIBS}")
  #message(STATUS "INTERFACELIBS=${ARG_INTERFACELIBS}")
  #message(STATUS "UNPARSED_ARGUMENTS=${ARG_PREFIX_UNPARSED_ARGUMENTS}")
  #message(STATUS "KEYWORDS_MISSING_VALUES=${ARG_KEYWORDS_MISSING_VALUES}")

  _make_test_target_name(targetNameTests TARGETNAME)

  if (NOT TARGET ${targetNameTests})
    return()
  endif()

  get_target_property(HASTESTMAINFILE ${targetNameTests} HAS_TEST_MAIN_FILE)
  #message(STATUS "HASTESTMAINFILE=${HASTESTMAINFILE}")

  if(NOT HASTESTMAINFILE)
  set(LinkGTestMain GTest::gtest_main)
  endif()

  target_link_libraries(${targetNameTests}
    PRIVATE
      ${ARG_PRIVATELIBS}
      ${LinkGTestMain}
      #$<$<NOT:$<BOOL:${ARG_MAINFILE}>>:GTest::gtest_main>
      TestUtilities
      SanitizersConfiguration
    PUBLIC
      ${ARG_PUBLICLIBS}
    INTERFACE
      ${ARG_INTERFACELIBS}
  )

endfunction()