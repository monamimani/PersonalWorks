# Track Specification: `refactor-delegate-tests_20260419`

#### **1. Overview**
The current `Delegate` tests rely on a complex, macro-heavy "test generation" system (`DelegateCommon.test.h` and `DelegateCommonTestCode.h`) that is difficult to read, debug, and navigate within IDEs. This track will refactor these tests into an idiomatic GTest structure using `TYPED_TEST` and **Policy Objects** for binding logic. This will improve readability while maintaining full coverage across all binding types (Free Functions, Member Functions, Functors, and Lambdas).

#### **2. Functional Requirements**
*   **Split Tests by Type:** Separate tests into `Delegate.test.cpp` and `MulticastDelegate.test.cpp` for better navigation.
*   **GTest Native Lifecycle Tests:** Use `TYPED_TEST` to implement shared lifecycle tests (Copy, Move, Swap, Equality) across both delegate types.
*   **Policy-Based Binding:** Replace the current `bind()` switch-case with a set of small, focused **Policy Objects** (e.g., `FreeFunctionPolicy`, `MemberFunctionPolicy`, `FunctorPolicy`, `LambdaPolicy`).
*   **Preserve & Expand Coverage:**
    *   **Value Categories:** Test L-Value vs R-Value functor binding.
    *   **Const-ness:** Test binding to `const` vs non-const member functions.
    *   **Advanced Bindings:** Ensure template and overloaded member functions are tested.
    *   **New Feature:** Add explicit tests for **Lambdas with captures** (which are currently obfuscated or missing).

#### **3. Non-Functional Requirements**
*   **Self-Documenting Code:** Tests should be readable at a glance without jumping between multiple files or parsing macros.
*   **Code Reduction:** Substantially reduce the complexity and line count of the "test infrastructure" code.
*   **IDE Friendly:** All tests must appear clearly in standard IDE Test Explorers.

#### **4. Acceptance Criteria**
1.  All existing delegate test coverage is maintained and verified passing in the new structure.
2.  New tests for "Lambdas with captures" are implemented and passing.
3.  The files `DelegateCommon.test.h` and `DelegateCommonTestCode.h` are removed from the project.
4.  A developer can modify a test case and see the change reflected in the IDE without full-project rebuilds.

#### **5. Out of Scope**
*   Modifying the internal implementation of the `Delegate` classes (unless a bug is found).
*   Refactoring other unrelated components in the `Core` library.
