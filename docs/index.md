# Computational Basis Transforms (CBT)

## Overview

CBT is a unified framework for understanding and implementing transformations between computational domains. It reveals that many algorithmic breakthroughs are actually discoveries of appropriate domain transformations.

## Quick Start

```cpp
#include <cbt/cbt.hpp>
using namespace cbt;

// Avoid underflow with logarithmic transform
lgd product = lgd(1e-100);
for(int i = 0; i < 1000000; ++i) {
    product = product * lgd(1e-100);  // No underflow!
}

// Bayesian inference without normalization
odds_ratio<double> posterior = prior * likelihood_ratio;

// Exact rational arithmetic
stern_brocot<int> pi_approx(355, 113);
```

## Key Concepts

### What is a CBT?

A Computational Basis Transform (CBT) is a systematic transformation from one computational domain to another that changes the complexity landscape of operations.

### The Fundamental Trade-off

**No Free Lunch Theorem**: Every CBT that makes some operations more efficient must make others less efficient. There is no universal "best" representation.

### CBT Network

The "normal" computational basis is just one node in a network of possible representations. Direct mappings between CBTs can preserve information and avoid overflow.

## Documentation

- [Theory Guide](theory.md) - Mathematical foundations and proofs
- [User Guide](user_guide.md) - How to use CBTs in practice
- [API Reference](api/index.html) - Generated from source
- [Examples](examples.md) - Practical applications
- [Inter-CBT Mappings](mappings.md) - Direct transformations

## Available Transforms

### Core Transforms

| Transform | Gain | Loss | Use Case |
|-----------|------|------|----------|
| Logarithmic | Multiplication → Addition | No direct addition | Products, extreme ranges |
| Odds-Ratio | Bayesian update → Multiplication | Cannot add probabilities | Inference without normalization |
| Stern-Brocot | Exact rationals | Irrationals need ∞ representation | Computer algebra |
| RNS | Parallel arithmetic | Complex comparison | Cryptography |
| Multiscale | Huge dynamic range | Precision at boundaries | Scientific computing |

### Additional Transforms

| Transform | Purpose | Application |
|-----------|---------|-------------|
| Dual | Automatic differentiation | Optimization |
| Interval | Rigorous error bounds | Validated numerics |
| Tropical | Optimization problems | Shortest paths |
| Modular | Cyclic arithmetic | Cryptography |
| Quaternion | 3D rotations | Graphics, robotics |

## Installation

### Header-Only Library

```bash
git clone https://github.com/[project]/cbt
# Include headers directly
```

### CMake Integration

```cmake
find_package(cbt REQUIRED)
target_link_libraries(your_target PRIVATE cbt::cbt)
```

## Examples

### Avoiding Underflow

```cpp
// Problem: Product of many small probabilities
double p = 1.0;
for(int i = 0; i < 100; ++i) {
    p *= 1e-10;  // Underflows to 0 after 30 iterations
}

// Solution: Use logarithmic transform
lgd log_p(1.0);
for(int i = 0; i < 100; ++i) {
    log_p = log_p * lgd(1e-10);  // No underflow!
}
double result = log_p.value();  // Convert back if needed
```

### Bayesian Inference

```cpp
// Traditional approach requires normalization
double posterior = (prior * likelihood) / normalization_constant;

// CBT approach: no normalization needed
odds_ratio<double> posterior = prior_odds * likelihood_ratio;
```

### Inter-CBT Mapping

```cpp
// Direct mapping preserves huge values
lgd huge = lgd::from_log(800);  // e^800
auto ms = mappings::lg_to_multiscale(huge);  // No overflow!
```

## Contributing

We welcome contributions! See [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

## Citation

If you use CBT in research, please cite:

```bibtex
@article{cbt2024,
  title={Computational Basis Transforms: A Unified Framework},
  author={...},
  journal={...},
  year={2024}
}
```

## License

MIT License - see [LICENSE](LICENSE) for details.