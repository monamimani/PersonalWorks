include_guard()

function(check_libfuzzer_support var_name)
  set(LibFuzzerTestSource
    "
#include <cstdint>
#include <cstddef>

extern \"C\" int LLVMFuzzerTestOneInput(const std::uint8_t *data, std::size_t size) {
  return  0;
}
    ")

  include(CheckCXXSourceCompiles)
  set(CMAKE_REQUIRED_FLAGS "-fsanitize=fuzzer")
  set(CMAKE_REQUIRED_LINK_OPTIONS "-fsanitize=fuzzer;-stdlib=libstdc++")
  check_cxx_source_compiles("${LibFuzzerTestSource}" ${var_name})
endfunction()
