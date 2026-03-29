# Implementation Plan: Simplify VS Component Installation

## Phase 1: Implement VS Installer CLI in GitHub Actions
- [ ] Task: Locate VS Installation Path
    - [ ] Update `.github/workflows/ci.yml` to use `vswhere.exe` to dynamically find the installation path of Visual Studio Enterprise on the runner.
- [ ] Task: Replace Chocolatey with VS Installer CLI
    - [ ] Remove the Chocolatey installation step for the VS components in `.github/workflows/ci.yml`.
    - [ ] Add a new PowerShell step to run the VS Installer (`vs_installer.exe`) with the `modify` command to add the `Microsoft.VisualStudio.Component.VC.Preview.Tools.x86.x64` component.
    - [ ] Ensure the installer command includes necessary arguments for headless execution (e.g., `--quiet`, `--wait`, `--norestart`).
- [ ] Task: Conductor - User Manual Verification 'Phase 1: Implement VS Installer CLI in GitHub Actions' (Protocol in workflow.md)

## Phase 2: Update VS Environment Script & Verification
- [ ] Task: Review and Update `VsEnvironment.ps1`
    - [ ] Review `Scripts/Build/VsEnvironment.ps1` to ensure it correctly identifies the updated Visual Studio instance.
    - [ ] While we will keep the `VSSetup` module dependency, verify if any simplifications can be made to correctly locate the instance with the new preview component.
    - [ ] Ensure `InvokeVcVarsAll` correctly sets up the environment for the preview toolset.
- [ ] Task: Verify CI Pipeline
    - [ ] Trigger the CI workflow on a test branch.
    - [ ] Confirm the component installs correctly, the environment script runs successfully, and the CMake configure/build steps succeed.
- [ ] Task: Conductor - User Manual Verification 'Phase 2: Update VS Environment Script & Verification' (Protocol in workflow.md)