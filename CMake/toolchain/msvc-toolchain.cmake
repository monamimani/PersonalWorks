# MSVC toolchain

include(${CMAKE_CURRENT_LIST_DIR}/common-toolchain.cmake)

# MSVC specific settings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest /MP /utf-8 /diagnostics:caret")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP /utf-8 /diagnostics:caret")


if("$ENV{VSCMD_VER}" STREQUAL "")
  message(WARNING "Not in a developer environment, run cmake from a Visual Studio Developer terminal.")
endif()

set(MSVC_WARNINGS
  /W4
  /w14242
  /w14254
  /w14263
  /w14265
  /w14287
  /we4289
  /w14296
  /w14311
  /w14545
  /w14546
  /w14547
  /w14549
  /w14555
  /w14619
  /w14640
  /w14826
  /w14905
  /w14906
  /w14928
  /permissive-
  /WX
)

string(REPLACE ";" " " MSVC_WARNINGS_STR "${MSVC_WARNINGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MSVC_WARNINGS_STR}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${MSVC_WARNINGS_STR}")
