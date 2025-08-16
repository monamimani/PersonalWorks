# Personal Works

[![Build](https://github.com/monamimani/PersonalWorks/actions/workflows/ci.yml/badge.svg)](https://github.com/monamimani/PersonalWorks/actions/workflows/ci.yml)
[![codecov](https://codecov.io/github/monamimani/PersonalWorks/graph/badge.svg?token=KE8K6BZBEJ)](https://codecov.io/github/monamimani/PersonalWorks)

This repository is a personal C++23 playground for developing and exploring various coding concepts. It is structured as a monorepo, built with CMake, and uses vcpkg for dependency management.

## Projects

### Config

The Config project is common code that other project can use to configure their code. It should probably be in [Engine Playground](#Engine-Playground)

### Engine Playground

This project is a small engine I am working on (forever). I use it to trry new ideas develop my skills and learn new technics. It is also somewhat opiniated on how I think thing should be.
But of course, with lack of time most/all of it is not to the standard that I would like it to be. So maybe a desired opinion.

### Code Exercise

Project where I do coding exercise and katas to learn like Codility and Leetcode.
I might separate this in another repo oneday.

### Code Incubator

Project where I can explore and develop new ideas without the burden of a whole application/engine. The idea is th develop some calss and feature here before using them in the Engine Playground.

### Cpp Exploration && CppStdExploration

Projects that contain code to explore and learn new cpp concepts and constructs in the various cpp standards and code that I found on the internet that I find interresting and is a place where I want to keep them.
I might separate this in another repo oneday.


### Onnx Inference App & OnnxRT

Projects related to ONNX model inference.

## Building

The project uses CMake presets for configuration and building.

**Windows (MSVC):**
```bash
cmake --preset Windows-Msvc-Ninja-Debug-NoTidy
cmake --build --preset Windows-Msvc-Ninja-Debug-NoTidy
```

**Linux (Clang):**
```bash
cmake --preset Linux-Clang-Ninja-Debug-NoTidy
cmake --build --preset Linux-Clang-Ninja-Debug-NoTidy
```

## Development Conventions

*   **Coding Style:** Enforced using `.editorconfig` and `.clang-format`.
*   **Dependencies:** Managed with `vcpkg` as defined in `vcpkg.json`.
*   **Testing:** Unit and performance tests are written using `gtest`, `catch2`, and `benchmark`.
*   **CI/CD:** All code is built and tested on Windows and Linux via GitHub Actions (`.github/workflows/ci.yml`).