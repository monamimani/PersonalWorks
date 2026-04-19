# Implementation Plan: `refactor-delegate-tests_20260419`

#### **Phase 1: Foundation and Shared Utilities**
*   [x] Task: Create `DelegateTestUtils.h` and implement the base `DelegateTest` fixture.
*   [x] Task: Implement **Policy Objects** for binding types:
    *   `FreeFunctionPolicy`
    *   `MemberFunctionPolicy` (Const and Non-Const)
    *   `FunctorPolicy` (L-Value and R-Value)
    *   `OverloadTemplatePolicy`
*   [x] Task: Conductor - User Manual Verification 'Phase 1: Foundation and Shared Utilities' (Protocol in workflow.md)

#### **Phase 2: Single Delegate Tests Refactor**
*   [x] Task: **Write Failing Tests (Red Phase):** Create `Delegate.test.cpp` and define `TYPED_TEST` lifecycle tests (Copy, Move, Swap, Equality).
*   [x] Task: **Implement to Pass Tests (Green Phase):** Instantiate `TYPED_TEST` for the Single Delegate and verify coverage against existing tests.
*   [x] Task: Add specific functional tests for Single Delegate edge cases (Unbinding, Rebinding).
*   [x] Task: Conductor - User Manual Verification 'Phase 2: Single Delegate Tests Refactor' (Protocol in workflow.md)

#### **Phase 3: Multicast Delegate Tests Refactor**
*   [x] Task: **Write Failing Tests (Red Phase):** Create `MulticastDelegate.test.cpp` and define `TYPED_TEST` lifecycle tests.
*   [x] Task: **Implement to Pass Tests (Green Phase):** Instantiate `TYPED_TEST` for Multicast Delegate.
*   [x] Task: Implement specific tests for Multicast behavior (Multiple bindings, Invocation order, Removing handlers).
*   [x] Task: Conductor - User Manual Verification 'Phase 3: Multicast Delegate Tests Refactor' (Protocol in workflow.md)

#### **Phase 4: New Coverage and Cleanup**
*   [x] Task: Add explicit tests for **Lambdas with captures** and other edge cases discovered during refactor.
*   [x] Task: **Refactor (Optional but Recommended):** Verify all tests are readable and well-documented.
*   [x] Task: Remove `DelegateCommon.test.h` and `DelegateCommonTestCode.h`.
*   [x] Task: Conductor - User Manual Verification 'Phase 4: New Coverage and Cleanup' (Protocol in workflow.md)
