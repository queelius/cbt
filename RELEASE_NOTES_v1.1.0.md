# CBT v1.1.0 Release Notes

## Overview

CBT (Computational Basis Transforms) v1.1.0 represents a major milestone in establishing CBT as both a practical C++ library and a serious academic contribution. This release focuses on documentation infrastructure, academic rigor, and professional presentation.

## What's New

### 📚 Academic Paper Transformation

The CBT paper has been significantly enhanced to meet the standards of top-tier academic venues:

- **Expanded from ~20 to ~50 pages** with comprehensive theoretical treatment
- **55+ new citations** from foundational papers (Cooley-Tukey FFT, Napier's logarithms) to recent research
- **Rigorous mathematical proofs** including formal No Free Lunch theorem with information-theoretic arguments
- **Proper author attribution**: Alexander Towell, Southern Illinois University Edwardsville
- **arXiv-ready formatting** with appropriate categories (cs.DS, cs.NA, cs.PL)
- **Concrete performance metrics** documented (8-70× speedups for domain-appropriate operations)
- **Category-theoretic foundations** establishing CBT as a unified framework
- **Submission tracking infrastructure** for multiple academic venues

### 🎨 Professional Documentation System

Complete documentation infrastructure has been added:

- **Doxygen integration** with modern doxygen-awesome theme
- **GitHub Pages configuration** for automatic documentation deployment
- **Comprehensive documentation structure**:
  - API reference with full class documentation
  - Quick start guides for immediate productivity
  - Mathematical foundations explained
  - Transform-specific guides
  - Extensive tutorials and examples
  - Troubleshooting section
- **Custom branding** with logo and consistent styling
- **Automated build system** via `build_docs.sh`

### 🐛 Bug Fixes

- **Fixed GitHub Actions workflow** - Updated deprecated action versions for documentation generation
- **Enhanced .gitignore** - Properly excludes generated documentation files

### 🏗️ Project Organization

- **Paper versioning**: Created `paper/master/` for main paper version control
- **Submission tracking**: Added `paper/submissions/` with venue-specific versions
- **Documentation hierarchy**: Organized `docs/` with clear structure
- **Build automation**: Added scripts for documentation and paper building

## Installation

The library remains header-only and easy to integrate:

```cpp
#include <cbt/cbt.hpp>
```

## Documentation

- **API Documentation**: Available via GitHub Pages after merge
- **Academic Paper**: [paper/cbt.pdf](/home/spinoza/github/released/cbt/paper/cbt.pdf)
- **Quick Start**: [docs/quick_start.md](/home/spinoza/github/released/cbt/docs/quick_start.md)
- **Tutorials**: [docs/tutorials.md](/home/spinoza/github/released/cbt/docs/tutorials.md)

## Example Usage

```cpp
// Stable probability computations
cbt::lg<double> product = cbt::lg<double>::from_value(1e-100);
for(int i = 0; i < 1000000; ++i) {
    product = product * cbt::lg<double>::from_value(1e-100);
}
// No underflow even with million small probabilities!

// Exact rational arithmetic
cbt::stern_brocot pi_approx = cbt::stern_brocot::approximate_real(M_PI, 1e-6);
auto squared = pi_approx * pi_approx;  // Still exact!
```

## Performance

The enhanced paper documents substantial performance improvements:
- **8-15×** speedup for probability products in log domain
- **30-70×** speedup for Bayesian updates with odds ratios
- **Near-linear scaling** for parallel arithmetic with RNS
- **Numerical stability** for products of millions of small values

## Academic Impact

This release positions CBT for academic publication with:
- Submission preparation for arXiv (cs.DS, cs.NA categories)
- Targeting top venues like ICFP, POPL, and JFP
- Rigorous proofs establishing fundamental computational trade-offs
- Novel insights connecting disparate algorithmic techniques

## Contributors

- **Alexander Towell** (Southern Illinois University Edwardsville) - Primary author
- Additional contributions via GitHub community

## Future Work

- Automatic CBT selection based on workload characteristics
- GPU implementations for massive parallelism
- Integration with numerical computing libraries
- Expanded transform library

## Compatibility

- **C++ Standard**: C++17 or later required
- **Compilers**: GCC 7+, Clang 5+, MSVC 2017+
- **Platforms**: Linux, macOS, Windows
- **Dependencies**: None (header-only)

## Links

- **Repository**: https://github.com/queelius/cbt
- **Documentation**: (Available after GitHub Pages deployment)
- **Paper**: Available in paper/cbt.pdf
- **Issues**: https://github.com/queelius/cbt/issues

## Acknowledgments

Thanks to the academic reviewers and GitHub community for feedback that shaped this release. Special acknowledgment to the foundational work in computational algebra, numerical analysis, and category theory that CBT builds upon.

---

*CBT: Where computational complexity is not inherent to problems, but depends on the choice of representation.*