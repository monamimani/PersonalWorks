# Implementation Plan: Fix vcpkg x-gha binary caching warning in GitHub Actions

## Phase 1: Update vcpkg caching configuration in GitHub Actions
- [x] Task: Investigate existing usage of `x-gha`
    - [x] Locate references to `x-gha` or `VCPKG_BINARY_SOURCES` in the `.github` directory.
- [x] Task: Implement NuGet-based GitHub Packages caching
    - [x] Update workflow files to configure vcpkg caching with `clear;nuget,GitHub,readwrite`.
    - [x] Provide the `GITHUB_TOKEN` to the NuGet configuration step.
    - [x] Add permissions: packages: write to ci.yml.
    - [x] Use `vcpkg fetch nuget` as per Microsoft documentation for both Windows and Linux.
- [~] Task: Verify caching functionality
    - [ ] Run the CI/CD pipeline on a branch to verify the warning is gone and caching works.
- [ ] Task: Conductor - User Manual Verification 'Phase 1: Update vcpkg caching configuration in GitHub Actions' (Protocol in workflow.md)