# if(EXISTS "${CURRENT_BUILDTREES_DIR}/src/.git")
# file(REMOVE_RECURSE ${CURRENT_BUILDTREES_DIR}/src)
# endif()

vcpkg_from_github(
  OUT_SOURCE_PATH SOURCE_PATH
  REPO google/fuzztest
  REF "2025-02-14"
  SHA512 d4e8b36d5a681f0a3bb252db58c1608dcb2441f5073cba88aa919ef168ecd4fd51d40f53ddcab8a56e08191f8a73766f619d539699dab495c0e1918402ee78f4
  HEAD_REF main
  PATCHES "conditionnally-add-gtest.patch"
)

# vcpkg_cmake_get_vars(cmake_vars_file)
# include("${cmake_vars_file}")

# message(STATUS "VCPKG_DETECTED_CMAKE_C_COMPILER=${VCPKG_DETECTED_CMAKE_C_COMPILER}")
# message(STATUS "VCPKG_DETECTED_CMAKE_CXX_COMPILER=${VCPKG_DETECTED_CMAKE_CXX_COMPILER}")

string(APPEND VCPKG_C_FLAGS "") # both must be set
string(APPEND VCPKG_CXX_FLAGS " -stdlib=libc++")

vcpkg_cmake_configure(
  SOURCE_PATH "${SOURCE_PATH}"
  OPTIONS
  "-DCMAKE_C_COMPILER=clang"
  "-DCMAKE_CXX_COMPILER=clang++"
  "-DFETCHCONTENT_FULLY_DISCONNECTED=OFF"

  # "-DFETCHCONTENT_FULLY_DISCONNECTED=ON"
  #"-DFETCHCONTENT_SOURCE_DIR_ABSEIL-CPPLIB=${SOURCE_PATH_ABSEIL}"
)

vcpkg_cmake_install()
vcpkg_copy_pdbs()
vcpkg_cmake_config_fixup(CONFIG_PATH "lib/cmake")

vcpkg_fixup_pkgconfig()
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/share")

vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/LICENSE")
configure_file("${CMAKE_CURRENT_LIST_DIR}/usage" "${CURRENT_PACKAGES_DIR}/share/${PORT}/usage" COPYONLY)
