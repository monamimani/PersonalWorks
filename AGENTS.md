# Context

This document provides context to understand the structure and conventions of the `PersonalWorks` repository.

## Project Overview

This repository, `PersonalWorks`, is a C++ project that serves as a personal playground for developing and exploring various coding concepts. It is structured as a monorepo containing several sub-projects, each with a specific purpose. The project uses CMake for building and vcpkg for dependency management.

The main sub-projects are:

*   **EnginePlayground:** A long-term project to build a small game engine. This is the core of the repository.
*   **CodeIncubator:** A space to develop and test new ideas and classes in isolation before integrating them into the `EnginePlayground`.
*   **CodeExercises:** A collection of solutions to coding challenges from platforms like Codility and LeetCode.
*   **CppExploration & CppStdExploration:** Projects for exploring and learning new C++ concepts and standards.
*   **Config:** A utility project for configuration management.
*   **OnnxInferenceApp & OnnxRT:** Projects related to ONNX model inference.

The project is built using C++23 and latest C++ and is intended to be cross-platform, with CI/CD pipelines for both Windows and Linux.

## Building and Running

The project is built using CMake and the provided presets. The CI/CD pipeline in `.github/workflows/ci.yml` provides the exact commands for building and testing on different platforms.

**Key Dependencies:**

*   `gtest`: For unit testing.
*   `benchmark`: For performance testing.
*   `catch2`: For testing in some sub-projects.
*   `spdlog`: For logging.
*   `vulkan-headers-tmp` & `vulkan-loader-tmp`: For graphics programming.
*   `fuzztest`: For fuzz testing on Linux.

**Build Commands:**

The project uses CMake presets for configuration and building. The following are examples of how to build the project:

*   **Windows (MSVC):**
    ```bash
    # Configure
    . Scripts\Build\VsEnvironment.ps1; InvokeVcVarsAll
    cmake --preset Windows-Msvc-Ninja-Debug-NoTidy
    # Build
    cmake --build --preset Windows-Msvc-Ninja-Debug-NoTidy

    # or Workflow
    cmake --workflow Windows-Msvc-Ninja-Debug-NoTidy
    ```

*   **Linux (Clang):**
    ```bash
    # Configure
    cmake --preset Linux-Clang-Ninja-Debug-NoTidy
    # Build
    cmake --build --preset Linux-Clang-Ninja-Debug-NoTidy

    # or Workflow
    cmake --workflow Linux-Clang-Ninja-Debug-NoTidy
    ```

**Testing:**

Tests are enabled by default. The main testing frameworks are `gtest` and `catch2`.

## Development Conventions

*   **Coding Style:** The project enforces a strict coding style using `.editorconfig` and `.clang-format`. The style is based on the CppCoreGuidelines and uses 2-space indentation.
*   **Testing:** Tests are a key part of the development process. New features should be accompanied by unit tests. The `CodeIncubator` project has examples of how tests are structured.
*   **Dependencies:** Dependencies are managed using `vcpkg`. New dependencies should be added to the `vcpkg.json` file.
*   **Continuous Integration:** All code is built and tested on Windows and Linux via GitHub Actions. The workflow is defined in `.github/workflows/ci.yml`.

## Instructions for Google Jules

To work with the `PersonalWorks` repository, you need to execute as root the script `JulesEnvSetup.sh` located in the root of the repository. This script will set up the necessary environment for you to work on the project.



## Project Readme

@README.md
