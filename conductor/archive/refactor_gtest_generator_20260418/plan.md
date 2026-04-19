# Implementation Plan: Refactor Google Test Generator

## Phase 1: Rewrite Tests for `Sample` Struct
- [x] Task: Write Tests (Red/Green Phase)
    - [x] Create explicit Google Test cases for the `Sample` struct in `Projects/EnginePlayground/TestUtilities/TestUtilities/BasicTests.test.cpp` to cover the 5 special member functions (Default Ctor, Copy/Move Ctor, Copy/Move Assign, Dtor).
    - [x] Create explicit Google Test cases for `operator<=>`, `operator()`, `operator bool`, and `swap` for the `Sample` struct.
    - [x] Verify the new tests compile and pass.
- [x] Task: Refactor (Remove Dead Usages)
    - [x] Delete the legacy `SampleTestF`, `SampleParamTestF`, and `RegistratorCommonTests` usages from `BasicTests.test.cpp`.
- [x] Task: Conductor - User Manual Verification 'Rewrite Tests for Sample Struct' (Protocol in workflow.md)

## Phase 2: Rewrite Tests in `ErasedStorage.test.cpp`
- [x] Task: Write Tests (Red/Green Phase)
    - [x] Review the commented-out `ErasedStorageF` generator tests and ensure their logic is explicitly rewritten as `TYPED_TEST` cases or individual `TEST` macros.
    - [x] Guarantee full coverage of the 5 special member functions for `ErasedStorage` that were previously targeted by the generator.
    - [x] Verify the new/updated tests compile and pass.
- [x] Task: Refactor (Remove Dead Usages)
    - [x] Delete the dead `ErasedStorageF` fixture and any related unused generator code from `Projects/EnginePlayground/Core/Core/ErasedStorage.test.cpp`.
- [x] Task: Conductor - User Manual Verification 'Rewrite Tests in ErasedStorage.test.cpp' (Protocol in workflow.md)

## Phase 3: Remove `BasicTestsGenerator` Dead Code
- [x] Task: Implement Code Removal
    - [x] Delete `Projects/EnginePlayground/TestUtilities/TestUtilities/BasicTestsGenerator.h`.
    - [x] Delete `Projects/EnginePlayground/TestUtilities/TestUtilities/BasicTestsGenerator.cpp`.
    - [x] Remove references to `BasicTestsGenerator.h` and `.cpp` from `Projects/EnginePlayground/TestUtilities/CMakeLists.txt`.
- [x] Task: Verify Testing and Coverage
    - [x] Run the complete test suite (including Google Tests and Catch2 tests) to ensure the build succeeds and no regressions were introduced.
    - [x] Review test coverage to ensure it is maintained and look for opportunities to increase it.
- [x] Task: Conductor - User Manual Verification 'Remove BasicTestsGenerator Dead Code' (Protocol in workflow.md)