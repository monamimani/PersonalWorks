if(static_analizers_included)
    return()
endif()
set(static_analizers_included true)

option(ENABLE_CPPCHECK "Enable static analysis with cppcheck" OFF)
option(ENABLE_CLANG_TIDY "Enable static analysis with clang-tidy" OFF)
if(ENABLE_CPPCHECK)
    find_program(CPPCHECK cppcheck)
    if(CPPCHECK)
        set(CMAKE_CXX_CPPCHECK ${CPPCHECK} --enable=all)
        #set(CMAKE_CXX_CPPCHECK ${CPPCHECK} --suppress=missingInclude --enable=all
        #        --inconclusive -i ${CMAKE_SOURCE_DIR}/imgui/lib)
    else()
        message(WARNING "cppcheck requested but executable not found")
    endif()
endif()

if(ENABLE_CLANG_TIDY)
    find_program(CLANGTIDY clang-tidy)
    if(CLANGTIDY)
        #message(STATUS "clang-tidy found: ${CLANGTIDY}")

        set(CLANG_TIDY_OPTIONS
            ${CLANGTIDY}
            --extra-arg=-Wno-unknown-warning-option
            --extra-arg=-Wno-ignored-optimization-argument
            --extra-arg=-Wno-unused-command-line-argument)

        #if("${CMAKE_CXX_CLANG_TIDY_DRIVER_MODE}" STREQUAL "cl")
        #    set(CLANG_TIDY_OPTIONS ${CLANG_TIDY_OPTIONS} --extra-arg=/std:c++${CMAKE_CXX_STANDARD})
        #else()
        #    set(CLANG_TIDY_OPTIONS ${CLANG_TIDY_OPTIONS} --extra-arg=-std=c++${CMAKE_CXX_STANDARD})
        #endif()

        set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY_OPTIONS})
    else()
        message(WARNING "clang-tidy requested but executable not found")
    endif()
endif()