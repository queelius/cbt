# Computational Basis Transforms (CBT)

[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B17)
[![Header-Only](https://img.shields.io/badge/header--only-yes-brightgreen.svg)](include/cbt)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![Documentation](https://img.shields.io/badge/docs-doxygen-orange.svg)](docs/api/html/index.html)

A header-only C++17 library implementing Computational Basis Transforms - a unified framework for understanding transformations that trade computational complexity in one domain for simplicity in another.

## 🎯 Overview

CBT formalizes the fundamental principle that underlies many computational techniques: transforming problems into different representational domains where certain operations become more efficient. This library provides clean, elegant implementations of both classical and novel transforms.

### Why CBT?

- **Unified Framework**: Understand FFT, logarithmic arithmetic, and Bayesian inference as instances of the same pattern
- **Performance**: Transform expensive operations into cheap ones (e.g., O(n²) multiplication → O(n) addition)
- **Numerical Stability**: Avoid overflow/underflow by choosing appropriate computational domains
- **Composability**: Combine transforms for multiplicative benefits
- **Header-Only**: Zero configuration, just include and use

## 📚 Table of Contents

- [Quick Start](#-quick-start)
- [Core Concepts](#-core-concepts)
- [Available Transforms](#-available-transforms)
- [Installation](#-installation)
- [Usage Examples](#-usage-examples)
- [API Documentation](#-api-documentation)
- [Performance](#-performance)
- [Contributing](#-contributing)
- [Citation](#-citation)

## 🚀 Quick Start

```cpp
#include <cbt/cbt.hpp>

int main() {
    using namespace cbt;
    
    // Avoid underflow in products of small probabilities
    lgd product(1e-100);
    for(int i = 0; i < 1000000; ++i) {
        product = product * lgd(1e-100);  // No underflow!
    }
    
    // Bayesian inference without normalization
    auto prior = odds_ratio<double>::from_probability(0.01);
    auto posterior = prior * likelihood_ratio;  // Simple multiplication!
    
    // Exact rational arithmetic
    stern_brocot<int> pi_approx(355, 113);  // 355/113 ≈ π
    
    return 0;
}
```

## 📐 Core Concepts

A Computational Basis Transform is a quadruple **(D, D', φ, Ω)** where:
- **D, D'** are computational domains
- **φ: D → D'** is the transform
- **Ω** captures the trade-offs

### Key Insight

> There is no universal "best" representation - only fitness for purpose. Every transform involves fundamental trade-offs.

### The CBT Pattern

Every transform follows a consistent pattern:
1. **Transform** to a new domain where target operations are efficient
2. **Compute** using the simplified operations
3. **Transform back** when needed (or stay in transformed domain)

## 🔧 Available Transforms

### Core Transforms

| Transform | Class | Trade-off | Best For |
|-----------|-------|-----------|----------|
| **Logarithmic** | `lg<T>` | Multiplication → Addition | Products, extreme ranges |
| **Odds-Ratio** | `odds_ratio<T>` | Bayesian update → Multiplication | Inference, ML |
| **Stern-Brocot** | `stern_brocot<T>` | Exact rationals vs. finite repr. | Computer algebra |
| **Residue Number** | `residue_number_system<T,N>` | Parallel ops vs. comparison | Cryptography, DSP |
| **Multiscale** | `multiscale<T>` | Huge range vs. precision | Scientific computing |

#### 📊 Logarithmic Transform (`cbt::lg<T>`)

```cpp
cbt::lgd a(1000), b(2000);
auto product = a * b;  // Internally: log(1000) + log(2000)
auto huge = lgd::from_log(800);  // Represents e^800 without overflow
```

- **Transform**: x → log(x)
- **Gain**: O(n²) multiplication → O(n) addition, extended range
- **Loss**: No direct addition, domain limited to positive reals
- **Use Cases**: Products of many terms, wide dynamic ranges, ML optimization

#### 🎲 Odds-Ratio Transform (`cbt::odds_ratio<T>`)

```cpp
auto prior = cbt::odds_ratio<double>::from_probability(0.01);
auto posterior = prior * likelihood_ratio;  // Bayesian update via multiplication!
double prob = posterior.to_probability();  // Back to probability
```

- **Transform**: p → p/(1-p)
- **Gain**: Bayesian updates without normalization, numerical stability
- **Loss**: Cannot directly add probabilities
- **Use Cases**: Medical diagnosis, spam filtering, sequential inference

#### 🎵 Stern-Brocot Transform (`cbt::stern_brocot<T>`)

```cpp
cbt::stern_brocot<int> a(1, 3), b(1, 7);
auto sum = a + b;  // Exact: 10/21, no rounding errors!
auto pi = stern_brocot<int>::approximate(3.14159, 1000);  // Best rational approx
```

- **Transform**: ℝ → Position in Stern-Brocot tree
- **Gain**: Exact rational arithmetic, optimal approximations
- **Loss**: Irrationals need infinite representation
- **Use Cases**: Computer algebra, music theory, continued fractions

#### ⚡ Residue Number System (`cbt::residue_number_system<T, N>`)

```cpp
auto rns_a = cbt::rns3<int>::from_integer(12345);
auto rns_b = cbt::rns3<int>::from_integer(67890);
auto sum = rns_a + rns_b;  // Component-wise, fully parallel!
auto product = rns_a * rns_b;  // No carry propagation!
```

- **Transform**: n → (n mod p₁, ..., n mod pₖ)
- **Gain**: Fully parallel arithmetic, constant-time operations
- **Loss**: Complex comparison and division
- **Use Cases**: Cryptography, DSP, fault-tolerant computing

#### 🔬 Multiscale Transform (`cbt::multiscale<T>`)

```cpp
cbt::multiscale<double> planck_length(1.616e-35);
cbt::multiscale<double> universe_diameter(8.8e26);
auto ratio = universe_diameter / planck_length;  // Handles 61 orders of magnitude!
```

- **Transform**: value → (mantissa, scale_level)
- **Gain**: Handle 200+ orders of magnitude, automatic scale management
- **Loss**: Precision at scale boundaries
- **Use Cases**: Astrophysics, quantum mechanics, multi-scale simulations

### Additional Transforms

| Transform | Class | Purpose | Application |
|-----------|-------|---------|-------------|
| **Dual** | `dual<T>` | Automatic differentiation | Optimization, sensitivity |
| **Interval** | `interval<T>` | Rigorous error bounds | Validated numerics |
| **Tropical** | `tropical<T>` | Min-plus algebra | Shortest paths, scheduling |
| **Modular** | `modular<T,M>` | Cyclic arithmetic | Cryptography, hashing |
| **Quaternion** | `quaternion<T>` | 3D rotations | Graphics, robotics |

## 🔗 Transform Composition

CBTs can be composed for multiplicative benefits:

```cpp
// Extreme scale multiplication efficiency
using extreme_mult = cbt::multiscale<cbt::lg<double>>;
extreme_mult value;  // Handles both extreme scales AND efficient multiplication

// Stable Bayesian inference  
using log_odds = cbt::lg<cbt::odds_ratio<double>>;
log_odds posterior;  // Numerical stability for sequential updates

// Exact rational intervals
using rational_interval = cbt::interval<cbt::stern_brocot<int>>;
rational_interval precise_bound;  // Rigorous bounds with exact arithmetic
```

## 💻 Installation

### Header-Only Library

CBT is header-only - no compilation required!

```bash
# Clone the repository
git clone https://github.com/[username]/cbt.git

# Include in your project
#include <cbt/cbt.hpp>  // All transforms
# Or specific transforms:
#include <cbt/logarithmic.hpp>
#include <cbt/odds_ratio.hpp>
```

### CMake Integration

```cmake
# Option 1: Add as subdirectory
add_subdirectory(path/to/cbt)
target_link_libraries(your_target PRIVATE cbt::cbt)

# Option 2: Use FetchContent
include(FetchContent)
FetchContent_Declare(
    cbt
    GIT_REPOSITORY https://github.com/[username]/cbt.git
    GIT_TAG main
)
FetchContent_MakeAvailable(cbt)
target_link_libraries(your_target PRIVATE cbt::cbt)
```

### Package Managers

```bash
# vcpkg (coming soon)
vcpkg install cbt

# Conan (coming soon)  
conan install cbt/1.0.0@
```

## 🔨 Building Examples and Tests

```bash
# Quick build
mkdir build && cd build
cmake .. -DCBT_BUILD_TESTS=ON -DCBT_BUILD_EXAMPLES=ON
make -j$(nproc)

# Run comprehensive demo
./examples/cbt_demo

# Run all tests
ctest --verbose

# With code coverage
cmake .. -DENABLE_COVERAGE=ON
make -j$(nproc)
./tests/test_cbt_comprehensive
gcov tests/CMakeFiles/test_cbt_comprehensive.dir/*.gcno

# Build documentation
doxygen Doxyfile
# Open docs/api/html/index.html in browser

# Build academic paper (requires LaTeX)
cd paper && make
```

## 📋 Prerequisites

### Required
- **C++17 compatible compiler**
  - GCC 7.0+ / Clang 5.0+ / MSVC 2017+ / Apple Clang 10.0+
  - Full C++17 support including:
    - Structured bindings
    - `if constexpr`
    - Class template argument deduction
    - `std::optional` and `std::variant`
- **CMake 3.14+** (for building examples/tests)

### Optional
- **Doxygen 1.8+** - API documentation generation
- **LaTeX** (pdflatex, bibtex) - Academic paper compilation
- **gcov/lcov** - Code coverage analysis
- **Google Test** - Unit testing framework (auto-downloaded)
- **graphviz** - Documentation diagrams

## 💡 Theory & Design

The CBT framework provides:

1. **Unifying Principle**: All these transforms share the pattern of trading operations
2. **Design Guidance**: Meta-patterns help discover new transforms
3. **Composition Rules**: Transforms can be systematically combined
4. **Trade-off Analysis**: Makes implicit costs explicit

## 🎯 Applications

### Scientific Computing
- **Astrophysics**: Handle scales from Planck length to universe diameter
- **Quantum Chemistry**: Stable computation of extremely small probabilities
- **Climate Modeling**: Multi-scale atmospheric simulations

### Machine Learning
- **Deep Learning**: Stable gradient computation in very deep networks
- **Probabilistic Models**: Avoid underflow in product of many probabilities
- **Bayesian Networks**: Efficient belief propagation

### Engineering
- **Signal Processing**: FFT-like transforms for custom domains
- **Control Systems**: Numerical stability in feedback loops
- **Robotics**: Gimbal-lock-free rotations with quaternions

### Other Domains
- **Cryptography**: Constant-time modular arithmetic
- **Finance**: Exact decimal arithmetic for monetary values
- **Music**: Rational frequency ratios for pure intervals
- **Computer Graphics**: Level-of-detail with multiscale transforms

## 📖 Usage Examples

### Example 1: Avoiding Numerical Underflow

```cpp
#include <cbt/logarithmic.hpp>

// Problem: Computing product of many small probabilities
double compute_likelihood_naive(const std::vector<double>& probs) {
    double result = 1.0;
    for(double p : probs) {
        result *= p;  // Underflows to 0 for many small values!
    }
    return result;
}

// Solution: Use logarithmic transform
double compute_likelihood_stable(const std::vector<double>& probs) {
    cbt::lgd result(1.0);
    for(double p : probs) {
        result = result * cbt::lgd(p);  // No underflow!
    }
    return result.value();  // Convert back only at the end
}
```

### Example 2: Bayesian Diagnosis System

```cpp
#include <cbt/odds_ratio.hpp>

class MedicalDiagnosis {
    cbt::odds_ratio<double> disease_odds;
    
public:
    MedicalDiagnosis(double prevalence) 
        : disease_odds(cbt::odds_ratio<double>::from_probability(prevalence)) {}
    
    void update_with_test(double sensitivity, double specificity) {
        double lr = sensitivity / (1.0 - specificity);
        disease_odds = disease_odds * cbt::odds_ratio<double>(lr);
    }
    
    double get_probability() const {
        return disease_odds.to_probability();
    }
};

// Usage
MedicalDiagnosis covid_test(0.01);  // 1% prevalence
covid_test.update_with_test(0.98, 0.95);  // First test
covid_test.update_with_test(0.98, 0.95);  // Second test
std::cout << "Probability: " << covid_test.get_probability() << std::endl;
```

### Example 3: Multi-Scale Physics Simulation

```cpp
#include <cbt/multiscale.hpp>
#include <cbt/composed.hpp>

using namespace cbt;

// Combine multiscale with logarithmic for extreme computations
using ExtremeMult = multiscale<lg<double>>;

void quantum_gravity_calculation() {
    ExtremeMult planck_scale(1.616e-35);   // Planck length
    ExtremeMult cosmic_scale(8.8e26);      // Observable universe
    
    // Compute ratios without overflow
    auto scale_ratio = cosmic_scale / planck_scale;
    
    // Perform multiplications efficiently at any scale
    ExtremeMult result = planck_scale;
    for(int i = 0; i < 100; ++i) {
        result = result * cosmic_scale;  // No overflow, efficient multiplication
    }
}
```

## 🛠 Troubleshooting

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