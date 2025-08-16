
if (NOT CMAKE_SYSTEM_NAME STREQUAL "Linux")
message(WARNING "CodeExercices only support Linux.")
return()
endif()

project(CodeExercices
  LANGUAGES CXX C)

# CPMAddPackage(
#   NAME fuzztest
#   GITHUB_REPOSITORY google/fuzztest
#   GIT_TAG "2025-02-14"
# )

# if(fuzztest_ADDED)

# endif()

get_filename_component(targetName ${CMAKE_CURRENT_LIST_DIR} NAME)

add_executable(${targetName} main.cpp)
