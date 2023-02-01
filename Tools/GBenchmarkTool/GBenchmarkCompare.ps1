
$ExecutionPolicy = "RemoteSigned" 
If ((Get-ExecutionPolicy) -ne $ExecutionPolicy) { 
    Get-ExecutionPolicy -List 
    Write-Output "Changing the Execution policy for the process scope to: $ExecutionPolicy"
    Set-ExecutionPolicy -ExecutionPolicy $ExecutionPolicy -Scope Process -Force
}

Push-Location ./code

$VenvDir = "$PSScriptRoot/.venv"

if (-Not $(Test-Path $VenvDir)) {
    Write-Host "Creating environment $VenvDir"
    py -m venv $VenvDir
}

(Get-Command py).Path

py -m pip install -r requirements.txt

&$VenvDir\Scripts\Activate.ps1

$BuildDin = "$PSScriptRoot\..\..\_Out\build"

&py compare.py filters "$BuildDin\windows-vs2022-checkers\_bin\RelWithDebInfo\CoreBenchmarks.exe" DelegateCompare DelegateCompare

&deactivate

Pop-Location