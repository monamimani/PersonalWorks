
set(CPM_DOWNLOAD_VERSION 0.38.7)
set(CPM_HASH_SUM "83e5eb71b2bbb8b1f2ad38f1950287a057624e385c238f6087f94cdfc44af9c5")

set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/CMake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")

set(CPM_SOURCE_CACHE ${CMAKE_BINARY_DIR}/../../cpm_sources)

# Expand relative path. This is important if the provided path contains a tilde (~)
get_filename_component(CPM_DOWNLOAD_LOCATION ${CPM_DOWNLOAD_LOCATION} ABSOLUTE)

# https://github.com/cpm-cmake/CPM.cmake/releases/latest/download/CPM.cmake
file(DOWNLOAD
  https://github.com/cpm-cmake/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake
  ${CPM_DOWNLOAD_LOCATION} EXPECTED_HASH SHA256=${CPM_HASH_SUM}
)

include(${CPM_DOWNLOAD_LOCATION})

set(CPM_USE_LOCAL_PACKAGES TRUE)
set(CPM_USE_NAMED_CACHE_DIRECTORIES TRUE)
