include_guard()

include(CheckIPOSupported)
include(CMakeDependentOption)

cmake_dependent_option(PersonalWorks_ENABLE_IPO "Enable IPO/LTO" ON "NOT PersonalWorks_BASIC_BUILD_MODE" OFF)

function(configure_ipo)
  check_ipo_supported(RESULT IPO_RESULT OUTPUT output)
  if(IPO_RESULT)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
  else()
    message(WARNING "IPO is not supported: ${output}")
  endif()
endfunction()
