
function InvokeVcVarsAll {
  param (
    [Parameter()]
    $Arch = 'x64',

    [Parameter(ValueFromRemainingArguments = $true)]
    $Arguments
  )

  $vsPath = GetVsInstallationPath
  $vcvarsallPath = [IO.Path]::Combine($vsPath, "VC", "Auxiliary", "Build", "vcvarsall.bat")
    
  #write-output($vcvarsallPath)
    
  if ($vcvarsallPath) {
    cmd /s /c """$vcvarsallPath"" $Arch && set" | where { $_ -match '(\w+)=(.*)' } | foreach {
      $null = new-item -force -path "Env:\$($Matches[1])" -value $Matches[2]
    }
  }

  #Get-ChildItem env:
}

function LaunchVsDevShell {
  #Get-ChildItem env:
  $vsPath = GetVsInstallationPath
  $vcvarsallPath = [IO.Path]::Combine($vsPath, "Common7", "Tools", "Launch-VsDevShell.ps1")
  #write-output($vcvarsallPath)
  & $vcvarsallPath -Arch amd64 -SkipAutomaticLocation
  #Get-ChildItem env:
}

function GetVsInstallationPath {
  Set-PSRepository -Name "PSGallery" -InstallationPolicy Trusted 
  Install-Module VSSetup -Scope CurrentUser
  Set-PSRepository -Name "PSGallery" -InstallationPolicy UnTrusted 

  # Use -All and -Prerelease to include all instances, including preview/insiders
  $instances = Get-VSSetupInstance -All -Prerelease
  
  # 1. Try to find an instance that explicitly has the 14.51 component first
  $vsPath = $instances | Select-VSSetupInstance -Require 'Microsoft.VisualStudio.Component.VC.14.51.x86.x64' -Latest
  
  # 2. If not found, try to look for the "Visual Studio Build Tools 2026 Preview" specifically
  # We check the version starts with 18 to ensure it's the right preview
  if (-not $vsPath) {
    $vsPath = $instances | Where-Object { 
      $_.Product.Id -eq "Microsoft.VisualStudio.Product.BuildTools" -and 
      $_.InstallationVersion.Major -eq 18 
    } | Select-Object -First 1
  }

  # 3. Fallback to any latest instance with generic VC tools (might pick up Enterprise 18)
  if (-not $vsPath) {
    $vsPath = $instances | Select-VSSetupInstance -Require 'Microsoft.VisualStudio.Component.VC.Tools.x86.x64' -Latest
  }

  return $vsPath.InstallationPath
}