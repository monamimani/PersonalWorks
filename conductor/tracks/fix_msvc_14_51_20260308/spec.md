# Specification: Fix Windows CI MSVC 14.51 & Chocolatey Caching

## Overview
The Windows CI workflow (`.github/workflows/ci.yml`) is currently unable to install the MSVC 14.51 (v145) compiler, and the installation process via Chocolatey is excessively slow. This track aims to ensure the correct compiler is installed and optimize the process using improved caching and installation methods.

## Functional Requirements
1. **MSVC 14.51 Installation:** Update the CI workflow to explicitly include the MSVC 14.51 (v145) toolset. This may involve adding the specific component ID `Microsoft.VisualStudio.Component.VC.14.51.x86.x64` to the installer parameters.
2. **Chocolatey Caching:** Optimize Chocolatey to reduce the installation time by caching downloaded installers and package metadata in the GitHub Actions runner.
3. **Runner Configuration:** Ensure the `windows-2025-vs2026` runner is correctly configured to find and use the preview tools during the build process.

## Non-Functional Requirements
- **Efficiency:** Significantly reduce the duration of the "Install Visual Studio Build Tools" step in the Windows CI.
- **Reliability:** Ensure the build consistently uses the intended compiler version (MSVC 14.51).

## Acceptance Criteria
1. The Windows CI build successfully identifies and uses MSVC 14.51 (or a compatible v145 toolset).
2. Subsequent CI runs show a reduced duration for the installation step due to effective caching.
3. The overall build on the Windows runner completes successfully.

## Out of Scope
- Updating Linux-based compilers (Clang/Gcc).
- General codebase refactoring or unrelated CI improvements.