include_guard()

find_program(CPPCHECK cppcheck)
find_program(CLANGTIDY clang-tidy)

macro(
  PersonalWorks_enable_cppcheck
  WARNINGS_AS_ERRORS
  CPPCHECK_OPTIONS)
  if(CPPCHECK)
    if(CMAKE_GENERATOR MATCHES ".*Visual Studio.*")
      set(CPPCHECK_TEMPLATE "vs")
    else()
      set(CPPCHECK_TEMPLATE "gcc")
    endif()

    if("${CPPCHECK_OPTIONS}" STREQUAL "")
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
      set(CMAKE_CXX_CPPCHECK ${CPPCHECK} --template=${CPPCHECK_TEMPLATE} ${CPPCHECK_OPTIONS})
    endif()

    if(NOT "${CMAKE_CXX_STANDARD}" STREQUAL "")
      if(CMAKE_CXX_STANDARD LESS 23)
        set(CMAKE_CXX_CPPCHECK ${CMAKE_CXX_CPPCHECK} --std=c++${CMAKE_CXX_STANDARD})
      else()
        # cppcheck doesn't support standard greater than c++20 yet.
        set(CMAKE_CXX_CPPCHECK ${CMAKE_CXX_CPPCHECK} --std=c++20)
      endif()
    endif()

    if(${WARNINGS_AS_ERRORS})
      set(CMAKE_CXX_CPPCHECK ${CMAKE_CXX_CPPCHECK} --error-exitcode=2)
    endif()
  else()
    message(WARNING "cppcheck requested but executable not found")
  endif()
endmacro()

macro(
  PersonalWorks_enable_clang_tidy
  WARNINGS_AS_ERRORS
)
  if(CLANGTIDY)
    # Clang Tidy can't handle GCC PCH file.
    set(CLANG_TIDY_OPTIONS
      ${CLANGTIDY}
      -extra-arg=-Wno-unknown-warning-option
      -extra-arg=-Wno-ignored-optimization-argument
      -extra-arg=-Wno-unused-command-line-argument
      -p
    )

    # set standard
    if(NOT "${CMAKE_CXX_STANDARD}" STREQUAL "")
      if(CMAKE_CXX_COMPILER_ID MATCHES ".*MSVC")
        set(CLANG_TIDY_OPTIONS ${CLANG_TIDY_OPTIONS} -extra-arg=/std:c++${CMAKE_CXX_STANDARD})
      else()
        set(CLANG_TIDY_OPTIONS ${CLANG_TIDY_OPTIONS} -extra-arg=-std=c++${CMAKE_CXX_STANDARD})
      endif()
    endif()

    if(${WARNINGS_AS_ERRORS})
      list(APPEND CLANG_TIDY_OPTIONS -warnings-as-errors=*)
    endif()

    set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY_OPTIONS})
  else()
    message(WARNING "clang-tidy requested but executable not found")
  endif()
endmacro()
