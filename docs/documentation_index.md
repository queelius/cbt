# CBT Documentation Index

Welcome to the Computational Basis Transforms (CBT) documentation. This guide will help you navigate the available resources.

## Documentation Structure

### Getting Started
- [README](../README.md) - Project overview and installation
- [Quick Start Guide](quick_start.md) - Get up and running in 5 minutes
- [Examples](../examples/) - Working code demonstrations

### Core Documentation
- [API Reference](api_reference.md) - Complete API documentation for all transforms
- [Transform Usage Guides](transform_guides.md) - In-depth guide for each transform
- [Mathematical Foundations](mathematical_foundations.md) - Theoretical background and proofs
- [Troubleshooting & FAQ](troubleshooting.md) - Common issues and solutions

### Advanced Topics
- [Theory Document](theory.md) - Extended theoretical treatment
- [Performance Guide](transform_guides.md#performance-optimization-guide) - Optimization techniques
- [Composition Patterns](transform_guides.md#advanced-composition-patterns) - Combining transforms

### Generated Documentation
- [Doxygen API Docs](api/html/index.html) - Auto-generated from source code
- [Academic Paper](../paper/cbt.pdf) - Formal academic treatment

## Quick Navigation

### By Use Case

#### Machine Learning / Data Science
- [Logarithmic Transform](transform_guides.md#logarithmic-transform-cbtlgt) - Stable probability computations
- [Odds-Ratio Transform](transform_guides.md#odds-ratio-transform-cbtodds_ratiot) - Bayesian inference
- [Quick Start Examples](quick_start.md#case-1-machine-learning---stable-probability-computations)

#### Scientific Computing
- [Multiscale Transform](transform_guides.md#multiscale-transform-cbtmultiscalet) - Extreme dynamic ranges
- [Interval Arithmetic](api_reference.md#interval-arithmetic-cbtintervalt) - Rigorous error bounds
- [Physics Examples](quick_start.md#case-3-scientific-computing---multi-scale-physics)

#### Cryptography / Security
- [Residue Number System](transform_guides.md#residue-number-system-cbtresidue_number_systemtn) - Parallel modular arithmetic
- [Modular Transform](api_reference.md#modular-arithmetic-cbtmodulartm) - Cyclic arithmetic
- [Cryptographic Examples](transform_guides.md#example-2-cryptographic-operations)

#### Exact Computation
- [Stern-Brocot Transform](transform_guides.md#stern-brocot-transform-cbtstern_brocott) - Exact rational arithmetic
- [Financial Examples](quick_start.md#case-4-financial-computing---exact-decimal-arithmetic)
- [Music Theory Applications](transform_guides.md#example-1-musical-interval-calculator)

### By Transform Type

#### Core Transforms
1. **[Logarithmic](api_reference.md#logarithmic-transform)** - `cbt::lg<T>`
   - Multiplication → Addition
   - [Usage Guide](transform_guides.md#logarithmic-transform-cbtlgt)
   - [Examples](quick_start.md#your-first-cbt-program)

2. **[Odds-Ratio](api_reference.md#odds-ratio-transform)** - `cbt::odds_ratio<T>`
   - Bayesian updates → Multiplication
   - [Usage Guide](transform_guides.md#odds-ratio-transform-cbtodds_ratiot)
   - [Medical Diagnosis Example](transform_guides.md#example-1-medical-diagnostic-system)

3. **[Stern-Brocot](api_reference.md#stern-brocot-transform)** - `cbt::stern_brocot<T>`
   - Exact rational arithmetic
   - [Usage Guide](transform_guides.md#stern-brocot-transform-cbtstern_brocott)
   - [Continued Fractions Example](transform_guides.md#example-2-continued-fraction-approximation)

4. **[Residue Number System](api_reference.md#residue-number-system)** - `cbt::residue_number_system<T,N>`
   - Parallel arithmetic
   - [Usage Guide](transform_guides.md#residue-number-system-cbtresidue_number_systemtn)
   - [DSP Filter Example](transform_guides.md#example-1-parallel-dsp-filter)

5. **[Multiscale](api_reference.md#multiscale-transform)** - `cbt::multiscale<T>`
   - Extreme dynamic ranges
   - [Usage Guide](transform_guides.md#multiscale-transform-cbtmultiscalet)
   - [Physics Engine Example](transform_guides.md#example-1-multi-scale-physics-engine)

#### Additional Transforms
- [Dual Numbers](api_reference.md#dual-numbers-cbtdualt) - Automatic differentiation
- [Interval Arithmetic](api_reference.md#interval-arithmetic-cbtintervalt) - Validated numerics
- [Tropical Algebra](api_reference.md#tropical-algebra-cbttropicalt) - Optimization problems
- [Modular Arithmetic](api_reference.md#modular-arithmetic-cbtmodulartm) - Cyclic operations
- [Quaternions](api_reference.md#quaternions-cbtquaterniont) - 3D rotations

### By Topic

#### Installation & Setup
- [Prerequisites](../README.md#prerequisites)
- [Installation Guide](../README.md#installation)
- [CMake Integration](quick_start.md#installation-in-30-seconds)
- [Building Examples](../README.md#building-examples-and-tests)

#### Theory & Mathematics
- [Core Concepts](mathematical_foundations.md#1-formal-definition)
- [Fundamental Theorems](mathematical_foundations.md#2-fundamental-theorems)
- [Category Theory View](mathematical_foundations.md#8-category-theory-perspective)
- [Information Theory](mathematical_foundations.md#9-information-theoretic-analysis)

#### Performance & Optimization
- [Performance Characteristics](api_reference.md#performance-characteristics)
- [Memory Optimization](transform_guides.md#memory-layout-considerations)
- [SIMD Optimization](transform_guides.md#simd-optimization)
- [Compile-Time Optimization](transform_guides.md#compile-time-optimization)

#### Troubleshooting
- [Build Issues](troubleshooting.md#build-and-compilation-issues)
- [Runtime Issues](troubleshooting.md#runtime-issues)
- [Performance Issues](troubleshooting.md#performance-issues)
- [Numerical Stability](troubleshooting.md#numerical-stability-issues)
- [FAQ](troubleshooting.md#frequently-asked-questions)

## Learning Path

### Beginner Path
1. Read the [README](../README.md) for overview
2. Follow the [Quick Start Guide](quick_start.md)
3. Run the [examples](../examples/cbt_demo.cpp)
4. Read [Core Concepts](mathematical_foundations.md#1-formal-definition)

### Intermediate Path
1. Study [Transform Usage Guides](transform_guides.md) for your use case
2. Understand [composition patterns](transform_guides.md#advanced-composition-patterns)
3. Review the [API Reference](api_reference.md)
4. Implement a small project using CBT

### Advanced Path
1. Read [Mathematical Foundations](mathematical_foundations.md) in detail
2. Study the [source code](../include/cbt/) directly
3. Explore [performance optimization](transform_guides.md#performance-optimization-guide)
4. Create custom transforms following the CBT pattern

## Contributing

Interested in contributing to CBT? See:
- [Contributing Guidelines](../README.md#contributing)
- [Development Workflow](../README.md#contributing)
- [Code Style Guide](../CLAUDE.md#code-style)

## Support

Need help?
- Check the [FAQ](troubleshooting.md#frequently-asked-questions)
- Review [Troubleshooting Guide](troubleshooting.md)
- Search [GitHub Issues](https://github.com/[username]/cbt/issues)
- Read the [academic paper](../paper/cbt.pdf)

## Version History

- **v1.0.0** - Initial release with core transforms
- Future versions will add:
  - GPU acceleration support
  - Additional transforms
  - Python bindings
  - More examples

## License

CBT is released under the MIT License. See [LICENSE](../LICENSE) for details.

---

*Last updated: 2024*

*For the most up-to-date documentation, visit the [GitHub repository](https://github.com/[username]/cbt)*