# MSVC toolchain
include_guard(GLOBAL)

set(CMAKE_SYSTEM_NAME Windows CACHE STRING "")

include(${CMAKE_CURRENT_LIST_DIR}/common-toolchain.cmake)

set(CMAKE_CXX_COMPILER "cl")
set(CMAKE_C_COMPILER "cl")

# MSVC specific settings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /utf-8 /diagnostics:caret")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP /utf-8 /diagnostics:caret")

#I should set the dynamicdeoptimization in release mode. not in shipping builds
#add_compile_options("/dynamicdeopt")

if("$ENV{VSCMD_VER}" STREQUAL "")
  message(WARNING "Not in a developer environment, run cmake from a Visual Studio Developer terminal.")
endif()

set(MSVC_WARNINGS
   /W4 # Baseline reasonable warnings
    /w14242 # 'identifier': conversion from 'type1' to 'type2', possible loss of data
    /w14254 # 'operator': conversion from 'type1:field_bits' to 'type2:field_bits', possible loss of data
    /w14263 # 'function': member function does not override any base class virtual member function
    /w14265 # 'classname': class has virtual functions, but destructor is not virtual instances of this class may not

    # be destructed correctly
    /w14287 # 'operator': unsigned/negative constant mismatch
    /we4289 # nonstandard extension used: 'variable': loop control variable declared in the for-loop is used outside

    # the for-loop scope
    /w14296 # 'operator': expression is always 'boolean_value'
    /w14311 # 'variable': pointer truncation from 'type1' to 'type2'
    /w14545 # expression before comma evaluates to a function which is missing an argument list
    /w14546 # function call before comma missing argument list
    /w14547 # 'operator': operator before comma has no effect; expected operator with side-effect
    /w14549 # 'operator': operator before comma has no effect; did you intend 'operator'?
    /w14555 # expression has no effect; expected expression with side- effect
    /w14619 # pragma warning: there is no warning number 'number'
    /w14640 # Enable warning on thread un-safe static member initialization
    /w14826 # Conversion from 'type1' to 'type2' is sign-extended. This may cause unexpected runtime behavior.
    /w14905 # wide string literal cast to 'LPSTR'
    /w14906 # string literal cast to 'LPWSTR'
    /w14928 # illegal copy-initialization; more than one user-defined conversion has been implicitly applied
    /permissive- # standards conformance mode for MSVC compiler.
)

string(REPLACE ";" " " MSVC_WARNINGS_STR "${MSVC_WARNINGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MSVC_WARNINGS_STR}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${MSVC_WARNINGS_STR}")

include(${CMAKE_CURRENT_LIST_DIR}/vcpkg-toolchain.cmake)
