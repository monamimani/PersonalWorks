include_guard()

include(CheckIPOSupported)
check_ipo_supported(RESULT IPO_RESULT OUTPUT output)

if(IPO_RESULT)
  set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
else()
  message(WARNING "IPO is not supported: ${output}")
endif()