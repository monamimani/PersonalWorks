
function InvokeVcVarsAll {

  $vsPath = GetVsInstallationPath
  $vcvarsPath = [IO.Path]::Combine($vsPath, "VC", "Auxiliary", "Build", "vcvars64.bat")
  Write-Output "vcvarsPath: $vcvarsPath"
  if (Test-Path $vcvarsPath) {

    $envLines = "call `"$vcvarsPath`" -vcvars_ver=Preview >nul 2>&1`r`nset" | cmd.exe
    foreach ($line in $envLines) {
      Write-Output "Environment variable: $line"
      if ($line -match "^([^=]+)=(.*)$") {
        [Environment]::SetEnvironmentVariable($matches[1], $matches[2], [EnvironmentVariableTarget]::Process)
      }
    }
  }
  else {
    Write-Error "Could not find vcvars64.bat at $vcvarsPath"
  }
}
function LaunchVsDevShell {
  $vsPath = GetVsInstallationPath
  $vcvarsallPath = [IO.Path]::Combine($vsPath, "Common7", "Tools", "Launch-VsDevShell.ps1")

  # Launch-VsDevShell.ps1 does not support specifying the toolset version.
  # We use dot-sourcing so it applies to the current scope.
  . $vcvarsallPath -Arch amd64 -SkipAutomaticLocation
}

function GetVsInstallationPath {
  Set-PSRepository -Name "PSGallery" -InstallationPolicy Trusted
  Install-Module VSSetup -Scope CurrentUser
  Set-PSRepository -Name "PSGallery" -InstallationPolicy UnTrusted

  # Use -All and -Prerelease to include all instances, including preview/insiders
  $instances = Get-VSSetupInstance -All -Prerelease
  [Console]::WriteLine("Instances:")
  [Console]::WriteLine(($instances | Format-List * | Out-String))
  # Try to find an instance that explicitly has the 14.51 component first
  $vsPath = $instances | Select-VSSetupInstance -Require 'Microsoft.VisualStudio.Component.VC.Preview.Tools.x86.x64' -Latest
  [Console]::WriteLine("VSPath: $($vsPath.InstallationPath)")
  # If not found, try to look for the generic toolset in an instance whose path contains "Insiders"
  if (-not $vsPath) {
    $vsPath = $instances | Select-VSSetupInstance -Require 'Microsoft.VisualStudio.Component.VC.Tools.x86.x64' -Latest
  }

  return $vsPath.InstallationPath
}
