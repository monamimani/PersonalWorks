# Specification: Simplify VS Component Installation

## Overview
This track aims to simplify the installation of the `Microsoft.VisualStudio.Component.VC.Preview.Tools.x86.x64` component in the GitHub Actions CI/CD pipeline (`.github/workflows/ci.yml`). The current implementation uses Chocolatey, which adds unnecessary overhead. We will transition to using the native Visual Studio Installer CLI.

## Functional Requirements
- **Native Installation:** Use the Visual Studio Installer CLI (`vs_installer.exe` or `setup.exe` found via `vswhere`) to add the component directly to the pre-installed Visual Studio Enterprise instance.
- **Verification:** Ensure the component is correctly installed and the build process (CMake/MSVC) succeeds.
- **Cleanup:** Remove Chocolatey-based installation steps for this specific component from the workflow.
- **Idempotency:** The installation script/step should handle cases where the component might already be installed without failing the build.

## Non-Functional Requirements
- **Performance:** Reduce total CI/CD runtime by avoiding Chocolatey overhead. Note: We will *not* attempt to cache the VS system installation, relying instead on the speed of adding a single component to an existing install.
- **Maintainability:** Use clean, readable scripting within the `.github/workflows/ci.yml`.

## Acceptance Criteria
1. The `Microsoft.VisualStudio.Component.VC.Preview.Tools.x86.x64` component is successfully installed using the native VS Installer CLI.
2. The CI/CD pipeline passes all build steps.
3. The `.github/workflows/ci.yml` file no longer uses Chocolatey for this component.

## Out of Scope
- Caching the Visual Studio installation via `actions/cache`.
- Modifying non-Windows CI workflows.