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

set(headerUnitFiles 
)

set(publicFiles
  ${targetName}/GoogleTest.h
  ${targetName}/GoogleBenchmark.h
  ${platform_publicFiles}
)

set(moduleFiles 
)

set(privateFiles 
  ${platform_privateFiles}
)

set(testFiles 
)

set(benchmarkFiles 
)

add_library(${targetName})

target_include_directories(${targetName}
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}"
)

add_target_sources(${targetName} 
   PRIVATEFILES ${privateFiles}
   MODULEFILES ${moduleFiles}
   PUBLICFILES ${publicFiles}
   HEADERUNITFILES ${headerUnitFiles}
   INTERFACEFILES ${interfaceFiles}
   TESTFILES ${testFiles}
   BENCHMARKFILES ${benchmarkFiles}
   )

set(privateLibs
)

add_target_link_libraries(${targetName}
  PRIVATELIBS ${privateLibs}
)