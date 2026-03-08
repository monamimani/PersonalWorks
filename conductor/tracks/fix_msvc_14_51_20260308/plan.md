# Implementation Plan: Fix Windows CI MSVC 14.51 & Chocolatey Caching

## Phase 1: Update Windows CI for MSVC 14.51 and Improve Caching
- [ ] Task: Investigate current runner environment
    - [ ] Add a step to `.github/workflows/ci.yml` to list installed Visual Studio components using `vswhere.exe`.
    - [ ] Confirm if MSVC 14.51 (v145) is already present or truly missing.
- [ ] Task: Explicitly Install MSVC 14.51
    - [ ] Update the `Install Visual Studio Build Tools` step in `.github/workflows/ci.yml`.
    - [ ] Add `--add Microsoft.VisualStudio.Component.VC.14.51.x86.x64` to the installation command.
    - [ ] Evaluate replacing `choco install` with a direct `vs_installer.exe` call if Chocolatey remains slow or unreliable.
- [ ] Task: Optimize Chocolatey Caching
    - [ ] Update the `Chocolatey Cache` step to include the Chocolatey download/temp directory (`${{ env.LOCALAPPDATA }}\Temp\chocolatey`).
    - [ ] Ensure the cache key includes the workflow file hash to trigger refreshes on configuration changes.
- [ ] Task: Verify MSVC 14.51 Usage
    - [ ] Trigger a CI run on the `windows-2025-vs2026` runner.
    - [ ] Inspect logs to confirm the compiler version detected by CMake and the execution of `Scripts\Build\VsEnvironment.ps1`.
- [ ] Task: Conductor - User Manual Verification 'Phase 1: Update Windows CI for MSVC 14.51 and Improve Caching' (Protocol in workflow.md)

## Phase 2: Finalize and Validate
- [ ] Task: Confirm Cache Hits
    - [ ] Run the CI pipeline multiple times to verify that the Chocolatey cache is being successfully restored and uploaded.
    - [ ] Ensure the "Install Visual Studio Build Tools" step shows a significant speedup.
- [ ] Task: Clean up and Commit
    - [ ] Remove any temporary debugging steps from `.github/workflows/ci.yml`.
    - [ ] Finalize the workflow configuration and commit changes.
- [ ] Task: Conductor - User Manual Verification 'Phase 2: Finalize and Validate' (Protocol in workflow.md)