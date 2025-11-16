# Repository Guidelines

## Project Structure & Module Organization
- `include/cbt/` houses the entire header-only API; `cbt/cbt.hpp` re-exports core, additional, composed, and mapping transforms.
- `tests/test_cbt_comprehensive.cpp` is the assert-driven harness; extend it rather than creating parallel frameworks.
- `examples/` shows canonical usage, `benchmarks/` exercises performance trade-offs, and `docs/api/html/index.html` contains generated Doxygen (regenerated via `build_docs.sh`).
- Keep build artifacts inside `build/` and reference reusable CMake helpers in `cmake/`; avoid editing generated files under `doxygen-output/`.

## Build, Test, and Development Commands
- `cmake -S . -B build -DCBT_BUILD_TESTS=ON -DCBT_BUILD_EXAMPLES=ON -DENABLE_COVERAGE=ON` configures with tests, examples, and coverage instrumentation.
- `cmake --build build -j$(nproc)` compiles the comprehensive suite and optional extras.
- `ctest --test-dir build --output-on-failure` (or `./build/tests/test_cbt_comprehensive`) runs the exhaustive transform checks.
- `./build_docs.sh` regenerates API docs; `cmake --build build --target benchmarks` runs perf probes when needed.

## Coding Style & Naming Conventions
- Modern C++17 with 4-space indentation, `#pragma once`, and zero exceptions unless justified; stay inside the `cbt` namespace.
- Types favor PascalCase (e.g., `SternBrocot` aliases), variables/functions prefer snake_case, and templates should expose `value_type` plus `from_*` helpers mirroring existing headers.
- Document each transform’s (D, D', φ, Ω) story using concise Doxygen comments and mathematical context; keep headers self-contained and constexpr-friendly.
- New transforms live in `include/cbt/<name>.hpp` and must be wired through `include/cbt/cbt.hpp`.

## Testing Guidelines
- Extend `tests/test_cbt_comprehensive.cpp` with `test_<transform>_comprehensive()` blocks that validate forward/inverse transforms, arithmetic shortcuts, and edge cases (overflow, degeneracy, numeric stability).
- Run the suite via `ctest` before pushing; add focused benchmarks if changes impact performance-critical paths.
- For coverage requests, build with `-DENABLE_COVERAGE=ON` and run `gcov tests/CMakeFiles/test_cbt_comprehensive.dir/*.gcno`, attaching summaries in the PR.

## Commit & Pull Request Guidelines
- Follow the existing imperative, descriptive style (`Fix GitHub Actions workflow - update deprecated action versions`); keep the subject ≤72 chars, present tense, and expand context in the body bullets.
- Reference related issues (`Fixes #42`) and mention doc/test impacts explicitly.
- PR descriptions should link specs/issues, list test commands executed, and include screenshots or doc previews when altering docs (`docs/api` or `paper/`).
- Ensure every PR adds tests for new behaviors, updates relevant docs/README sections, and calls out new configuration flags or toolchain requirements.
