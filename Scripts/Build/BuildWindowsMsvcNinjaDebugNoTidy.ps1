. Scripts\Build\VsEnvironment.ps1; InvokeVcVarsAll

cmake --preset Windows-Msvc-Ninja-Debug-NoTidy
cmake --build --preset Windows-Msvc-Ninja-Debug-NoTidy
