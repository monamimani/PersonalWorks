include_guard()

include(CMakeDependentOption)

cmake_dependent_option(PersonalWorks_ENABLE_CLANG_TIDY "Enable clang-tidy" ON "NOT PersonalWorks_BASIC_BUILD_MODE" OFF)

function(configure_clang_tidy)
  find_program(CLANGTIDY clang-tidy)
  if(CLANGTIDY)
    set(CLANG_TIDY_OPTIONS
      ${CLANGTIDY}
      --enable-module-headers-parsing
      -p=${CMAKE_BINARY_DIR}
    )

    if(NOT "${CMAKE_CXX_STANDARD}" STREQUAL "")
      if(CMAKE_CXX_COMPILER_ID MATCHES ".*MSVC")
        set(CLANG_TIDY_OPTIONS ${CLANG_TIDY_OPTIONS}
          --extra-arg=/EHsc
          --extra-arg=-v
          --extra-arg-before=-v
        )
      endif()
    endif()

    option(PersonalWorks_CLANG_TIDY_WARNINGS_AS_ERRORS "Treat clang-tidy warnings as errors" OFF)
    if(PersonalWorks_CLANG_TIDY_WARNINGS_AS_ERRORS)
      list(APPEND CLANG_TIDY_OPTIONS -warnings-as-errors=*)
    endif()

    set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY_OPTIONS} PARENT_SCOPE)
  else()
    message(WARNING "clang-tidy requested but executable not found")
  endif()
endfunction()
