# Fix CMake Visual Studio Generator with MSVC v14.51 Preview

## Background & Motivation

The project is currently using the MSVC v14.51 preview toolset via Visual Studio 2026 Build Tools installed on the GitHub Actions runner. 

According to the [MSVC v14.51 Preview Release Blog Post](https://devblogs.microsoft.com/cppblog/microsoft-c-msvc-build-tools-v14-51-preview-released-how-to-opt-in/), there is a known bug when using the **Visual Studio (MSBuild) generator** with CMake and the v14.51 preview toolset. CMake looks for version-specific MSBuild property files in a directory named `14.51`, but the preview installation currently places them in a generic `v145` directory. 

Because of this bug, CMake fails to generate the `.sln` and `.vcxproj` files correctly when the configure preset is set to `Windows-Msvc-VisualStudio`.

## Proposed Solution

To work around this bug, we need to manually create the expected `14.51` directory and copy the required `.props` and `.txt` files into it. 

This workaround must be executed:
1. **After** the `VsEnvironment.ps1` script is sourced, because we need the `$env:VCINSTALLDIR` environment variable to be populated.
2. **Only** for the `Windows-Msvc-VisualStudio` configuration, as Ninja generators do not rely on these MSBuild property files.

## Implementation Steps

### 1. Update `ci.yml`

Modify the `CMake Configure & Build` step in `.github/workflows/ci.yml` to include the file-copy workaround. Since the step uses PowerShell (`pwsh`), we will implement the workaround using PowerShell syntax.

**Proposed Changes:**
```yaml
      - name: "CMake Configure & Build"
        shell: pwsh
        run: |
          . Scripts\Build\VsEnvironment.ps1; InvokeVcVarsAll

          # Workaround for CMake Visual Studio Generator bug with MSVC v14.51 Preview
          if ('${{ matrix.configure-preset }}' -eq 'Windows-Msvc-VisualStudio') {
              Write-Host "Applying workaround for MSVC v14.51 Preview MSBuild properties..."
              
              $vcInstallDir = $env:VCINSTALLDIR
              if (-not $vcInstallDir -and $env:VCToolsInstallDir) {
                  $vcInstallDir = Resolve-Path (Join-Path $env:VCToolsInstallDir "..\..\..")
              }

              if ($vcInstallDir) {
                  $buildDir = Join-Path $vcInstallDir "Auxiliary\Build"
                  $targetDir = Join-Path $buildDir "14.51"
                  
                  if (-not (Test-Path $targetDir)) {
                      New-Item -ItemType Directory -Path $targetDir | Out-Null
                  }
                  
                  $srcProps = Join-Path $buildDir "v145\Microsoft.VCToolsVersion.VC.14.51.props"
                  $srcTxt = Join-Path $buildDir "v145\Microsoft.VCToolsVersion.VC.14.51.txt"
                  
                  if (Test-Path $srcProps) {
                      Copy-Item -Path $srcProps -Destination (Join-Path $targetDir "Microsoft.VCToolsVersion.14.51.props") -Force
                  }
                  if (Test-Path $srcTxt) {
                      Copy-Item -Path $srcTxt -Destination (Join-Path $targetDir "Microsoft.VCToolsVersion.14.51.txt") -Force
                  }
              } else {
                  Write-Warning "Could not find VCINSTALLDIR or VCToolsInstallDir. Workaround skipped."
              }
          }

          cmake --preset ${{matrix.configure-preset}} -DGIT_SHA:STRING=${{ github.sha }} -DPersonalWorks_ENABLE_CLANG_TIDY=OFF
          cmake --build --preset ${{matrix.build-preset}}
```

## Verification
1. Commit and push the changes.
2. Observe the GitHub Actions CI run.
3. Ensure the `Windows-Msvc-VisualStudio-Debug` job successfully configures and builds the project without the CMake generator error.
4. Ensure the `Ninja` and `NinjaMultiConfig` jobs remain unaffected and skip the workaround.