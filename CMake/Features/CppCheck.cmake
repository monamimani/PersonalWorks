include_guard()

include(CMakeDependentOption)

cmake_dependent_option(PersonalWorks_ENABLE_CPPCHECK "Enable cpp-check analysis" OFF "NOT PersonalWorks_BASIC_BUILD_MODE" OFF)

function(configure_cppcheck)
  find_program(CPPCHECK cppcheck)
  if(CPPCHECK)
    if(CMAKE_GENERATOR MATCHES ".*Visual Studio.*")
      set(CPPCHECK_TEMPLATE "vs")
    else()
      set(CPPCHECK_TEMPLATE "gcc")
    endif()

    option(PersonalWorks_CPPCHECK_WARNINGS_AS_ERRORS "Treat cppcheck warnings as errors" OFF)
    option(PersonalWorks_CPPCHECK_OPTIONS "Additional options for cppcheck" "")

    if("${PersonalWorks_CPPCHECK_OPTIONS}" STREQUAL "")
      # Enable all warnings that are actionable by the user of this toolset
      # style should enable the other 3, but we'll be explicit just in case
      set(CMAKE_CXX_CPPCHECK
        ${CPPCHECK}
        --template=${CPPCHECK_TEMPLATE}
        --enable=style,performance,warning,portability
        --inline-suppr

        # We cannot act on a bug/missing feature of cppcheck
        --suppress=cppcheckError
        --suppress=internalAstError

        # if a file does not have an internalAstError, we get an unmatchedSuppression error
        --suppress=unmatchedSuppression

        # noisy and incorrect sometimes
        --suppress=passedByValue

        # ignores code that cppcheck thinks is invalid C++
        --suppress=syntaxError
        --suppress=preprocessorErrorDirective
        --inconclusive)
    else()
      # if the user provides a CPPCHECK_OPTIONS with a template specified, it will override this template
      set(CMAKE_CXX_CPPCHECK ${CPPCHECK} --template=${CPPCHECK_TEMPLATE} ${PersonalWorks_CPPCHECK_OPTIONS})
    endif()

    if(NOT "${CMAKE_CXX_STANDARD}" STREQUAL "")
      if(CMAKE_CXX_STANDARD LESS 23)
        set(CMAKE_CXX_CPPCHECK ${CMAKE_CXX_CPPCHECK} --std=c++${CMAKE_CXX_STANDARD})
      else()
        # cppcheck doesn't support standard greater than c++20 yet.
        set(CMAKE_CXX_CPPCHECK ${CMAKE_CXX_CPPCHECK} --std=c++20)
      endif()
    endif()

    if(PersonalWorks_CPPCHECK_WARNINGS_AS_ERRORS)
      set(CMAKE_CXX_CPPCHECK ${CMAKE_CXX_CPPCHECK} --error-exitcode=2)
    endif()
    set(CMAKE_CXX_CPPCHECK ${CMAKE_CXX_CPPCHECK} PARENT_SCOPE)
  else()
    message(WARNING "cppcheck requested but executable not found")
  endif()
endfunction()
