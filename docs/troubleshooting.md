# Troubleshooting & FAQ

## Common Issues and Solutions

### Build and Compilation Issues

#### Issue: "no type named 'optional' in namespace 'std'"

**Problem**: Your compiler doesn't support C++17 features.

**Solution**:
```bash
# Check compiler version
g++ --version  # Should be 7.0+
clang++ --version  # Should be 5.0+

# Explicitly enable C++17
g++ -std=c++17 your_file.cpp

# For CMake projects
cmake .. -DCMAKE_CXX_STANDARD=17
```

#### Issue: "fatal error: cbt/cbt.hpp: No such file or directory"

**Problem**: Include path not set correctly.

**Solution**:
```bash
# Manual compilation
g++ -I/path/to/cbt/include your_file.cpp

# CMake
target_include_directories(your_target PRIVATE ${CBT_INCLUDE_DIR})

# Or use FetchContent
include(FetchContent)
FetchContent_Declare(cbt
    GIT_REPOSITORY https://github.com/[user]/cbt.git
)
FetchContent_MakeAvailable(cbt)
```

#### Issue: Linking errors with examples

**Problem**: Examples need to be explicitly enabled.

**Solution**:
```bash
cmake .. -DCBT_BUILD_EXAMPLES=ON -DCBT_BUILD_TESTS=ON
make -j$(nproc)
```

### Runtime Issues

#### Issue: Numeric overflow when converting from transform domain

**Problem**: Value too large for native representation.

**Code**:
```cpp
auto huge = cbt::lgd::from_log(1000);
double val = huge.value();  // OVERFLOW!
```

**Solution**:
```cpp
// Option 1: Stay in transform domain
double log_val = huge.log();  // Safe

// Option 2: Use multiscale for extreme values
auto ms = cbt::mappings::lg_to_multiscale(huge);

// Option 3: Check before converting
if(huge.log() < 700) {  // log(DBL_MAX) ≈ 709
    double val = huge.value();
}
```

#### Issue: Incorrect results with negative numbers

**Problem**: Logarithmic transform undefined for negative values.

**Code**:
```cpp
cbt::lgd negative(-5.0);  // Results in -∞
```

**Solution**:
```cpp
// Option 1: Check sign
if(value > 0) {
    cbt::lgd positive(value);
}

// Option 2: Use different transform for signed values
// Option 3: Work with absolute values and track sign separately
```

#### Issue: Loss of precision in multiscale boundaries

**Problem**: Precision loss when values cross scale boundaries.

**Solution**:
```cpp
// Use higher precision base type
cbt::multiscale<long double> precise;

// Or use exact arithmetic where possible
cbt::stern_brocot<int64_t> exact;
```

#### Issue: RNS comparison giving wrong results

**Problem**: Direct comparison in RNS domain is complex.

**Code**:
```cpp
auto a = rns3<int>::from_integer(100);
auto b = rns3<int>::from_integer(200);
// Can't directly compare a < b in RNS!
```

**Solution**:
```cpp
// Convert back for comparison
if(a.to_integer() < b.to_integer()) {
    // ...
}

// Or use mixed radix representation for ordering
```

### Performance Issues

#### Issue: Slower than expected performance

**Problem**: Frequent domain conversions negating benefits.

**Bad Code**:
```cpp
for(int i = 0; i < 1000000; ++i) {
    double val = lg_result.value();  // Expensive exp()
    val *= 2.0;
    lg_result = cbt::lgd(val);  // Expensive log()
}
```

**Solution**:
```cpp
// Stay in transform domain
cbt::lgd factor(2.0);
for(int i = 0; i < 1000000; ++i) {
    lg_result = lg_result * factor;  // Just addition!
}
```

#### Issue: High memory usage

**Problem**: Transform overhead for small datasets.

**Analysis**:
```cpp
sizeof(double) == 8 bytes
sizeof(cbt::lgd) == 8 bytes (same)
sizeof(cbt::multiscale<double>) == 16 bytes (2x)
sizeof(cbt::rns3<int>) == 12 bytes (3x for 3 moduli)
```

**Solution**:
- Use transforms only where benefits outweigh overhead
- Consider batch processing
- Profile memory usage in your specific application

#### Issue: No SIMD speedup with RNS

**Problem**: Compiler not vectorizing RNS operations.

**Solution**:
```cpp
// Enable vectorization hints
#pragma omp simd
for(size_t i = 0; i < n; ++i) {
    result[i] = a[i] + b[i];  // Component-wise
}

// Compile with optimization
g++ -O3 -march=native -fopenmp your_file.cpp
```

### Numerical Stability Issues

#### Issue: Catastrophic cancellation in log domain

**Problem**: Subtracting nearly equal logarithms.

**Code**:
```cpp
auto a = cbt::lgd(1000000);
auto b = cbt::lgd(999999);
auto diff = cbt::lgd(exp(a.log() - b.log()));  // Loss of precision
```

**Solution**:
```cpp
// Use log1p for better accuracy near 1
double log_diff = log1p(exp(b.log() - a.log()));

// Or work with log-sum-exp formulation
```

#### Issue: Accumulating rounding errors

**Problem**: Long chains of operations accumulate error.

**Solution**:
```cpp
// Use higher precision transforms
cbt::lg<long double> high_precision;

// Or use exact arithmetic
cbt::stern_brocot<int64_t> exact;

// Or periodically renormalize
if(operations % 1000 == 0) {
    value = cbt::lgd(value.value());  // Renormalize
}
```

---

## Frequently Asked Questions

### General Questions

**Q: What is a Computational Basis Transform?**

A: A CBT is a systematic transformation from one computational domain to another that changes the complexity landscape of operations. Think of it like choosing the right coordinate system in mathematics - some problems become much simpler in polar coordinates than Cartesian.

**Q: When should I use CBT?**

A: Use CBT when:
- You have a computational bottleneck (e.g., many multiplications)
- Working with extreme numerical ranges
- Need specific mathematical properties (e.g., exact arithmetic)
- Can identify a clear trade-off that benefits your use case

**Q: Is CBT just another math library?**

A: No, CBT is a unified framework that reveals the common pattern underlying many seemingly different techniques (FFT, logarithms, modular arithmetic, etc.). It's about understanding and systematically applying domain transformations.

**Q: What's the performance overhead?**

A: Transform overhead is typically negligible compared to the computational savings. For example, converting to log domain (one log operation) enables thousands of multiplications to become additions. The key is staying in the transform domain.

### Technical Questions

**Q: Can I use CBT with negative numbers?**

A: It depends on the transform:
- **Logarithmic**: No, restricted to positive reals
- **Odds-Ratio**: No, probabilities must be in [0,1]
- **Stern-Brocot**: Yes, with sign tracking
- **RNS**: Yes, using signed representation
- **Multiscale**: Yes, handles all reals

**Q: How do I choose between transforms?**

A: Consider:
1. What operations do you do most frequently?
2. What's your numerical range?
3. Do you need exact or approximate arithmetic?
4. Is parallelism available?
5. What are your memory constraints?

See the selection guide in the main documentation.

**Q: Can transforms be combined?**

A: Yes! Composition is a key feature:
```cpp
// Combine benefits
using ExtremeMult = cbt::multiscale<cbt::lg<double>>;
// Gets both: extreme range AND efficient multiplication
```

**Q: Is CBT thread-safe?**

A: Yes, all CBT classes are immutable and thread-safe. Operations return new instances rather than modifying existing ones.

**Q: How do I handle errors/exceptions?**

A: CBT uses safe defaults:
- Invalid operations (e.g., log of negative) return limiting values (-∞)
- Overflow/underflow handled gracefully
- Only explicit errors (e.g., division by zero in exact arithmetic) throw

### Algorithm-Specific Questions

**Q: How do I implement log-sum-exp with CBT?**

A: Here's a numerically stable implementation:
```cpp
template<typename Iterator>
double log_sum_exp(Iterator begin, Iterator end) {
    if(begin == end) return -INFINITY;
    
    double max_log = std::max_element(begin, end,
        [](const cbt::lgd& a, const cbt::lgd& b) {
            return a.log() < b.log();
        })->log();
    
    cbt::lgd sum(0);
    for(auto it = begin; it != end; ++it) {
        sum = sum + cbt::lgd(exp(it->log() - max_log));
    }
    
    return max_log + sum.log();
}
```

**Q: How do I handle Bayesian networks with CBT?**

A: Use odds-ratio or log-odds transforms:
```cpp
using LogOdds = cbt::lg<cbt::odds_ratio<double>>;

class BayesianNode {
    LogOdds belief;
    
    void update(double likelihood_ratio) {
        belief = belief * LogOdds::from_inner(
            cbt::odds_ratio<double>(likelihood_ratio)
        );
    }
};
```

**Q: Can I use CBT for automatic differentiation?**

A: Yes, use the dual number transform:
```cpp
cbt::dual<double> x(2.0, 1.0);  // value=2, derivative=1
auto y = x * x + cbt::dual<double>(3.0);
// y.value() = 7, y.derivative() = 4
```

### Performance Questions

**Q: Why is my CBT code slower than expected?**

A: Common causes:
1. **Frequent conversions**: Stay in transform domain
2. **Wrong transform**: Ensure the transform matches your operations
3. **Small datasets**: Overhead may dominate for small problems
4. **Missing optimization**: Compile with -O3 and -march=native

**Q: How can I benchmark CBT performance?**

A: Example benchmark:
```cpp
#include <chrono>

template<typename Func>
double benchmark(Func f, int iterations) {
    auto start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < iterations; ++i) {
        f();
    }
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double>(end - start).count();
}

// Compare approaches
double time_normal = benchmark(compute_normal, 10000);
double time_cbt = benchmark(compute_with_cbt, 10000);
std::cout << "Speedup: " << time_normal / time_cbt << "x\n";
```

**Q: Does CBT work with GPU acceleration?**

A: RNS transforms are particularly GPU-friendly:
```cpp
// Component-wise operations map directly to GPU threads
__global__ void rns_multiply(int* a, int* b, int* c, int n) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if(idx < n) {
        c[idx] = (a[idx] * b[idx]) % moduli[idx % NUM_MODULI];
    }
}
```

### Integration Questions

**Q: How do I integrate CBT with existing code?**

A: CBT is designed for incremental adoption:
```cpp
// Wrap existing computations
double existing_calculation(double a, double b) {
    return complicated_formula(a, b);
}

// CBT wrapper
cbt::lgd cbt_calculation(const cbt::lgd& a, const cbt::lgd& b) {
    return cbt::lgd(existing_calculation(a.value(), b.value()));
}

// Gradually move operations into transform domain
```

**Q: Can I use CBT with other libraries?**

A: Yes, CBT is header-only and has no dependencies:
```cpp
// With Eigen
Eigen::MatrixXd matrix;
cbt::lg<Eigen::MatrixXd> log_matrix(matrix);

// With OpenCV
cv::Mat image;
cbt::multiscale<double> pixel_value(image.at<double>(x, y));
```

**Q: How do I serialize CBT objects?**

A: Extract the underlying representation:
```cpp
// Serialization
void save(const cbt::lgd& value, std::ostream& os) {
    os << value.log();
}

// Deserialization
cbt::lgd load(std::istream& is) {
    double log_val;
    is >> log_val;
    return cbt::lgd::from_log(log_val);
}
```

### Advanced Questions

**Q: Can I create custom transforms?**

A: Yes, follow the CBT pattern:
```cpp
template<typename T>
class my_transform {
    T internal_repr;
    
public:
    explicit my_transform(T value) : internal_repr(transform(value)) {}
    T value() const { return inverse_transform(internal_repr); }
    
    // Define operations
    my_transform operator*(const my_transform& other) const {
        return my_transform::from_internal(
            efficient_multiply(internal_repr, other.internal_repr)
        );
    }
};
```

**Q: How do I handle multi-dimensional transforms?**

A: Compose or extend transforms:
```cpp
// 2D logarithmic transform
struct lg2d {
    cbt::lgd x, y;
    
    lg2d(double x_val, double y_val) : x(x_val), y(y_val) {}
    
    double magnitude() const {
        return sqrt(x.value() * x.value() + y.value() * y.value());
    }
};
```

**Q: What about complex number transforms?**

A: Several approaches:
```cpp
// Complex logarithm
struct complex_lg {
    cbt::lgd magnitude;
    double phase;
    
    complex_lg(std::complex<double> z) 
        : magnitude(std::abs(z)), phase(std::arg(z)) {}
};
```

### Debugging Questions

**Q: How do I debug CBT code?**

A: Tips for debugging:
```cpp
// Add debug output
template<typename T>
void debug_transform(const cbt::lg<T>& value, const std::string& name) {
    std::cerr << name << ": log=" << value.log() 
              << ", value=" << value.value() << std::endl;
}

// Check for edge cases
assert(value.log() > -INFINITY && "Unexpected -infinity");
assert(value.log() < 700 && "Potential overflow on conversion");

// Use conditional compilation
#ifdef CBT_DEBUG
    validate_transform_invariants(value);
#endif
```

**Q: How do I profile CBT performance?**

A: Use standard profiling tools:
```bash
# Compile with profiling
g++ -pg -O2 your_code.cpp

# Run and generate profile
./a.out
gprof a.out gmon.out > analysis.txt

# Or use perf
perf record ./a.out
perf report
```

---

## Getting Help

### Resources

- **Documentation**: `/docs` folder in the repository
- **Examples**: `/examples` folder with working code
- **Tests**: `/tests` for usage patterns
- **Paper**: `/paper/cbt.pdf` for theoretical background

### Community Support

- **GitHub Issues**: Report bugs or request features
- **Discussions**: GitHub Discussions for questions
- **Stack Overflow**: Tag with `cbt` and `c++`

### Commercial Support

For commercial support, consulting, or custom transform development, contact the maintainers through GitHub.

---

## Quick Debugging Checklist

When something goes wrong, check:

1. ✓ Compiler supports C++17?
2. ✓ Include paths set correctly?
3. ✓ Using appropriate transform for your data?
4. ✓ Staying in transform domain (not converting unnecessarily)?
5. ✓ Input values in valid range for transform?
6. ✓ Compiled with optimizations enabled?
7. ✓ Memory alignment for SIMD (if using)?
8. ✓ Thread safety (if multithreaded)?

---

## Error Messages Reference

| Error | Likely Cause | Solution |
|-------|-------------|----------|
| `static_assert failed: lg requires floating-point` | Wrong type parameter | Use float/double/long double |
| `std::domain_error` | Invalid operation (e.g., 0/0) | Check input validity |
| `std::overflow_error` | Result too large | Use multiscale transform |
| `std::bad_alloc` | Out of memory | Reduce problem size or optimize memory usage |

---

## Performance Tuning Guide

### Compiler Flags

```bash
# Maximum optimization
g++ -O3 -march=native -mtune=native -flto

# With OpenMP for parallelism
g++ -O3 -fopenmp

# Profile-guided optimization
g++ -fprofile-generate
./a.out  # Run with representative data
g++ -fprofile-use -O3
```

### Memory Optimization

```cpp
// Align for SIMD
alignas(64) cbt::lgd values[1024];

// Use reserve for vectors
std::vector<cbt::lgd> data;
data.reserve(expected_size);

// Consider memory pools for frequent allocations
```

### Algorithm Selection

| If your bottleneck is... | Consider using... |
|-------------------------|-------------------|
| Multiplication chains | Logarithmic transform |
| Bayesian updates | Odds-ratio transform |
| Wide dynamic range | Multiscale transform |
| Parallel hardware | RNS transform |
| Exact arithmetic | Stern-Brocot transform |