# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

CBT (Computational Basis Transforms) is a header-only C++17 library that implements a unified framework for transformations that trade computational complexity in one domain for simplicity in another. The library provides elegant implementations of both classical and novel computational transforms.

**Core Principle**: A CBT is a quadruple (D, D', φ, Ω) where D and D' are computational domains, φ is the transform, and Ω captures the trade-offs. Every transform involves fundamental trade-offs - there is no universal "best" representation, only fitness for purpose.

## Build Commands

```bash
# Configure and build (from project root)
mkdir build && cd build
cmake .. -DCBT_BUILD_TESTS=ON -DCBT_BUILD_EXAMPLES=ON
make -j$(nproc)

# Run all tests
ctest --verbose
# Or run test executable directly:
./tests/test_cbt_comprehensive

# Run examples
./examples/cbt_demo
./examples/mapping_demo

# Enable code coverage
cmake .. -DCBT_BUILD_TESTS=ON -DENABLE_COVERAGE=ON
make -j$(nproc)
./tests/test_cbt_comprehensive
gcov tests/CMakeFiles/test_cbt_comprehensive.dir/*.gcno

# Build academic paper (requires pdflatex and bibtex)
cd paper
make          # Builds cbt.pdf with full bibliography
make clean    # Remove auxiliary files
```

## Architecture

### Core Structure
- **include/cbt/**: Header-only library implementation
  - `cbt.hpp`: Main include file that brings in all transforms
  - Individual transform headers (e.g., `logarithmic.hpp`, `odds_ratio.hpp`)
  - Each transform follows the CBT pattern: (D, D', φ, Ω)
  - All transforms are in the `cbt::` namespace

### Transform Categories

**Core Transforms** (represent the fundamental CBT patterns):
- `logarithmic.hpp` - Multiplication → Addition (e.g., `cbt::lg<double>`, alias `lgd`)
- `odds_ratio.hpp` - Bayesian updates → Multiplication
- `stern_brocot.hpp` - Exact rational arithmetic via tree navigation
- `residue_number_system.hpp` - Parallel arithmetic without carries (e.g., `rns3<int>`)
- `multiscale.hpp` - Handle 200+ orders of magnitude with automatic scale management

**Additional Transforms**:
- `dual.hpp` - Automatic differentiation
- `interval.hpp` - Rigorous error bounds
- `tropical.hpp` - Min-plus algebra for shortest paths
- `modular.hpp` - Cyclic arithmetic for cryptography
- `quaternion.hpp` - 3D rotations without gimbal lock

**Transform Composition**:
- `composed.hpp` - Examples of composed transforms for multiplicative benefits
- `mappings.hpp` - Inter-CBT mappings and custom transform utilities
- Transforms can be nested: `multiscale<lg<double>>` combines extreme scale handling with efficient multiplication

### Key Design Patterns

**Naming Conventions**:
- Type aliases use short names: `lgd` = `lg<double>`, `lgi` = `lg<int>`
- Transform classes use descriptive names: `stern_brocot`, `odds_ratio`
- All types support standard arithmetic operators when semantically appropriate

**Typical Transform Structure**:
1. Private member storing transformed value
2. Constructors: default, explicit from base type, copy/move
3. Static factory methods: `from_*` for non-standard construction
4. Conversion methods: `value()`, `to_*()` for extracting result
5. Operator overloads for the "cheap" operations in transformed domain
6. Trade-off documentation in header comments

### Testing
- Main test file: `tests/test_cbt_comprehensive.cpp`
- Tests all transforms comprehensively with edge cases
- Run with `ctest` or `./tests/test_cbt_comprehensive`
- Coverage can be enabled with `-DENABLE_COVERAGE=ON` CMake option

### Examples
- `examples/cbt_demo.cpp`: Demonstrates all major transforms
- `examples/mapping_demo.cpp`: Shows custom mapping capabilities and inter-transform conversions

## Development Guidelines

### Adding New Transforms
1. Create header in `include/cbt/new_transform.hpp`
2. Follow the CBT pattern with clear trade-off documentation:
   - Document what becomes cheap (the "gain")
   - Document what becomes expensive or impossible (the "loss")
   - Include usage guidance in header comments
3. Add include to `include/cbt/cbt.hpp`
4. Add comprehensive tests to `tests/test_cbt_comprehensive.cpp`:
   - Basic construction and conversion
   - Arithmetic operations
   - Edge cases (overflow, underflow, zero, negative)
   - Composition with other transforms if applicable
5. Add demo usage to `examples/cbt_demo.cpp`
6. Update selection guide in `cbt.hpp` if needed

### Code Style
- Header-only design with template metaprogramming
- C++17 features are used extensively:
  - Structured bindings
  - `if constexpr` for compile-time branching
  - Class template argument deduction (CTAD)
  - `std::optional` and `std::variant` where appropriate
- Use `cbt` namespace for all library code
- Document trade-offs clearly in comments - this is central to the CBT philosophy
- Prefer mathematical elegance over micro-optimization

## Prerequisites

**Required**:
- C++17 compatible compiler (GCC 7.0+, Clang 5.0+, MSVC 2017+, Apple Clang 10.0+)
- CMake 3.14+ (for building tests/examples)

**Optional**:
- gcov/lcov for code coverage analysis
- pdflatex and bibtex for building the academic paper
- Doxygen for API documentation generation

## Important Notes

- Library is header-only - no linking required, just `#include <cbt/cbt.hpp>`
- C++17 is required (not optional)
- Transforms can be composed for multiplicative power: `multiscale<lg<double>>`
- Focus on mathematical elegance and clear trade-off documentation
- Each transform documents its domain limitations (e.g., logarithmic is positive reals only)
- The philosophy is "no universal best representation" - choose transforms based on your computational bottleneck
