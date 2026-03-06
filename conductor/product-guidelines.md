# Product Guidelines

## Engineering Principles
- **Modern C++ First:** Strictly adhere to the latest C++ features, avoiding deprecated paradigms.
- **Optimal Performance:** Write optimal code by avoiding premature pessimization.
- **Strict Modularity:** Enforce strong modularity by keeping experimental code isolated from the core engine.
- **Cross-Platform Mindset:** Always design and write code with cross-platform compatibility in mind.

## Testing & Validation
- **Balanced Testing:** Maintain a healthy amount of unit tests and smoke tests using gtest/Catch2.
- **Performance Tracking:** Produce benchmarks when necessary to ensure there are no regressions in performance.
- **Environment Validation:** Rely on CI/CD pipelines or devcontainers to validate cross-platform compatibility.

## Code Style & Format
- **Strict Clang-Format:** Strictly enforce formatting using the project's existing `.clang-format` configuration to maintain consistency.

## Documentation
- **High-Level Architecture:** Maintain high-level architecture documents (like READMEs) to explain the system's design.
- **Self-Documenting Code:** Keep inline comments minimal; rely primarily on self-documenting code and clear function/variable names.
- **Targeted Doxygen:** Require minimal Doxygen-style comments specifically for main public APIs and core engine components.