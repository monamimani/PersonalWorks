# Specification: Refactor Google Test Generator to Explicit Tests

## Overview
Refactor the Google Test cases that relied on the complex `BasicTestsGenerator.h` mechanism. The generator code will be removed, and any tests previously using it will be rewritten using explicit Google Test macros to ensure full coverage of the 5 special member functions (Default Constructor, Copy/Move Constructor, Copy/Move Assignment, and Destructor).

## Functional Requirements
1. **Rewrite `BasicTests.test.cpp`:**
   - Replace the legacy `RegistratorCommonTests` with explicit `TEST` macros for the `Sample` struct.
   - Explicitly cover the Default Constructor, Copy Constructor, Move Constructor, Copy Assignment, Move Assignment, and Destructor.
   - Explicitly cover `operator<=>`, `operator()`, `operator bool`, and `swap`.

2. **Clean up `ErasedStorage.test.cpp`:**
   - Remove the dead `ErasedStorageF` fixture class, which was previously intended for use with the generator. The explicit `ErasedStorageTestStructF` (which uses `TYPED_TEST`) already exists in the file and provides the necessary coverage.

3. **Remove Dead Code:**
   - Delete `Projects/EnginePlayground/TestUtilities/TestUtilities/BasicTestsGenerator.h`
   - Delete `Projects/EnginePlayground/TestUtilities/TestUtilities/BasicTestsGenerator.cpp`
   - Remove references to these deleted files from `Projects/EnginePlayground/TestUtilities/CMakeLists.txt`.

## Non-Functional Requirements
- Maintain existing test coverage (no regressions).
- Enhance readability by adhering to standard Google Test idioms (explicit `TEST` macros).
- Increase test coverage for the refactored classes/functions if possible after the rewrite.

## Acceptance Criteria
- All tests in `BasicTests.test.cpp` and `ErasedStorage.test.cpp` pass successfully.
- Test coverage for `Sample` and the 5 special member functions is maintained or increased.
- `BasicTestsGenerator.h` and `BasicTestsGenerator.cpp` are completely removed from the repository and build system.
- No Catch2 tests are modified or affected.

## Out of Scope
- Catch2 tests are strictly out of scope.
- Adding new features or refactoring the core implementation of `ErasedStorage`.