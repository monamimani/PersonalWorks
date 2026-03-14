# Objective
Fix the CI failure in the `Windows-Msvc-Ninja-Debug` preset caused by a timeout during GoogleTest discovery.

# Background & Motivation
The log shows that the build fails during the linking/post-build phase of `CoreTests.exe`. The specific error originates from `GoogleTestAddTests.cmake`, which is executing `CoreTests.exe` to discover the tests available in the executable:
```
Result: Process terminated due to timeout
```
By default, CMake's `gtest_discover_tests` function has a hardcoded `DISCOVERY_TIMEOUT` of 5 seconds. When building with MSVC in Debug mode on a CI runner, especially with Address Sanitizers enabled (`x64-windows-sanitizers` was resolved by vcpkg), the executable can take longer than 5 seconds just to load DLLs and initialize before it can even print the test list. This triggers the CMake timeout and fails the build.

# Scope & Impact
- **Impacted Files**: `CMake/Targets/TestTarget.cmake`
- The change is minimal and only affects the CMake configuration for test generation. It extends the timeout window allowing the test executable enough time to report its tests.

# Proposed Solution
Modify `CMake/Targets/TestTarget.cmake` where `gtest_discover_tests` is called. Add the `DISCOVERY_TIMEOUT` argument with a higher value, such as 60 seconds, to prevent timeouts on slower CI runners.

# Implementation Steps
1. Modify `CMake/Targets/TestTarget.cmake`.
2. Change the following line:
   ```cmake
   gtest_discover_tests(${targetNameTests})
   ```
   to:
   ```cmake
   gtest_discover_tests(${targetNameTests} DISCOVERY_TIMEOUT 60)
   ```

# Verification
After making this change, the post-build test discovery step will have 60 seconds to complete, which is more than enough time for `CoreTests.exe` to initialize and list its tests on the CI runner. We will verify the build locally and the CI pipeline will confirm the fix globally.
