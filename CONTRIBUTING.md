# Contributing to CBT

Thank you for your interest in contributing to the Computational Basis Transforms (CBT) library! This document provides guidelines and instructions for contributing.

## Code of Conduct

Please note that this project is released with a [Contributor Code of Conduct](CODE_OF_CONDUCT.md). By participating in this project you agree to abide by its terms.

## How Can I Contribute?

### Reporting Bugs

Before creating bug reports, please check existing issues to avoid duplicates. When you create a bug report, include as many details as possible:

- **Use a clear and descriptive title**
- **Describe the exact steps to reproduce the problem**
- **Provide specific examples** demonstrating the issue
- **Include your environment details** (OS, compiler, C++ version)
- **Include relevant code snippets** that demonstrate the problem

### Suggesting Enhancements

Enhancement suggestions are tracked as GitHub issues. When creating an enhancement suggestion:

- **Use a clear and descriptive title**
- **Provide a detailed description** of the suggested enhancement
- **Include examples** of how the feature would be used
- **Explain why** this enhancement would be useful

### Pull Requests

1. Fork the repository and create your branch from `master`
2. Follow the existing code style and conventions
3. Add tests for any new functionality
4. Ensure all tests pass
5. Update documentation as needed
6. Submit your pull request

## Development Setup

### Prerequisites

- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- CMake 3.14 or higher
- Git

### Building and Testing

```bash
# Clone your fork
git clone https://github.com/your-username/cbt.git
cd cbt

# Create build directory
mkdir build && cd build

# Configure with tests enabled
cmake .. -DCBT_BUILD_TESTS=ON -DCBT_BUILD_EXAMPLES=ON -DENABLE_COVERAGE=ON

# Build
make -j$(nproc)

# Run tests
ctest --verbose

# Run specific test
./tests/test_cbt_comprehensive

# Generate coverage report (optional)
gcov tests/CMakeFiles/test_cbt_comprehensive.dir/*.gcno
```

## Adding New Transforms

When adding a new transform to CBT:

1. **Create the header file** in `include/cbt/your_transform.hpp`
2. **Follow the CBT pattern**: Implement the four components (D, D', φ, Ω)
3. **Document the trade-offs** clearly in comments
4. **Include in main header**: Add to `include/cbt/cbt.hpp`
5. **Add comprehensive tests** to `tests/test_cbt_comprehensive.cpp`
6. **Add demo usage** to `examples/cbt_demo.cpp`
7. **Update documentation** if adding new concepts

### Transform Template

```cpp
// include/cbt/your_transform.hpp
#pragma once
#include <cbt/core.hpp>

namespace cbt {

template<typename T>
class your_transform {
public:
    using value_type = T;
    
    // Constructor from original domain
    explicit your_transform(T value) : transformed_(transform(value)) {}
    
    // Transform to new basis (φ)
    static T transform(T value) { 
        // Implementation
    }
    
    // Transform back (φ⁻¹)
    T inverse() const { 
        // Implementation
    }
    
    // Operations in transformed domain (Ω')
    your_transform operator*(const your_transform& other) const {
        // Implementation of efficient operation
    }
    
private:
    T transformed_;
};

} // namespace cbt
```

## Code Style Guidelines

### General C++ Style

- Use descriptive names for variables and functions
- Follow the existing naming conventions (snake_case for functions/variables, PascalCase for types)
- Keep functions focused and concise
- Document complex algorithms and non-obvious code
- Use `constexpr` and `noexcept` where appropriate

### CBT-Specific Guidelines

- All transforms must be in the `cbt` namespace
- Follow the (D, D', φ, Ω) pattern consistently
- Document computational complexity trade-offs
- Provide both `transform` and `inverse` methods where feasible
- Include type aliases for common instantiations (e.g., `using lgd = lg<double>`)

### Documentation

- Use Doxygen-style comments for public APIs
- Include usage examples in documentation
- Document the mathematical basis of transforms
- Explain when and why to use each transform

## Testing Guidelines

All new code should include tests:

- Unit tests for basic functionality
- Edge case testing (zero, negative, overflow conditions)
- Performance benchmarks for new transforms
- Integration tests showing transform composition

Example test structure:

```cpp
TEST_CASE("YourTransform operations", "[your_transform]") {
    SECTION("Basic transformation") {
        // Test forward and inverse transforms
    }
    
    SECTION("Arithmetic operations") {
        // Test transformed operations
    }
    
    SECTION("Edge cases") {
        // Test boundary conditions
    }
    
    SECTION("Composition with other transforms") {
        // Test transform composition if applicable
    }
}
```

## Documentation

- Update README.md if adding major features
- Add inline documentation for all public methods
- Include mathematical formulations where appropriate
- Update the API documentation by running Doxygen

## Commit Messages

- Use present tense ("Add feature" not "Added feature")
- Use imperative mood ("Move cursor to..." not "Moves cursor to...")
- Limit first line to 72 characters
- Reference issues and pull requests when relevant

Example:
```
Add tropical algebra transform implementation

- Implements min-plus algebra operations
- Adds comprehensive test coverage
- Updates documentation with use cases

Fixes #42
```

## Questions?

Feel free to open an issue for any questions about contributing. We appreciate your interest in making CBT better!