include_guard()

if(PersonalWorks_BUILD_BENCHMARKS)
  find_package(benchmark CONFIG REQUIRED)
endif()

if(BUILD_TESTING) # or if(PersonalWorks_BUILD_TESTS)
  find_package(Catch2 3 CONFIG REQUIRED)
  include(Catch)

  if(NOT (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC"))
    # block(SCOPE_FOR VARIABLES)
    # unset(CMAKE_CXX_CPPCHECK)
    # unset(CMAKE_CXX_CLANG_TIDY)
    # FetchContent_Declare(
    # fuzztest
    # GIT_REPOSITORY https://github.com/google/fuzztest.git
    # GIT_TAG main
    # )
    # FetchContent_MakeAvailable(fuzztest)
    # endblock()
    block(SCOPE_FOR VARIABLES)
    unset(CMAKE_CXX_CPPCHECK)
    unset(CMAKE_CXX_CLANG_TIDY)
    CPMAddPackage(
      NAME fuzztest
      GITHUB_REPOSITORY google/fuzztest
      GIT_TAG 951aada2ac48f3a37606718e60c264ed57bdd772
      GIT_SHALLOW
      SYSTEM
      EXCLUDE_FROM_ALL
    )
    endblock()

    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
      set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    endif()
  endif()

  if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(_FIND_COMPONENTS "") # To Silence Gtest dev warning
    find_package(GTest CONFIG REQUIRED)
    include(GoogleTest)
    unset(_FIND_COMPONENTS)

    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
      set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    endif()
  endif()
endif()
