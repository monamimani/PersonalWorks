# Specification: Fix vcpkg x-gha binary caching warning in GitHub Actions

## Overview
The goal of this track is to migrate from the deprecated `x-gha` binary caching backend for vcpkg to a NuGet-based binary caching provider in GitHub Actions. This will resolve warnings and ensure consistent binary caching across Windows and Linux runners.

## Requirements
- **Windows Runners:** Strictly follow the [Windows-specific NuGet-based binary caching tutorial](https://learn.microsoft.com/en-us/vcpkg/consume/binary-caching-github-packages?pivots=windows-runner).
- **Linux Runners (dev_container):** Strictly follow the [Linux-specific NuGet-based binary caching tutorial](https://learn.microsoft.com/en-us/vcpkg/consume/binary-caching-github-packages?pivots=linux-runner).
- **NuGet Dependency (Linux):** Use `vcpkg fetch nuget` to download a standalone NuGet executable that doesn't require a full mono installation in the dev_container.
- **Token Management:** Inject the `GITHUB_TOKEN` directly into the workflow steps that interact with the NuGet cache.
- **Environment:** Assume `VCPKG_INSTALLATION_ROOT` is set and valid.

## Functional Requirements
- Remove all references to `x-gha` in the `.github/actions/vcpkg_cache/` and related workflow files.
- Configure `VCPKG_BINARY_SOURCES` to use the NuGet source: `clear;nuget,GitHub,readwrite`.
- Add a step to configure the NuGet source using `vcpkg fetch nuget` and `nuget config`.
- Ensure the `GITHUB_TOKEN` is correctly passed for authentication.

## Acceptance Criteria
- CI/CD pipeline runs successfully on both Windows and Linux without `x-gha` warnings.
- Binaries are successfully uploaded to and retrieved from GitHub Packages during workflow runs.

## References
- [Binary caching GitHub Packages (Windows Runner)](https://learn.microsoft.com/en-us/vcpkg/consume/binary-caching-github-packages?pivots=windows-runner)
- [Binary caching GitHub Packages (Linux Runner)](https://learn.microsoft.com/en-us/vcpkg/consume/binary-caching-github-packages?pivots=linux-runner)
