# Plan: Fix Linux CI Compiler Mismatch via Explicit vcpkg Triplets

## Objective
Resolve the "incompatible ASan runtimes" linker error in the Linux Clang CI by configuring `vcpkg` to explicitly use the same compiler (Clang or GCC) as the main project, without relying on environment variables.

## Background & Rationale
Currently, the `x64-linux-sanitizers.cmake` triplet attempts to inherit the compiler via `VCPKG_ENV_PASSTHROUGH CC CXX`. Since you recently modernized the CMake Presets to use the `CMAKE_CXX_COMPILER` cache variable instead of relying on the `CC`/`CXX` environment variables, `vcpkg` is falling back to the system default compiler (`g++`). This leads to a mismatch where `vcpkg` builds dependencies (like Catch2) with GCC+ASan, but the main project is compiled with Clang+ASan.

The most robust, "vcpkg-native" solution is to stop relying on environment variables. Instead, we should create specific triplets for GCC and Clang. This guarantees deterministic dependency builds that match the main project's toolchain.

## Proposed Changes

### 1. Create Explicit Triplets
We will replace the generic `x64-linux-sanitizers.cmake` with two explicit triplets:
- `Vcpkg/OverlayTriplets/x64-linux-clang-sanitizers.cmake`: Explicitly sets `VCPKG_C_COMPILER` to `clang` and `VCPKG_CXX_COMPILER` to `clang++`.
- `Vcpkg/OverlayTriplets/x64-linux-gcc-sanitizers.cmake`: Explicitly sets `VCPKG_C_COMPILER` to `gcc` and `VCPKG_CXX_COMPILER` to `g++`.

Both will still read the `BUILD_SANITIZERS` environment variable to apply the correct sanitizer flags.

### 2. Update Toolchain Files
We need to update the CMake toolchain files to point to these new triplets.

- **File**: `CMake/Toolchain/Linux/llvm-toolchain.cmake`
  - **Change**: Set `VCPKG_TARGET_TRIPLET` to `"x64-linux-clang-sanitizers"`.
- **File**: `CMake/Toolchain/Linux/llvm-libc++-toolchain.cmake`
  - **Change**: Currently it doesn't set a triplet explicitly, but we should make sure it inherits or sets `"x64-linux-clang-sanitizers"` (or a new libc++ sanitizer triplet if needed, but for now we'll match `llvm-toolchain.cmake`).
- **File**: `CMake/Toolchain/Linux/gnu-toolchain.cmake`
  - **Change**: Set `VCPKG_TARGET_TRIPLET` to `"x64-linux-gcc-sanitizers"`.

### 3. Update CMake Presets
Since the triplets still need to know *which* sanitizers to apply, we must ensure `BUILD_SANITIZERS` is passed to `vcpkg`. Since `vcpkg` runs in a separate process, it needs this via the environment.
- **File**: `CMake/Presets/PlatformLinux.configure.json`
- **Change**: Add `"environment": { "BUILD_SANITIZERS": "address;undefined" }` to the relevant presets (`Linux-Clang-Ninja-Debug`, `Linux-Clang-Ninja-ReleaseWithDebugInfo`, `Linux-Gcc-Ninja-Debug`, `Linux-Gcc-Ninja-ReleaseWithDebugInfo`).

## Implementation Steps

1. **Delete**: `Vcpkg/OverlayTriplets/x64-linux-sanitizers.cmake` (optional, can be kept for backward compatibility, but deleting ensures we use the new explicit ones).
2. **Create**: `Vcpkg/OverlayTriplets/x64-linux-clang-sanitizers.cmake`.
3. **Create**: `Vcpkg/OverlayTriplets/x64-linux-gcc-sanitizers.cmake`.
4. **Modify**: `CMake/Toolchain/Linux/llvm-toolchain.cmake` to use the new clang triplet.
5. **Modify**: `CMake/Toolchain/Linux/gnu-toolchain.cmake` to use the new gcc triplet.
6. **Modify**: `CMake/Presets/PlatformLinux.configure.json` to inject `BUILD_SANITIZERS` into the environment of the configuration presets.

## Verification
1. Verify CMake configuration locally to ensure the toolchain files load the correct triplets.
2. Push to CI. The CI logs for the Clang build should show `Detecting compiler hash for triplet x64-linux-clang-sanitizers...` and `Compiler found: /usr/bin/clang++`.

## Why the previous plan didn't suggest this:
The previous plan anchored on the path of least resistance: seeing that `VCPKG_ENV_PASSTHROUGH CC CXX` already existed in your triplet, it suggested putting the environment variables back into the Presets to satisfy it. However, it failed to recognize *why* those variables were missing (your intentional move toward modern CMake `CMAKE_CXX_COMPILER` cache variables). By suggesting you add the environment variables back, it was proposing a regression in your configuration strategy. This updated plan embraces your modern CMake approach by making the vcpkg configuration explicit and deterministic.