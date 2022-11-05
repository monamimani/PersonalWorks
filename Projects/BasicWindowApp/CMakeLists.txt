# file(GLOB_RECURSE sources CONFIGURE_DEPENDS "*.cpp" "*.cxx" "*.ixx")
# file(GLOB_RECURSE headers CONFIGURE_DEPENDS "*.h" "*.hpp")

# Optionally glob, but only for CMake 3.12 or later: file(GLOB HEADER_LIST
# CONFIGURE_DEPENDS "${ModernCMakeExample_SOURCE_DIR}/include/modern/*.hpp")

get_filename_component(targetName ${CMAKE_CURRENT_LIST_DIR} NAME)

if(CMAKE_SYSTEM_NAME STREQUAL Windows)

  set(platform_publicFiles 

  )

  set(platform_privateFiles 
  )

elseif (CMAKE_SYSTEM_NAME STREQUAL Linux)

  set(platform_publicFiles 
  )

  set(platform_privateFiles 
  )

endif()

set(interfaceFiles 
)

set(publicFiles 
  ${targetName}/BasicWindowApp.h
)

set(privateFiles 
    ${targetName}/BasicWindowApp.cpp
)

set(testFiles 
  ${targetName}/BasicWindowApp.test.cpp
)

set(benchmarkFiles 
)

add_executable(${targetName} ${platform_privateFiles})

target_sources(${targetName}
  PRIVATE
    ${privateFiles}
    main.cpp
  PUBLIC
    ${publicFiles}
  INTERFACE
    ${interfaceFiles}
)

target_include_directories(${targetName}
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(${targetName}
  PRIVATE
  PUBLIC
    Core
    ApplicationCore
)

if(BUILD_TESTS)
  add_test_target(
    TARGET ${targetName} 
    TESTFILES ${testFiles}
    PRIVATEFILES ${privateFiles}
    PUBLICFILES ${publicFiles}
    INTERFACEFILES ${interfaceFiles}
    MAINFILE BasicWindowAppMain.test.cpp
)
endif()

if(BUILD_BENCHMARKS)
  add_benchmark_target(
    TARGET ${targetName} 
    BENCHMARKFILES ${benchmarkFiles}
    PRIVATEFILES ${privateFiles}
    PUBLICFILES ${publicFiles}
    INTERFACEFILES ${interfaceFiles})
endif()