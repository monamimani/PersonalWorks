
function InvokeVcVarsAll
{
  param (
  [Parameter()]
  $Arch = 'x64',

  [Parameter(ValueFromRemainingArguments=$true)]
  $Arguments
  )

    Set-PSRepository -Name "PSGallery" -InstallationPolicy Trusted 
    Install-Module VSSetup -Scope CurrentUser
    Set-PSRepository -Name "PSGallery" -InstallationPolicy UnTrusted 
    $vsPath =Get-VSSetupInstance | Select-VSSetupInstance -Require 'Microsoft.VisualStudio.Component.VC.Tools.x86.x64' -Latest
    $vcvarsallPath = [IO.Path]::Combine($vsPath.InstallationPath, "VC", "Auxiliary", "Build", "vcvarsall.bat")
    
    #write-output($vcvarsallPath)
    
    if($vcvarsallPath) {
        cmd /s /c """$vcvarsallPath"" $Arch && set" | where { $_ -match '(\w+)=(.*)' } | foreach {
            $null = new-item -force -path "Env:\$($Matches[1])" -value $Matches[2]
        }
    }

    #Get-ChildItem env:
}