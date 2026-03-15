# Objective
Fix the CI configuration failure for the `Windows-Msvc-NinjaMultiConfig-Debug` preset caused by passing an unsupported toolset specification to the `Ninja Multi-Config` generator.

# Background & Motivation
The log shows that the build fails during the CMake configuration step:
```
CMake Error at CMakeLists.txt:24 (project):
  Generator
    Ninja Multi-Config
  does not support toolset specification, but toolset
    v145,host=x64,version=14.51
  was specified.
```
In `CMakePresets.json` (specifically `CMake/Presets/Generators.json`), the `NinjaMultiConfig` preset is defined, but it lacks the `architecture` and `toolset` overrides that the `Ninja` preset has.
When the `Windows-Msvc-NinjaMultiConfig` preset is evaluated, it inherits from the `Msvc` preset (which defines a specific MSVC toolset string with the default `"strategy": "set"`) and `NinjaMultiConfig`. Because `NinjaMultiConfig` does not override the toolset strategy to `"external"`, CMake attempts to pass the `-T v145,host=x64,version=14.51` flag to the `Ninja Multi-Config` generator, which it does not support, causing the configuration to fail.

# Scope & Impact
- **Impacted Files**: `CMake/Presets/Generators.json`
- This fix will align the `NinjaMultiConfig` generator preset with the `Ninja` generator preset, ensuring CMake knows that architecture and toolset selection is handled externally (by the compiler/environment) when using Ninja generators, preventing the `-T` and `-A` flags from being passed.

# Proposed Solution
Modify the `NinjaMultiConfig` object in `CMake/Presets/Generators.json` to include the same `architecture` and `toolset` definitions (with `"strategy": "external"`) as the `Ninja` object.

# Implementation Steps
1. Modify `CMake/Presets/Generators.json`.
2. Update the `NinjaMultiConfig` preset definition from:
   ```json
    {
      "name": "NinjaMultiConfig",
      "hidden": true,
      "generator": "Ninja Multi-Config"
    },
   ```
   to:
   ```json
    {
      "name": "NinjaMultiConfig",
      "hidden": true,
      "generator": "Ninja Multi-Config",
      "architecture": {
        "value": "x64",
        "strategy": "external"
      },
      "toolset": {
        "value": "x64",
        "strategy": "external"
      }
    },
   ```

# Verification
By setting `"strategy": "external"`, CMake will drop the `-T` flag when configuring the project with the `Ninja Multi-Config` generator. This will resolve the CI failure without impacting Visual Studio generator presets (which correctly use `"strategy": "set"`).
