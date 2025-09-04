# Computational Basis Transforms (CBT)

A header-only C++17 library implementing Computational Basis Transforms - a unified framework for understanding transformations that trade computational complexity in one domain for simplicity in another.

## Overview

CBT formalizes the fundamental principle that underlies many computational techniques: transforming problems into different representational domains where certain operations become more efficient. This library provides clean, elegant implementations of both classical and novel transforms.

## Core Concept

A Computational Basis Transform is a quadruple (D, D', φ, Ω) where:
- **D, D'** are computational domains
- **φ: D → D'** is the transform
- **Ω** captures the trade-offs

### Key Insight

> There is no universal "best" representation - only fitness for purpose. Every transform involves fundamental trade-offs.

## Transforms Included

### Classical Transforms

#### Logarithmic Transform (`cbt::lg<T>`)
- **Transform**: x → log(x)
- **Gain**: Multiplication becomes addition
- **Loss**: Direct addition not available
- **Use**: Products of many terms, wide dynamic range

```cpp
cbt::lgd a(1000), b(2000);
auto product = a * b;  // Internally: log(1000) + log(2000)
```

### Novel Transforms

#### Odds-Ratio Transform (`cbt::odds_ratio<T>`)
- **Transform**: p → p/(1-p)
- **Gain**: Bayesian updates become multiplication
- **Loss**: Cannot directly add probabilities
- **Use**: Medical diagnosis, spam filtering, ML

```cpp
auto prior = cbt::odds_ratio<double>::from_probability(0.01);
auto posterior = prior * likelihood_ratio;  // Bayesian update!
```

#### Stern-Brocot Transform (`cbt::stern_brocot<T>`)
- **Transform**: ℝ → Position in Stern-Brocot tree
- **Gain**: Exact rational arithmetic
- **Loss**: Irrationals need infinite representation
- **Use**: Computer algebra, music theory

```cpp
cbt::stern_brocot<int> a(1, 3), b(1, 7);
auto sum = a + b;  // Exact: 10/21
```

#### Residue Number System (`cbt::residue_number_system<T, N>`)
- **Transform**: n → (n mod p₁, ..., n mod pₖ)
- **Gain**: Parallel arithmetic, no carries
- **Loss**: Comparison/division complex
- **Use**: Cryptography, DSP, fault tolerance

```cpp
auto rns_a = cbt::rns3<int>::from_integer(12345);
auto rns_b = cbt::rns3<int>::from_integer(67890);
auto sum = rns_a + rns_b;  // Component-wise, fully parallel!
```

#### Multiscale Transform (`cbt::multiscale<T>`)
- **Transform**: value → (mantissa, scale_level)
- **Gain**: Handle 200+ orders of magnitude
- **Loss**: Precision at scale boundaries
- **Use**: Physics simulations, scientific computing

```cpp
cbt::multiscale<double> planck_length(1.616e-35);
cbt::multiscale<double> universe_diameter(8.8e26);
auto ratio = universe_diameter / planck_length;  // No overflow!
```

## Composed Transforms

CBTs can be composed for multiplicative power:

```cpp
// Extreme scale multiplication efficiency
cbt::multiscale<cbt::lg<double>> value;

// Bayesian inference with numerical stability
cbt::log_odds<double> posterior;
```

## Installation

CBT is header-only. Simply include the headers:

```cpp
#include <cbt/cbt.hpp>  // All transforms
```

Or include specific transforms:

```cpp
#include <cbt/logarithmic.hpp>
#include <cbt/odds_ratio.hpp>
```

### CMake Integration

```cmake
find_package(cbt REQUIRED)
target_link_libraries(your_target PRIVATE cbt::cbt)
```

## Building Examples and Tests

```bash
mkdir build && cd build
cmake ..
cmake --build .

# Run demo
./examples/cbt_demo

# Run tests (if enabled)
ctest
```

## Prerequisites

### Required
- **C++17 compatible compiler**
  - GCC 7.0 or later
  - Clang 5.0 or later
  - MSVC 2017 (19.11) or later
  - Apple Clang 10.0 or later
- **CMake 3.14 or higher** (for building examples/tests)

### Optional
- **Doxygen** (for generating API documentation)
- **LaTeX** (pdflatex, bibtex) for building the academic paper
- **gcov/lcov** (for code coverage analysis)

## Theory

The CBT framework provides:

1. **Unifying Principle**: All these transforms share the pattern of trading operations
2. **Design Guidance**: Meta-patterns help discover new transforms
3. **Composition Rules**: Transforms can be systematically combined
4. **Trade-off Analysis**: Makes implicit costs explicit

## Applications

- **Scientific Computing**: Handle extreme scales efficiently
- **Machine Learning**: Numerical stability for probabilistic models
- **Cryptography**: Parallel arithmetic without timing leaks
- **Signal Processing**: Efficient convolution and filtering
- **Computer Graphics**: Level-of-detail and scale management
- **Finance**: High-precision arithmetic for monetary calculations

## Troubleshooting

### Common Issues

**Build Errors with C++17 Features**
- Ensure your compiler supports C++17. Check version with:
  ```bash
  g++ --version  # For GCC
  clang++ --version  # For Clang
  ```
- Update your compiler if needed or use `-std=c++17` flag explicitly

**CMake Cannot Find Package**
- When integrating CBT in your project:
  ```cmake
  # Add CBT directory to module path
  list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/path/to/cbt")
  ```

**Numeric Overflow/Underflow**
- Use `multiscale<T>` transform for extreme values
- Consider `lg<T>` for products of many terms
- Check transform documentation for domain limitations

**Test Coverage Not Working**
- Ensure gcov is installed: `apt install gcov` (Ubuntu) or `brew install gcov` (macOS)
- Build with coverage flags: `cmake .. -DENABLE_COVERAGE=ON`
- Run tests before generating coverage reports

## Contributing

Contributions of new transforms are welcome! Each transform should:
1. Clearly document its trade-offs
2. Provide clean, elegant implementation
3. Include usage examples
4. Follow the CBT pattern
5. Include comprehensive unit tests

### Development Workflow
```bash
# Fork and clone the repository
git clone https://github.com/yourusername/cbt.git
cd cbt

# Create a feature branch
git checkout -b new-transform

# Build and test
mkdir build && cd build
cmake .. -DCBT_BUILD_TESTS=ON -DENABLE_COVERAGE=ON
make -j$(nproc)
./tests/test_cbt_comprehensive

# Check coverage
gcov tests/CMakeFiles/test_cbt_comprehensive.dir/*.gcno
```

## License

MIT License - See LICENSE file for details

## Citation

If you use CBT in your research, please cite:

```bibtex
@article{cbt2024,
  title={Computational Basis Transforms: A Unified Framework},
  author={...},
  year={2024}
}
```

## Further Reading

- See `paper/cbt_theory.tex` for formal treatment
- Examples in `examples/` demonstrate each transform
- Documentation in `docs/` for detailed API reference