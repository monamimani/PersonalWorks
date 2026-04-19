
function InvokeVcVarsAll {

  $vsPath = GetVsInstallationPath
  $vcvarsPath = [IO.Path]::Combine($vsPath, "VC", "Auxiliary", "Build", "vcvars64.bat")
  if (Test-Path $vcvarsPath) {

    $envLines = "call `"$vcvarsPath`" -vcvars_ver=Preview >nul 2>&1`r`nset" | cmd.exe
    foreach ($line in $envLines) {
      if ($line -match "^([^=]+)=(.*)$") {
        [Environment]::SetEnvironmentVariable($matches[1], $matches[2], [EnvironmentVariableTarget]::Process)
      }
    }
  }
  else {
    Write-Error "Could not find vcvars64.bat at $vcvarsPath"
  }
}

function PatchAndLaunchVsDevShell {
  $vsPath = GetVsInstallationPath
  $launchVsDevShell = [IO.Path]::Combine($vsPath, "Common7", "Tools", "Launch-VsDevShell.ps1")
  
  if (Test-Path $launchVsDevShell) {
    Write-Host "Patching Launch-VsDevShell.ps1 at $launchVsDevShell..."
    $content = Get-Content -Raw $launchVsDevShell
    $paramRegex = 'VsInstanceId = \$instanceId'
    $paramSubst = '$&; DevCmdArguments = "-vcvars_ver=preview";'
    
    if ($content -match $paramRegex -and -not ($content -match 'DevCmdArguments = "-vcvars_ver=preview"')) {
      $newContent = $content -creplace $paramRegex, $paramSubst
      Set-Content -Path $launchVsDevShell -Value $newContent -NoNewLine -Force
      Write-Host "Successfully patched Launch-VsDevShell.ps1"
    } else {
      Write-Host "Launch-VsDevShell.ps1 already patched or regex not found."
    }
  } else {
    Write-Error "Could not find Launch-VsDevShell.ps1 at $launchVsDevShell"
  }

  . $launchVsDevShell -Arch amd64 -SkipAutomaticLocation
}

function LaunchVsDevShell {
  $vsPath = GetVsInstallationPath
  $launchVsDevShell = [IO.Path]::Combine($vsPath, "Common7", "Tools", "Launch-VsDevShell.ps1")

  # Launch-VsDevShell.ps1 does not support specifying the toolset version.
  # We use dot-sourcing so it applies to the current scope.
  . $launchVsDevShell -Arch amd64 -SkipAutomaticLocation -Preview
}

function GetVsInstallationPath {
  if (-not (Get-Module -ListAvailable -Name VSSetup)) {
    Write-Host "Installing VSSetup module..."
    Set-PSRepository -Name "PSGallery" -InstallationPolicy Trusted
    Install-Module VSSetup -Scope CurrentUser -Force
    Set-PSRepository -Name "PSGallery" -InstallationPolicy UnTrusted
  }

  # Use -All and -Prerelease to include all instances, including preview/insiders
  $instances = Get-VSSetupInstance -All -Prerelease

  # Try to find an instance that explicitly has the 14.51 component first
  $vsPath = $instances | Select-VSSetupInstance -Product * -Require 'Microsoft.VisualStudio.Component.VC.Preview.Tools.x86.x64' -Latest

  # If not found, try to look for the generic toolset in an instance whose path contains "Insiders"
  if (-not $vsPath) {
    $vsPath = $instances | Select-VSSetupInstance -Product * -Require 'Microsoft.VisualStudio.Component.VC.Tools.x86.x64' -Latest
  }

  return $vsPath.InstallationPath
}
