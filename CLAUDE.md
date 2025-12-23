# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

CBT (Computational Basis Transforms) is a header-only C++17 library implementing a unified framework for transformations that trade computational complexity in one domain for simplicity in another.

**Core Principle**: A CBT is a quadruple (D, D', φ, Ω) where D and D' are computational domains, φ is the transform, and Ω captures the trade-offs. There is no universal "best" representation - only fitness for purpose.

## Build Commands

```bash
# Full build with tests and examples
cmake -S . -B build -DCBT_BUILD_TESTS=ON -DCBT_BUILD_EXAMPLES=ON
cmake --build build -j$(nproc)

# Run tests
ctest --test-dir build --output-on-failure
# Or directly:
./build/tests/test_cbt_comprehensive

# Run examples
./build/examples/cbt_demo
./build/examples/mapping_demo

# Build with coverage
cmake -S . -B build -DCBT_BUILD_TESTS=ON -DENABLE_COVERAGE=ON
cmake --build build -j$(nproc)
./build/tests/test_cbt_comprehensive
gcov build/tests/CMakeFiles/test_cbt_comprehensive.dir/*.gcno

# Regenerate API docs
./build_docs.sh

# Build academic paper (requires pdflatex/bibtex)
cd paper && make
```

## Architecture

### Directory Structure
- `include/cbt/` - Header-only library; `cbt.hpp` re-exports all transforms
- `tests/test_cbt_comprehensive.cpp` - Assert-driven test harness (extend this, don't create parallel frameworks)
- `examples/` - Canonical usage demonstrations
- `benchmarks/` - Performance trade-off exercises
- `docs/api/html/` - Generated Doxygen (via `build_docs.sh`)
- `paper/` - Academic paper LaTeX source

### Transform Categories

**Core Transforms**:
- `logarithmic.hpp` - Multiplication → Addition (`cbt::lg<T>`, alias `lgd`)
- `odds_ratio.hpp` - Bayesian updates → Multiplication
- `stern_brocot.hpp` - Exact rational arithmetic via tree navigation
- `residue_number_system.hpp` - Parallel arithmetic without carries (`rns3<int>`)
- `multiscale.hpp` - 200+ orders of magnitude with automatic scale management

**Additional Transforms**:
- `dual.hpp` - Automatic differentiation
- `interval.hpp` - Rigorous error bounds
- `tropical.hpp` - Min-plus algebra for shortest paths
- `modular.hpp` - Cyclic arithmetic
- `quaternion.hpp` - 3D rotations without gimbal lock

**Composition**: `composed.hpp` and `mappings.hpp` - Transforms nest for multiplicative benefits: `multiscale<lg<double>>`

### Transform Structure Pattern
1. Private member storing transformed value
2. Constructors: default, explicit from base type, copy/move
3. Static factory methods: `from_*` for non-standard construction
4. Conversion methods: `value()`, `to_*()` for extraction
5. Operator overloads for "cheap" operations in transformed domain
6. Trade-off documentation in header comments

## Code Style

- Modern C++17 with 4-space indentation, `#pragma once`
- Stay inside `cbt` namespace
- Types: PascalCase for aliases (e.g., `SternBrocot`), snake_case for variables/functions
- Templates expose `value_type` plus `from_*` helpers
- Document each transform's (D, D', φ, Ω) story with Doxygen comments
- Headers must be self-contained and constexpr-friendly
- Prefer mathematical elegance over micro-optimization

## Adding New Transforms

1. Create `include/cbt/<name>.hpp`
2. Document trade-offs: what becomes cheap (gain) and expensive (loss)
3. Wire through `include/cbt/cbt.hpp`
4. Add `test_<transform>_comprehensive()` to `tests/test_cbt_comprehensive.cpp`:
   - Forward/inverse transforms
   - Arithmetic shortcuts
   - Edge cases (overflow, degeneracy, numeric stability)
5. Add demo to `examples/cbt_demo.cpp`

## Testing

- Extend `tests/test_cbt_comprehensive.cpp` rather than creating new test files
- Run `ctest --test-dir build --output-on-failure` before pushing
- For coverage: build with `-DENABLE_COVERAGE=ON`, run tests, then `gcov`

## Requirements

- C++17 compiler (GCC 7+, Clang 5+, MSVC 2017+, Apple Clang 10+)
- CMake 3.14+
- Optional: gcov/lcov (coverage), pdflatex/bibtex (paper), Doxygen (API docs)
