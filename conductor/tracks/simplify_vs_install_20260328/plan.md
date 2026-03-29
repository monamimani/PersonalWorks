# Implementation Plan: Simplify VS Component Installation

## Phase 1: Implement VS Installer CLI in GitHub Actions
- [x] Task: Locate VS Installation Path
    - [x] Update `.github/workflows/ci.yml` to use `vswhere.exe` to dynamically find the installation path of Visual Studio Enterprise on the runner.
- [x] Task: Replace Chocolatey with VS Installer CLI
    - [x] Remove the Chocolatey installation step for the VS components in `.github/workflows/ci.yml`.
    - [x] Add a new PowerShell step to run the VS Installer (`vs_installer.exe`) with the `modify` command to add the `Microsoft.VisualStudio.Component.VC.Preview.Tools.x86.x64` component.
    - [x] Ensure the installer command includes necessary arguments for headless execution (e.g., `--quiet`, `--wait`, `--norestart`).
- [x] Task: Conductor - User Manual Verification 'Phase 1: Implement VS Installer CLI in GitHub Actions' (Protocol in workflow.md)

## Phase 2: Update VS Environment Script & Verification
- [x] Task: Review and Update `VsEnvironment.ps1`
    - [x] Review `Scripts/Build/VsEnvironment.ps1` to ensure it correctly identifies the updated Visual Studio instance.
    - [x] While we will keep the `VSSetup` module dependency, verify if any simplifications can be made to correctly locate the instance with the new preview component.
    - [x] Ensure `InvokeVcVarsAll` correctly sets up the environment for the preview toolset.
- [x] Task: Verify CI Pipeline
    - [x] Trigger the CI workflow on a test branch.
    - [x] Confirm the component installs correctly, the environment script runs successfully, and the CMake configure/build steps succeed.
- [x] Task: Conductor - User Manual Verification 'Phase 2: Update VS Environment Script & Verification' (Protocol in workflow.md)