# Plan - Fix GTest Discovery Timeout in CI

The CI job `Windows-Msvc-Ninja-Debug` is failing during test discovery for `CoreTests.exe` because the default 5-second timeout is insufficient for the ASan build. Although `DISCOVERY_TIMEOUT 600` is defined in `CMake/Targets/TestTarget.cmake`, this file is not included in the build, and most tests call `gtest_discover_tests` directly without a timeout.

## Objective
Increase the test discovery timeout to 600 seconds for all GTest targets to prevent CI failures.

## Key Files & Context
- `Projects/EnginePlayground/*/CMakeLists.txt`: Individual project files calling `gtest_discover_tests`.

## Implementation Steps

### 1. Update manual gtest_discover_tests calls
Add `DISCOVERY_TIMEOUT 600` to all manual calls of `gtest_discover_tests` in the following files:
- `Projects/EnginePlayground/TestUtilities/CMakeLists.txt`
- `Projects/EnginePlayground/Core/CMakeLists.txt`
- `Projects/EnginePlayground/ApplicationCore/CMakeLists.txt`
- `Projects/EnginePlayground/BasicWindowApp/CMakeLists.txt`

## Verification & Testing

### 1. Local Configuration Check
Run CMake configuration locally:
```pwsh
.\Scripts\Build\BuildWindowsMsvcNinjaDebugNoTidy.ps1
```
Then check the generated test discovery files in the build directory (e.g., `_Out/build/Windows-Msvc-Ninja-Debug/Projects/EnginePlayground/Core/CoreTests[1]_tests.cmake`) to verify that `TEST_DISCOVERY_TIMEOUT` is set to `600`.

### 2. CI Validation
The primary validation will be the successful completion of the `Windows-Msvc-Ninja-Debug` job in CI.
