include_guard()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "Thirdparty")

include(FetchContent)
include(ProcessorCount)
ProcessorCount(PROCESSOR_COUNT)
include(GenerateExportHeader)
include(CMake/Utilities/Utilities.cmake)
include(CMake/BuildConfig/BuildTypes.cmake)
include(CMake/BuildConfig/BuildOutput.cmake)
include(CMake/Targets/Targets.cmake)

set(GIT_SHA "Unknown" CACHE STRING "SHA this build was generated from")
string(SUBSTRING "${GIT_SHA}" 0 8 GIT_SHORT_SHA)
