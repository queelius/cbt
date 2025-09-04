# Quick Start Guide

## Installation in 30 Seconds

```bash
# Clone the repository
git clone https://github.com/[username]/cbt.git

# Include in your C++ file
#include <cbt/cbt.hpp>

# Compile with C++17
g++ -std=c++17 -I/path/to/cbt/include your_file.cpp
```

That's it! CBT is header-only, so no building or linking required.

## Your First CBT Program

```cpp
#include <iostream>
#include <cbt/cbt.hpp>

int main() {
    using namespace cbt;
    
    // Problem: Multiply many small numbers
    lgd product(0.1);
    for(int i = 0; i < 100; ++i) {
        product = product * lgd(0.1);
    }
    
    std::cout << "0.1^100 = " << product.value() << std::endl;
    // Works correctly! No underflow to zero
    
    return 0;
}
```

## Five-Minute Tutorial

### 1. Choose Your Transform

Ask yourself: What operation do I do most frequently?

- **Multiplication?** → Use `lg<T>` (logarithmic)
- **Bayesian updates?** → Use `odds_ratio<T>`
- **Exact fractions?** → Use `stern_brocot<T>`
- **Parallel ops?** → Use `residue_number_system<T,N>`
- **Extreme scales?** → Use `multiscale<T>`

### 2. Transform Your Data

```cpp
// From normal domain to transform domain
cbt::lgd log_value(1000.0);

// Or construct directly in transform domain
auto huge = cbt::lgd::from_log(500);  // e^500
```

### 3. Compute Efficiently

```cpp
// Multiplication is now just addition internally
auto result = log_value * huge;  // Fast!

// Chain operations stay in transform domain
auto final = result * lgd(2.0) * lgd(3.14);
```

### 4. Convert Back (If Needed)

```cpp
// Get result in original domain
double normal_value = final.value();

// Or stay in transform domain for efficiency
double log_result = final.log();  // Often more useful
```

## Common Use Cases

### Case 1: Machine Learning - Stable Probability Computations

**Problem:** Computing likelihood of data given model parameters often involves products of many small probabilities.

```cpp
#include <cbt/logarithmic.hpp>
#include <vector>

double compute_log_likelihood(const std::vector<double>& probabilities) {
    cbt::lgd likelihood(1.0);
    
    for(double p : probabilities) {
        likelihood = likelihood * cbt::lgd(p);
    }
    
    // Return log-likelihood (more stable than likelihood itself)
    return likelihood.log();
}

// Example: Document classification
std::vector<double> word_probs = {0.001, 0.0001, 0.002, /*...*/};
double log_score = compute_log_likelihood(word_probs);
```

### Case 2: Medical Diagnosis - Sequential Bayesian Inference

**Problem:** Update disease probability as test results arrive.

```cpp
#include <cbt/odds_ratio.hpp>

class DiagnosticSystem {
private:
    cbt::odds_ratio<double> current_odds;
    
public:
    DiagnosticSystem(double disease_prevalence) 
        : current_odds(cbt::odds_ratio<double>::from_probability(disease_prevalence)) {}
    
    void add_test_result(bool positive, double sensitivity, double specificity) {
        double likelihood_ratio = positive ? 
            sensitivity / (1.0 - specificity) :
            (1.0 - sensitivity) / specificity;
            
        current_odds = current_odds * cbt::odds_ratio<double>(likelihood_ratio);
    }
    
    double get_probability() const {
        return current_odds.to_probability();
    }
};

// Usage
DiagnosticSystem covid_diagnosis(0.01);  // 1% prevalence
covid_diagnosis.add_test_result(true, 0.98, 0.95);   // Positive rapid test
covid_diagnosis.add_test_result(true, 0.995, 0.98);  // Positive PCR
std::cout << "Probability of COVID: " << covid_diagnosis.get_probability() << std::endl;
```

### Case 3: Scientific Computing - Multi-Scale Physics

**Problem:** Simulate phenomena across vastly different scales.

```cpp
#include <cbt/multiscale.hpp>

void quantum_to_cosmic_simulation() {
    using namespace cbt;
    
    // Define scale constants
    multiscale<double> planck_length(1.616e-35);  // Smallest meaningful length
    multiscale<double> proton_radius(8.4e-16);    // Subatomic
    multiscale<double> earth_radius(6.371e6);     // Planetary
    multiscale<double> light_year(9.461e15);      // Astronomical
    
    // Compute ratios without overflow
    auto earth_to_planck = earth_radius / planck_length;
    std::cout << "Earth radius in Planck lengths: " 
              << earth_to_planck.mantissa() 
              << "e" << earth_to_planck.scale_level() << std::endl;
    
    // Chain calculations across scales
    auto volume_ratio = (earth_radius / proton_radius).pow(3);
    std::cout << "Volume ratio: " << volume_ratio << std::endl;
}
```

### Case 4: Financial Computing - Exact Decimal Arithmetic

**Problem:** Monetary calculations require exact decimal arithmetic.

```cpp
#include <cbt/stern_brocot.hpp>

class MoneyCalculator {
private:
    using Money = cbt::stern_brocot<int64_t>;
    
public:
    Money from_dollars(double dollars) {
        // Convert to cents internally for exactness
        int64_t cents = static_cast<int64_t>(dollars * 100 + 0.5);
        return Money(cents, 100);
    }
    
    double to_dollars(const Money& amount) {
        return amount.to_double();
    }
    
    Money calculate_interest(const Money& principal, const Money& rate, int years) {
        Money result = principal;
        for(int i = 0; i < years; ++i) {
            result = result * (Money(1, 1) + rate);
        }
        return result;
    }
};

// Usage
MoneyCalculator calc;
auto principal = calc.from_dollars(10000.00);
auto rate = cbt::stern_brocot<int64_t>(5, 100);  // 5% interest
auto final_amount = calc.calculate_interest(principal, rate, 10);
std::cout << "After 10 years: $" << calc.to_dollars(final_amount) << std::endl;
```

### Case 5: Cryptography - Parallel Modular Arithmetic

**Problem:** Perform many modular operations efficiently.

```cpp
#include <cbt/residue_number_system.hpp>

void parallel_crypto_operation() {
    using namespace cbt;
    using RNS = residue_number_system<uint32_t, 4>;
    
    // Large numbers for cryptographic operations
    uint32_t message = 0xDEADBEEF;
    uint32_t key = 0xCAFEBABE;
    
    // Convert to RNS for parallel processing
    auto rns_message = RNS::from_integer(message);
    auto rns_key = RNS::from_integer(key);
    
    // Operations are component-wise (parallelizable on GPU/SIMD)
    auto encrypted = rns_message * rns_key;
    
    // Convert back when needed
    uint32_t result = encrypted.to_integer();
}
```

## Composition Pattern

Combine transforms for multiplicative benefits:

```cpp
#include <cbt/composed.hpp>

// Problem: Extreme-scale multiplication
// Solution: Combine multiscale (range) with logarithmic (multiplication)
using ExtremeMult = cbt::multiscale<cbt::lg<double>>;

ExtremeMult tiny(1e-300);
ExtremeMult huge(1e300);

// This works without underflow OR overflow!
auto result = tiny * huge;

// Access via composition
double log_value = result.inner().log();  // Logarithmic value
int scale = result.scale_level();         // Multiscale level
```

## Best Practices

### 1. Stay in Transform Domain

```cpp
// BAD: Constant conversion
for(int i = 0; i < 1000; ++i) {
    double val = lg_value.value();  // Expensive exp()
    val *= 2.0;
    lg_value = lgd(val);  // Expensive log()
}

// GOOD: Stay in transform domain
for(int i = 0; i < 1000; ++i) {
    lg_value = lg_value * lgd(2.0);  // Just addition internally
}
```

### 2. Use Factory Methods for Extreme Values

```cpp
// BAD: Will overflow
cbt::lgd huge(1e400);  // OVERFLOW!

// GOOD: Construct directly in log domain
auto huge = cbt::lgd::from_log(920);  // e^920, no overflow
```

### 3. Choose the Right Transform

```cpp
// For Bayesian inference
cbt::odds_ratio<double> posterior;  // Natural for sequential updates

// For numerical stability in products
cbt::lgd stable_product;  // Prevents underflow

// For exact arithmetic
cbt::stern_brocot<int> exact_fraction;  // No rounding errors
```

### 4. Leverage Composition

```cpp
// When you need benefits of multiple transforms
using StableBayes = cbt::lg<cbt::odds_ratio<double>>;
StableBayes very_small_posterior;  // Handles extremely small probabilities
```

## Performance Tips

### Memory Layout

```cpp
// Efficient: Contiguous memory
std::vector<cbt::lgd> log_values(1000);

// Consider alignment for SIMD
alignas(32) cbt::lgd aligned_values[8];
```

### Parallelization

```cpp
// RNS operations are naturally parallel
#pragma omp parallel for
for(int i = 0; i < n; ++i) {
    rns_results[i] = rns_a[i] * rns_b[i];  // Component-wise
}
```

### Compile-Time Optimization

```cpp
// Use constexpr where possible
constexpr cbt::lgd log_two = cbt::lgd(2.0);
constexpr auto log_four = log_two * log_two;  // Computed at compile time
```

## Common Pitfalls and Solutions

### Pitfall 1: Negative Values in Logarithmic Transform

```cpp
// PROBLEM
cbt::lgd bad(-5.0);  // Undefined! log(-5) doesn't exist

// SOLUTION
// Option 1: Check before transforming
if(value > 0) {
    cbt::lgd good(value);
}

// Option 2: Use complex logarithm (if appropriate)
// Option 3: Use a different transform
```

### Pitfall 2: Precision Loss in Multiscale

```cpp
// PROBLEM
multiscale<float> imprecise;  // Limited mantissa precision

// SOLUTION
multiscale<double> precise;  // Better precision
// Or use exact arithmetic
stern_brocot<int64_t> exact;
```

### Pitfall 3: Overflow in Back-Conversion

```cpp
// PROBLEM
auto huge = lgd::from_log(1000);
double val = huge.value();  // OVERFLOW!

// SOLUTION
// Stay in log domain
double log_val = huge.log();  // Safe

// Or use multiscale for extreme values
auto ms = mappings::lg_to_multiscale(huge);
```

## Next Steps

1. **Explore Examples**: Check out `examples/cbt_demo.cpp` for comprehensive demonstrations

2. **Read the Theory**: Understanding the mathematical foundations in `docs/theory.md` will deepen your intuition

3. **Try Composition**: Experiment with combining transforms for your specific use case

4. **Benchmark**: Measure performance improvements in your actual application

5. **Contribute**: Found a new transform? Share it with the community!

## Getting Help

- **API Reference**: Detailed documentation in `docs/api_reference.md`
- **Issue Tracker**: Report bugs or request features on GitHub
- **Academic Paper**: Read `paper/cbt.pdf` for formal treatment
- **Community**: Join discussions in GitHub Issues

## Quick Reference Card

```cpp
// Logarithmic - Multiplication to addition
cbt::lgd log_val(100.0);
auto squared = log_val * log_val;

// Odds-Ratio - Bayesian updates
auto odds = cbt::odds_ratio<double>::from_probability(0.1);
odds = odds * cbt::odds_ratio<double>(likelihood_ratio);

// Stern-Brocot - Exact rationals  
cbt::stern_brocot<int> third(1, 3);
auto two_thirds = third + third;

// RNS - Parallel arithmetic
auto rns = cbt::rns3<int>::from_integer(12345);

// Multiscale - Extreme ranges
cbt::multiscale<double> tiny(1e-100);

// Composition - Combined benefits
using LogOdds = cbt::lg<cbt::odds_ratio<double>>;
```