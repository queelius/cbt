# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

CBT (Computational Basis Transforms) is a header-only C++17 library that implements a unified framework for transformations that trade computational complexity in one domain for simplicity in another. The library provides elegant implementations of both classical and novel computational transforms.

## Build Commands

```bash
# Configure and build
mkdir build && cd build
cmake .. -DCBT_BUILD_TESTS=ON -DCBT_BUILD_EXAMPLES=ON
make -j$(nproc)

# Run tests
ctest --verbose
# Or directly: ./tests/test_cbt

# Run examples
./examples/cbt_demo
./examples/mapping_demo

# Build documentation
./build_docs.sh  # Generates Doxygen API docs and compiles LaTeX paper
```

## Architecture

### Core Structure
- **include/cbt/**: Header-only library implementation
  - `cbt.hpp`: Main include file that brings in all transforms
  - Individual transform headers (e.g., `logarithmic.hpp`, `odds_ratio.hpp`)
  - Each transform follows the CBT pattern: (D, D', φ, Ω)

### Key Transforms
- **Classical**: `logarithmic.hpp` - multiplication becomes addition
- **Probabilistic**: `odds_ratio.hpp` - Bayesian updates become multiplication  
- **Exact Rational**: `stern_brocot.hpp` - exact rational arithmetic via tree navigation
- **Parallel**: `residue_number_system.hpp` - parallel arithmetic without carries
- **Scale**: `multiscale.hpp` - handle 200+ orders of magnitude
- **Advanced**: `dual.hpp`, `interval.hpp`, `tropical.hpp`, `quaternion.hpp`, `modular.hpp`
- **Utilities**: `composed.hpp` for transform composition, `mappings.hpp` for custom mappings

### Testing
- Tests are in `tests/test_cbt.cpp`
- Currently tests basic functionality of core transforms
- No coverage tooling configured yet

### Examples
- `examples/cbt_demo.cpp`: Demonstrates all major transforms
- `examples/mapping_demo.cpp`: Shows custom mapping capabilities

## Development Guidelines

### Adding New Transforms
1. Create header in `include/cbt/new_transform.hpp`
2. Follow the CBT pattern with clear trade-off documentation
3. Include in `include/cbt/cbt.hpp`
4. Add tests to `tests/test_cbt.cpp`
5. Add demo usage to `examples/cbt_demo.cpp`

### Code Style
- Header-only design with template metaprogramming
- Use `cbt` namespace
- Follow existing naming conventions (e.g., `lgd` for log-domain double)
- Document trade-offs clearly in comments

### Testing Individual Components
```bash
# Build and run specific test
cd build
make test_cbt
./tests/test_cbt

# Debug with gdb
gdb ./tests/test_cbt
```

## Important Notes
- C++17 required (uses features like structured bindings, if constexpr)
- All transforms are in `cbt::` namespace
- Transforms can be composed for multiplicative power (e.g., `cbt::multiscale<cbt::lg<double>>`)
- Focus on mathematical elegance and clear trade-off documentation