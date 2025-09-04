# Transform Usage Guides

## Overview

This guide provides in-depth coverage of each transform in the CBT library, including:
- Mathematical foundations
- When to use (and when not to use)
- Performance characteristics
- Real-world examples
- Common patterns and anti-patterns

---

## Logarithmic Transform (`cbt::lg<T>`)

### Mathematical Foundation

The logarithmic transform maps positive real numbers to the extended real line through the natural logarithm:

```
φ: ℝ⁺ → ℝ ∪ {-∞}
φ(x) = ln(x)
φ⁻¹(y) = e^y
```

### Key Properties

- **Homomorphism**: `log(a×b) = log(a) + log(b)`
- **Domain**: Positive reals only
- **Range**: All reals (including -∞ for zero)
- **Precision**: Relative precision maintained across scales

### When to Use

✅ **Use when:**
- Multiplying many numbers
- Computing products with extreme ranges
- Working with probabilities in machine learning
- Implementing numerical optimization
- Computing geometric means

❌ **Avoid when:**
- Numbers can be negative
- Addition is the primary operation
- Working with complex numbers
- Exact arithmetic is required

### Performance Profile

| Operation | Original Domain | Transform Domain | Speedup |
|-----------|----------------|------------------|---------|
| Multiply | O(n²) | O(n) | ~n× |
| Power | O(log k) | O(1) | ~log k× |
| Product of n terms | O(n) | O(n) | Better numerical stability |

### Detailed Examples

#### Example 1: Stable Likelihood Computation

```cpp
#include <cbt/logarithmic.hpp>
#include <vector>
#include <numeric>

class GaussianMixtureModel {
private:
    struct Component {
        double mean, variance, weight;
    };
    std::vector<Component> components;
    
public:
    double log_likelihood(const std::vector<double>& data) {
        cbt::lgd total_likelihood(0);  // Start with log(0) = -∞
        
        for(const auto& x : data) {
            cbt::lgd point_likelihood(0);
            
            for(const auto& c : components) {
                // Compute Gaussian probability
                double exponent = -0.5 * pow(x - c.mean, 2) / c.variance;
                double normalizer = sqrt(2 * M_PI * c.variance);
                
                // Use log domain to prevent underflow
                auto component_prob = cbt::lgd::from_log(
                    exponent - log(normalizer) + log(c.weight)
                );
                
                // Sum probabilities (expensive but necessary)
                if(point_likelihood.log() == -INFINITY) {
                    point_likelihood = component_prob;
                } else {
                    // log(a + b) = log(a) + log(1 + exp(log(b) - log(a)))
                    double diff = component_prob.log() - point_likelihood.log();
                    if(diff > 0) {
                        point_likelihood = cbt::lgd::from_log(
                            component_prob.log() + log1p(exp(-diff))
                        );
                    } else {
                        point_likelihood = cbt::lgd::from_log(
                            point_likelihood.log() + log1p(exp(diff))
                        );
                    }
                }
            }
            
            total_likelihood = total_likelihood * point_likelihood;
        }
        
        return total_likelihood.log();
    }
};
```

#### Example 2: Geometric Mean with Extreme Values

```cpp
template<typename Iterator>
double geometric_mean_stable(Iterator begin, Iterator end) {
    size_t count = 0;
    cbt::lgd product(1.0);
    
    for(auto it = begin; it != end; ++it) {
        product = product * cbt::lgd(*it);
        ++count;
    }
    
    // Geometric mean = (product)^(1/n)
    return product.pow(1.0 / count).value();
}

// Usage
std::vector<double> values = {1e-50, 1e100, 1e-30, 1e80};
double gmean = geometric_mean_stable(values.begin(), values.end());
// Works despite 150 order of magnitude range!
```

### Common Patterns

#### Pattern 1: Log-Sum-Exp Trick

```cpp
double log_sum_exp(const std::vector<double>& log_values) {
    if(log_values.empty()) return -INFINITY;
    
    // Find maximum for numerical stability
    double max_val = *std::max_element(log_values.begin(), log_values.end());
    if(max_val == -INFINITY) return -INFINITY;
    
    // Compute log(sum(exp(values)))
    double sum = 0;
    for(double log_val : log_values) {
        sum += exp(log_val - max_val);
    }
    
    return max_val + log(sum);
}
```

#### Pattern 2: Power Iterations

```cpp
// Computing x^n for large n
cbt::lgd power_stable(double x, int n) {
    return cbt::lgd(x).pow(n);
}

// Chain of multiplications
cbt::lgd chain_multiply(const std::vector<double>& factors) {
    return std::accumulate(factors.begin(), factors.end(),
                          cbt::lgd(1.0),
                          [](const cbt::lgd& acc, double val) {
                              return acc * cbt::lgd(val);
                          });
}
```

---

## Odds-Ratio Transform (`cbt::odds_ratio<T>`)

### Mathematical Foundation

The odds-ratio transform maps probabilities to odds:

```
φ: [0,1] → [0,∞]
φ(p) = p/(1-p)
φ⁻¹(o) = o/(1+o)
```

### Key Properties

- **Bayesian Update**: `posterior_odds = prior_odds × likelihood_ratio`
- **Log-Odds**: Often combined with logarithmic transform
- **Symmetry**: `odds(p) × odds(1-p) = 1`
- **No Normalization**: Updates don't require normalization constant

### When to Use

✅ **Use when:**
- Performing Bayesian inference
- Sequential probability updates
- Medical diagnosis systems
- Spam filtering
- A/B testing analysis

❌ **Avoid when:**
- Need to add probabilities
- Working outside [0,1] range
- Require probability distributions (not just ratios)

### Detailed Examples

#### Example 1: Medical Diagnostic System

```cpp
#include <cbt/odds_ratio.hpp>
#include <vector>
#include <map>

class MedicalDiagnosticEngine {
private:
    struct Test {
        std::string name;
        double sensitivity;    // P(+|disease)
        double specificity;    // P(-|no disease)
    };
    
    struct Disease {
        std::string name;
        double prevalence;
        std::vector<Test> available_tests;
    };
    
    std::map<std::string, Disease> diseases;
    
public:
    struct Diagnosis {
        std::string disease_name;
        cbt::odds_ratio<double> current_odds;
        std::vector<std::string> tests_performed;
        
        double probability() const {
            return current_odds.to_probability();
        }
    };
    
    Diagnosis start_diagnosis(const std::string& disease_name) {
        const auto& disease = diseases.at(disease_name);
        return {
            disease_name,
            cbt::odds_ratio<double>::from_probability(disease.prevalence),
            {}
        };
    }
    
    void apply_test(Diagnosis& diagnosis, 
                    const std::string& test_name, 
                    bool positive_result) {
        const auto& disease = diseases.at(diagnosis.disease_name);
        auto test_it = std::find_if(disease.available_tests.begin(),
                                    disease.available_tests.end(),
                                    [&](const Test& t) { 
                                        return t.name == test_name; 
                                    });
        
        if(test_it != disease.available_tests.end()) {
            double likelihood_ratio;
            if(positive_result) {
                // LR+ = sensitivity / (1 - specificity)
                likelihood_ratio = test_it->sensitivity / 
                                 (1.0 - test_it->specificity);
            } else {
                // LR- = (1 - sensitivity) / specificity
                likelihood_ratio = (1.0 - test_it->sensitivity) / 
                                  test_it->specificity;
            }
            
            // Bayesian update via multiplication
            diagnosis.current_odds = diagnosis.current_odds * 
                                    cbt::odds_ratio<double>(likelihood_ratio);
            diagnosis.tests_performed.push_back(test_name);
        }
    }
    
    void add_disease(const std::string& name, double prevalence) {
        diseases[name] = {name, prevalence, {}};
    }
    
    void add_test(const std::string& disease_name,
                  const std::string& test_name,
                  double sensitivity,
                  double specificity) {
        diseases[disease_name].available_tests.push_back(
            {test_name, sensitivity, specificity}
        );
    }
};

// Usage example
int main() {
    MedicalDiagnosticEngine engine;
    
    // Configure diseases and tests
    engine.add_disease("COVID-19", 0.01);  // 1% prevalence
    engine.add_test("COVID-19", "Rapid Antigen", 0.85, 0.95);
    engine.add_test("COVID-19", "RT-PCR", 0.98, 0.99);
    
    // Perform diagnosis
    auto diagnosis = engine.start_diagnosis("COVID-19");
    std::cout << "Initial probability: " << diagnosis.probability() << std::endl;
    
    engine.apply_test(diagnosis, "Rapid Antigen", true);
    std::cout << "After positive rapid test: " << diagnosis.probability() << std::endl;
    
    engine.apply_test(diagnosis, "RT-PCR", true);
    std::cout << "After positive PCR: " << diagnosis.probability() << std::endl;
    
    return 0;
}
```

#### Example 2: A/B Testing Framework

```cpp
class ABTestAnalyzer {
private:
    struct Variant {
        int successes;
        int trials;
        
        double conversion_rate() const {
            return static_cast<double>(successes) / trials;
        }
    };
    
public:
    // Bayesian A/B test using odds ratios
    double probability_a_better_than_b(const Variant& a, const Variant& b) {
        // Use Beta-Binomial conjugate prior
        // Approximate with normal for large samples
        
        double rate_a = a.conversion_rate();
        double rate_b = b.conversion_rate();
        
        // Convert to odds
        auto odds_a = cbt::odds_ratio<double>::from_probability(rate_a);
        auto odds_b = cbt::odds_ratio<double>::from_probability(rate_b);
        
        // Compute odds ratio
        auto odds_ratio = odds_a.value() / odds_b.value();
        
        // Use Monte Carlo or analytical approximation
        return compute_probability_superiority(odds_ratio, a.trials, b.trials);
    }
    
private:
    double compute_probability_superiority(double odds_ratio, int n_a, int n_b) {
        // Simplified - in practice use proper Bayesian computation
        // This demonstrates the odds ratio usage
        if(odds_ratio > 1) {
            return 0.5 + 0.5 * tanh(log(odds_ratio) * sqrt(n_a + n_b) / 4);
        } else {
            return 0.5 - 0.5 * tanh(-log(odds_ratio) * sqrt(n_a + n_b) / 4);
        }
    }
};
```

---

## Stern-Brocot Transform (`cbt::stern_brocot<T>`)

### Mathematical Foundation

The Stern-Brocot tree provides a systematic enumeration of all positive rationals:

```
Each rational p/q appears exactly once in lowest terms
Tree structure enables binary search for approximations
Mediants provide optimal convergents
```

### Key Properties

- **Uniqueness**: Every positive rational appears exactly once
- **Order**: Tree maintains ordering property
- **Optimality**: Convergents are best rational approximations
- **Exactness**: No rounding errors in arithmetic

### When to Use

✅ **Use when:**
- Exact rational arithmetic required
- Finding best rational approximations
- Music theory (frequency ratios)
- Computer algebra systems
- Financial calculations requiring exactness

❌ **Avoid when:**
- Working with irrationals
- Need floating-point performance
- Memory is severely constrained

### Detailed Examples

#### Example 1: Musical Interval Calculator

```cpp
class MusicalIntervals {
private:
    using Rational = cbt::stern_brocot<int>;
    
    struct Interval {
        std::string name;
        Rational ratio;
        double cents;  // 1200 * log2(ratio)
    };
    
    std::vector<Interval> intervals;
    
public:
    MusicalIntervals() {
        // Just intonation intervals
        intervals = {
            {"Unison", Rational(1, 1), 0},
            {"Minor Second", Rational(16, 15), 111.73},
            {"Major Second", Rational(9, 8), 203.91},
            {"Minor Third", Rational(6, 5), 315.64},
            {"Major Third", Rational(5, 4), 386.31},
            {"Perfect Fourth", Rational(4, 3), 498.04},
            {"Tritone", Rational(45, 32), 590.22},
            {"Perfect Fifth", Rational(3, 2), 701.96},
            {"Minor Sixth", Rational(8, 5), 813.69},
            {"Major Sixth", Rational(5, 3), 884.36},
            {"Minor Seventh", Rational(16, 9), 996.09},
            {"Major Seventh", Rational(15, 8), 1088.27},
            {"Octave", Rational(2, 1), 1200}
        };
    }
    
    // Find closest musical interval
    std::string identify_interval(double frequency_ratio) {
        auto approx = Rational::approximate(frequency_ratio, 100);
        
        double min_diff = INFINITY;
        std::string closest;
        
        for(const auto& interval : intervals) {
            double diff = std::abs(approx.to_double() - interval.ratio.to_double());
            if(diff < min_diff) {
                min_diff = diff;
                closest = interval.name;
            }
        }
        
        return closest;
    }
    
    // Combine intervals (multiplication of ratios)
    Rational combine_intervals(const std::string& interval1,
                               const std::string& interval2) {
        auto it1 = std::find_if(intervals.begin(), intervals.end(),
                                [&](const Interval& i) { 
                                    return i.name == interval1; 
                                });
        auto it2 = std::find_if(intervals.begin(), intervals.end(),
                                [&](const Interval& i) { 
                                    return i.name == interval2; 
                                });
        
        return it1->ratio * it2->ratio;
    }
    
    // Generate harmonic series
    std::vector<Rational> harmonic_series(int n) {
        std::vector<Rational> series;
        for(int i = 1; i <= n; ++i) {
            series.push_back(Rational(i, 1));
        }
        return series;
    }
};
```

#### Example 2: Continued Fraction Approximation

```cpp
class ContinuedFractionAnalyzer {
private:
    using Rational = cbt::stern_brocot<int64_t>;
    
public:
    struct Convergent {
        Rational value;
        double error;
        int denominator_size;
    };
    
    std::vector<Convergent> find_convergents(double x, int max_terms) {
        std::vector<Convergent> convergents;
        std::vector<int> cf_terms;
        
        // Generate continued fraction representation
        double remainder = x;
        for(int i = 0; i < max_terms && remainder > 1e-10; ++i) {
            int term = static_cast<int>(floor(remainder));
            cf_terms.push_back(term);
            remainder = 1.0 / (remainder - term);
        }
        
        // Build convergents
        Rational p_prev(0, 1), p_curr(1, 1);
        Rational q_prev(1, 1), q_curr(0, 1);
        
        for(int term : cf_terms) {
            Rational p_next = p_curr * Rational(term, 1) + p_prev;
            Rational q_next = q_curr * Rational(term, 1) + q_prev;
            
            Rational convergent = p_next / q_next;
            convergents.push_back({
                convergent,
                std::abs(x - convergent.to_double()),
                static_cast<int>(q_next.denominator())
            });
            
            p_prev = p_curr; p_curr = p_next;
            q_prev = q_curr; q_curr = q_next;
        }
        
        return convergents;
    }
    
    // Find best rational approximation with denominator limit
    Rational best_approximation(double x, int max_denominator) {
        return Rational::approximate(x, max_denominator);
    }
};

// Example: Approximate pi
int main() {
    ContinuedFractionAnalyzer analyzer;
    
    auto convergents = analyzer.find_convergents(M_PI, 10);
    
    std::cout << "Convergents of π:\n";
    for(const auto& c : convergents) {
        std::cout << c.value << " = " << c.value.to_double()
                  << ", error = " << c.error << "\n";
    }
    
    // Famous approximations
    auto pi_22_7 = analyzer.best_approximation(M_PI, 7);
    auto pi_355_113 = analyzer.best_approximation(M_PI, 113);
    
    std::cout << "\nClassical approximations:\n";
    std::cout << "22/7 = " << pi_22_7 << "\n";
    std::cout << "355/113 = " << pi_355_113 << "\n";
}
```

---

## Residue Number System (`cbt::residue_number_system<T,N>`)

### Mathematical Foundation

RNS represents integers as tuples of residues modulo coprime bases:

```
x → (x mod p₁, x mod p₂, ..., x mod pₙ)
Operations performed component-wise
Chinese Remainder Theorem for reconstruction
```

### Key Properties

- **Parallelism**: All components independent
- **No Carries**: Addition/multiplication without carry propagation  
- **Fault Tolerance**: Error detection via redundant moduli
- **Range**: Product of all moduli

### When to Use

✅ **Use when:**
- Parallel hardware available (GPU, FPGA, SIMD)
- Cryptographic operations
- Digital signal processing
- Fault-tolerant computing
- Need constant-time arithmetic

❌ **Avoid when:**
- Comparison operations frequent
- Division operations needed
- Sequential processing only
- Dynamic range requirements

### Detailed Examples

#### Example 1: Parallel DSP Filter

```cpp
#include <cbt/residue_number_system.hpp>
#include <vector>
#include <execution>

template<int N>
class ParallelFIRFilter {
private:
    using RNS = cbt::residue_number_system<int32_t, N>;
    std::vector<RNS> coefficients;
    std::vector<RNS> delay_line;
    
public:
    ParallelFIRFilter(const std::vector<int32_t>& filter_coeffs) {
        for(auto coeff : filter_coeffs) {
            coefficients.push_back(RNS::from_integer(coeff));
        }
        delay_line.resize(coefficients.size());
    }
    
    int32_t process_sample(int32_t input) {
        // Shift delay line
        for(size_t i = delay_line.size() - 1; i > 0; --i) {
            delay_line[i] = delay_line[i-1];
        }
        delay_line[0] = RNS::from_integer(input);
        
        // Compute filter output (fully parallel)
        RNS output = RNS::from_integer(0);
        
        #pragma omp parallel for reduction(+:output)
        for(size_t i = 0; i < coefficients.size(); ++i) {
            output = output + (coefficients[i] * delay_line[i]);
        }
        
        return output.to_integer();
    }
    
    std::vector<int32_t> process_block(const std::vector<int32_t>& input) {
        std::vector<int32_t> output;
        output.reserve(input.size());
        
        for(auto sample : input) {
            output.push_back(process_sample(sample));
        }
        
        return output;
    }
};
```

#### Example 2: Cryptographic Operations

```cpp
class RSACrypto {
private:
    using RNS = cbt::residue_number_system<uint64_t, 5>;
    
public:
    struct PublicKey {
        RNS n;  // modulus
        RNS e;  // public exponent
    };
    
    struct PrivateKey {
        RNS n;  // modulus  
        RNS d;  // private exponent
    };
    
    // Modular exponentiation using RNS
    RNS mod_exp(const RNS& base, const RNS& exp, const RNS& mod) {
        RNS result = RNS::from_integer(1);
        RNS b = base;
        uint64_t e = exp.to_integer();
        
        while(e > 0) {
            if(e & 1) {
                result = (result * b);  // Component-wise, parallel
            }
            b = b * b;  // Component-wise, parallel
            e >>= 1;
        }
        
        return result;
    }
    
    RNS encrypt(const RNS& message, const PublicKey& key) {
        return mod_exp(message, key.e, key.n);
    }
    
    RNS decrypt(const RNS& ciphertext, const PrivateKey& key) {
        return mod_exp(ciphertext, key.d, key.n);
    }
};
```

---

## Multiscale Transform (`cbt::multiscale<T>`)

### Mathematical Foundation

Represents values as (mantissa, scale_level) pairs:

```
value = mantissa × base^scale_level
Automatic scale selection for optimal precision
Handles ~200 orders of magnitude
```

### Key Properties

- **Dynamic Range**: From 10^-100 to 10^100 or more
- **Automatic Scaling**: Transparent scale management
- **Precision Preservation**: Maintains relative precision
- **Scale Transitions**: Smooth handling of scale boundaries

### When to Use

✅ **Use when:**
- Values span many orders of magnitude
- Physical simulations (quantum to cosmic)
- Automatic scale management needed
- Avoiding manual scaling code
- Multi-scale phenomena

❌ **Avoid when:**
- Fixed-point precision required
- Scale is known and fixed
- Exact arithmetic needed
- Memory is very constrained

### Detailed Examples

#### Example 1: Multi-Scale Physics Engine

```cpp
class MultiScalePhysics {
private:
    using Scale = cbt::multiscale<double>;
    
public:
    struct Particle {
        Scale mass;
        Scale position[3];
        Scale velocity[3];
        Scale energy;
    };
    
    // Constants across scales
    const Scale planck_constant{6.626e-34};
    const Scale speed_of_light{299792458};
    const Scale gravitational_constant{6.674e-11};
    
    // Compute de Broglie wavelength
    Scale de_broglie_wavelength(const Particle& p) {
        Scale momentum = p.mass * 
                         Scale(sqrt(pow(p.velocity[0].to_value(), 2) +
                                   pow(p.velocity[1].to_value(), 2) +
                                   pow(p.velocity[2].to_value(), 2)));
        return planck_constant / momentum;
    }
    
    // Schwarzschild radius
    Scale schwarzschild_radius(const Scale& mass) {
        return Scale(2.0) * gravitational_constant * mass / 
               (speed_of_light * speed_of_light);
    }
    
    // Quantum to cosmic scale example
    void demonstrate_scales() {
        // Electron
        Particle electron;
        electron.mass = Scale(9.109e-31);  // kg
        electron.velocity[0] = Scale(2.2e6);  // m/s
        
        auto electron_wavelength = de_broglie_wavelength(electron);
        std::cout << "Electron wavelength: " << electron_wavelength << " m\n";
        
        // Black hole
        Scale solar_mass(1.989e30);  // kg
        Scale black_hole_mass = solar_mass * Scale(10);  // 10 solar masses
        
        auto bh_radius = schwarzschild_radius(black_hole_mass);
        std::cout << "Black hole radius: " << bh_radius << " m\n";
        
        // Scale ratio
        auto scale_ratio = bh_radius / electron_wavelength;
        std::cout << "Scale ratio: " << scale_ratio << "\n";
    }
};
```

#### Example 2: Adaptive Numerical Integration

```cpp
template<typename Func>
class AdaptiveIntegrator {
private:
    using Scale = cbt::multiscale<double>;
    
public:
    Scale integrate(Func f, double a, double b, double tolerance) {
        return adaptive_simpson(f, Scale(a), Scale(b), tolerance, 10);
    }
    
private:
    Scale adaptive_simpson(Func f, Scale a, Scale b, 
                          double tol, int max_depth) {
        Scale c = (a + b) / Scale(2.0);
        Scale h = (b - a) / Scale(2.0);
        
        Scale fa = Scale(f(a.to_value()));
        Scale fb = Scale(f(b.to_value()));
        Scale fc = Scale(f(c.to_value()));
        
        Scale s = h / Scale(3.0) * (fa + Scale(4.0) * fc + fb);
        
        if(max_depth == 0) {
            return s;
        }
        
        Scale d = (a + c) / Scale(2.0);
        Scale e = (c + b) / Scale(2.0);
        Scale fd = Scale(f(d.to_value()));
        Scale fe = Scale(f(e.to_value()));
        
        Scale sl = h / Scale(6.0) * (fa + Scale(4.0) * fd + fc);
        Scale sr = h / Scale(6.0) * (fc + Scale(4.0) * fe + fb);
        Scale s2 = sl + sr;
        
        // Check convergence (handles multiple scales)
        if(std::abs((s2 - s).to_value() / s.to_value()) < tol) {
            return s2;
        }
        
        // Recursive refinement
        return adaptive_simpson(f, a, c, tol/2, max_depth-1) +
               adaptive_simpson(f, c, b, tol/2, max_depth-1);
    }
};

// Example: Integrate function with extreme values
int main() {
    AdaptiveIntegrator<double(*)(double)> integrator;
    
    // Function with multiple scales
    auto f = [](double x) { 
        return exp(-x*x) * sin(1000*x) + 1e-10 * cos(x); 
    };
    
    auto result = integrator.integrate(f, 0, 10, 1e-8);
    std::cout << "Integral result: " << result << "\n";
}
```

---

## Advanced Composition Patterns

### Combining Transforms for Maximum Benefit

```cpp
// Example 1: Stable Bayesian inference with extreme probabilities
using LogOdds = cbt::composed<cbt::lg, cbt::odds_ratio>;

class RobustBayesianNetwork {
private:
    std::map<std::string, LogOdds<double>> beliefs;
    
public:
    void set_prior(const std::string& node, double probability) {
        beliefs[node] = LogOdds<double>::from_inner(
            cbt::odds_ratio<double>::from_probability(probability)
        );
    }
    
    void update(const std::string& node, double likelihood_ratio) {
        beliefs[node] = beliefs[node] * 
                       LogOdds<double>::from_inner(
                           cbt::odds_ratio<double>(likelihood_ratio)
                       );
    }
    
    double get_probability(const std::string& node) {
        return beliefs[node].inner().to_probability();
    }
};

// Example 2: Exact rational intervals for validated numerics
using RationalInterval = cbt::composed<cbt::interval, cbt::stern_brocot>;

class ValidatedComputation {
private:
    using RI = RationalInterval<int64_t>;
    
public:
    RI compute_pi_bounds(int precision) {
        // Use Machin's formula with interval arithmetic
        auto one = RI::from_inner(cbt::stern_brocot<int64_t>(1, 1));
        auto four = RI::from_inner(cbt::stern_brocot<int64_t>(4, 1));
        // ... implement Machin's formula
        return pi_bounds;
    }
};

// Example 3: Extreme-scale parallel computation
using ParallelMultiscale = cbt::composed<cbt::multiscale, 
                                         cbt::residue_number_system>;

class QuantumGravitySimulation {
    // Handles both quantum (10^-35 m) and cosmic (10^26 m) scales
    // while maintaining parallel computation capabilities
};
```

---

## Performance Optimization Guide

### Memory Layout Considerations

```cpp
// Optimize cache locality
struct ParticleSystem {
    // BAD: Array of structures
    struct Particle {
        cbt::multiscale<double> mass;
        cbt::multiscale<double> position[3];
        cbt::multiscale<double> velocity[3];
    };
    std::vector<Particle> particles;
    
    // GOOD: Structure of arrays
    struct Particles {
        std::vector<cbt::multiscale<double>> masses;
        std::vector<cbt::multiscale<double>> positions_x;
        std::vector<cbt::multiscale<double>> positions_y;
        std::vector<cbt::multiscale<double>> positions_z;
        // Better cache utilization
    };
};
```

### SIMD Optimization

```cpp
// Enable SIMD for RNS operations
template<typename T, int N>
void parallel_rns_multiply(const T* a, const T* b, T* result, size_t count) {
    #pragma omp simd
    for(size_t i = 0; i < count * N; ++i) {
        result[i] = (a[i] * b[i]) % moduli[i % N];
    }
}
```

### Compile-Time Optimization

```cpp
// Use constexpr for compile-time computation
template<int N>
constexpr auto factorial_rational() {
    cbt::stern_brocot<int> result(1, 1);
    for(int i = 2; i <= N; ++i) {
        result = result * cbt::stern_brocot<int>(i, 1);
    }
    return result;
}

constexpr auto fact_10 = factorial_rational<10>();  // Computed at compile time
```

---

## Debugging and Troubleshooting

### Common Issues and Solutions

1. **Numerical Instability**
   ```cpp
   // Problem: Loss of precision
   double result = lg_value.value();  // May overflow
   
   // Solution: Stay in transform domain
   double log_result = lg_value.log();
   ```

2. **Performance Degradation**
   ```cpp
   // Problem: Frequent domain switching
   for(auto& val : values) {
       val = cbt::lgd(val.value() * 2);  // Expensive!
   }
   
   // Solution: Batch operations
   auto factor = cbt::lgd(2.0);
   for(auto& val : values) {
       val = val * factor;  // Efficient!
   }
   ```

3. **Memory Overhead**
   ```cpp
   // Consider transform overhead
   sizeof(cbt::lgd) vs sizeof(double)
   sizeof(cbt::multiscale<double>) vs sizeof(double)
   
   // Choose transforms wisely based on memory constraints
   ```

---

## Best Practices Summary

1. **Choose the Right Transform**: Match transform properties to your problem
2. **Stay in Domain**: Minimize conversions between domains
3. **Compose Wisely**: Combine transforms for multiplicative benefits
4. **Profile Performance**: Measure actual speedup in your use case
5. **Handle Edge Cases**: Consider domain restrictions and overflow
6. **Document Trade-offs**: Make transform choices explicit in code
7. **Test Thoroughly**: Verify numerical accuracy and edge cases