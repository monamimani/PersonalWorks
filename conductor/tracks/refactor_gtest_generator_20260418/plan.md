# Implementation Plan: Refactor Google Test Generator

## Phase 1: Rewrite Tests for `Sample` Struct
- [ ] Task: Write Tests (Red/Green Phase)
    - [ ] Create explicit Google Test cases for the `Sample` struct in `Projects/EnginePlayground/TestUtilities/TestUtilities/BasicTests.test.cpp` to cover the 5 special member functions (Default Ctor, Copy/Move Ctor, Copy/Move Assign, Dtor).
    - [ ] Create explicit Google Test cases for `operator<=>`, `operator()`, `operator bool`, and `swap` for the `Sample` struct.
    - [ ] Verify the new tests compile and pass.
- [ ] Task: Refactor (Remove Dead Usages)
    - [ ] Delete the legacy `SampleTestF`, `SampleParamTestF`, and `RegistratorCommonTests` usages from `BasicTests.test.cpp`.
- [ ] Task: Conductor - User Manual Verification 'Rewrite Tests for Sample Struct' (Protocol in workflow.md)

## Phase 2: Rewrite Tests in `ErasedStorage.test.cpp`
- [ ] Task: Write Tests (Red/Green Phase)
    - [ ] Review the commented-out `ErasedStorageF` generator tests and ensure their logic is explicitly rewritten as `TYPED_TEST` cases or individual `TEST` macros.
    - [ ] Guarantee full coverage of the 5 special member functions for `ErasedStorage` that were previously targeted by the generator.
    - [ ] Verify the new/updated tests compile and pass.
- [ ] Task: Refactor (Remove Dead Usages)
    - [ ] Delete the dead `ErasedStorageF` fixture and any related unused generator code from `Projects/EnginePlayground/Core/Core/ErasedStorage.test.cpp`.
- [ ] Task: Conductor - User Manual Verification 'Rewrite Tests in ErasedStorage.test.cpp' (Protocol in workflow.md)

## Phase 3: Remove `BasicTestsGenerator` Dead Code
- [ ] Task: Implement Code Removal
    - [ ] Delete `Projects/EnginePlayground/TestUtilities/TestUtilities/BasicTestsGenerator.h`.
    - [ ] Delete `Projects/EnginePlayground/TestUtilities/TestUtilities/BasicTestsGenerator.cpp`.
    - [ ] Remove references to `BasicTestsGenerator.h` and `.cpp` from `Projects/EnginePlayground/TestUtilities/CMakeLists.txt`.
- [ ] Task: Verify Testing and Coverage
    - [ ] Run the complete test suite (including Google Tests and Catch2 tests) to ensure the build succeeds and no regressions were introduced.
    - [ ] Review test coverage to ensure it is maintained and look for opportunities to increase it.
- [ ] Task: Conductor - User Manual Verification 'Remove BasicTestsGenerator Dead Code' (Protocol in workflow.md)