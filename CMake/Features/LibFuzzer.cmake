include_guard()



function(check_libfuzzer_support var_name)
  set(LibFuzzerTestSource
    "\n#include <cstdint>\n#include <cstddef>\n\nextern \"C\" int LLVMFuzzerTestOneInput(const std::uint8_t *data, std::size_t size) {\n  return  0;\n}\n    ")

  include(CheckCXXSourceCompiles)
  set(CMAKE_REQUIRED_FLAGS "-fsanitize=fuzzer")
  set(CMAKE_REQUIRED_LINK_OPTIONS "-fsanitize=fuzzer;-stdlib=libstdc++")
  check_cxx_source_compiles("${LibFuzzerTestSource}" ${var_name})
endfunction()

function(configure_libfuzzer)
  check_libfuzzer_support(LIBFUZZER_SUPPORTED)

  if(LIBFUZZER_SUPPORTED AND(PersonalWorks_ENABLE_SANITIZER_ADDRESS OR PersonalWorks_ENABLE_SANITIZER_THREAD OR PersonalWorks_ENABLE_SANITIZER_UNDEFINED))
    set(DEFAULT_FUZZER ON)
  else()
    set(DEFAULT_FUZZER OFF)
  endif()

  option(PersonalWorks_BUILD_FUZZ_TESTS "Enable fuzz testing executable" ${DEFAULT_FUZZER})

endfunction()
