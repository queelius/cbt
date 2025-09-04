# API Reference

## Core Namespace

All CBT functionality is contained within the `cbt` namespace.

```cpp
namespace cbt {
    // All transforms and utilities
}
```

## Transform Classes

### Base Transform Pattern

All transforms follow a consistent interface pattern:

```cpp
template<typename T>
class transform_name {
public:
    // Construction from original domain
    explicit transform_name(T value);
    
    // Factory methods for direct construction in transformed domain
    static transform_name from_transformed(/*...*/);
    
    // Convert back to original domain
    T value() const;
    
    // Arithmetic operations (transform-specific)
    transform_name operator*(const transform_name& other) const;
    transform_name operator+(const transform_name& other) const;
    // ... other operations
    
    // Comparison operators
    bool operator==(const transform_name& other) const;
    bool operator<(const transform_name& other) const;
    // ... other comparisons
};
```

---

## Logarithmic Transform

### Class: `cbt::lg<T>`

Transforms multiplication into addition by working in logarithmic space.

#### Template Parameters
- `T` - Underlying floating-point type (`float`, `double`, `long double`)

#### Type Aliases
- `lgf` - Single-precision logarithmic transform (`lg<float>`)
- `lgd` - Double-precision logarithmic transform (`lg<double>`)

#### Constructors

```cpp
constexpr lg()
```
Default constructor - represents zero (log of zero is -∞).

```cpp
explicit constexpr lg(T value)
```
Construct from a real value. Values ≤ 0 map to -∞.

#### Factory Methods

```cpp
static constexpr lg from_log(T log_val)
```
Create directly from log value. Useful for avoiding overflow.

**Example:**
```cpp
auto huge = cbt::lgd::from_log(800);  // Represents e^800
```

#### Member Functions

```cpp
constexpr T value() const
```
Convert back to real domain. May overflow for large log values.

```cpp
constexpr T log() const
```
Get the internal log representation. Safe for extreme values.

```cpp
constexpr lg operator*(const lg& other) const
```
Multiplication in original domain (addition in log domain).

```cpp
constexpr lg operator/(const lg& other) const
```
Division in original domain (subtraction in log domain).

```cpp
constexpr lg pow(T exponent) const
```
Raise to power (multiplication in log domain).

```cpp
constexpr lg sqrt() const
```
Square root (division by 2 in log domain).

---

## Odds-Ratio Transform

### Class: `cbt::odds_ratio<T>`

Transforms Bayesian updates into multiplication operations.

#### Template Parameters
- `T` - Underlying type (typically `float` or `double`)

#### Constructors

```cpp
explicit constexpr odds_ratio(T odds)
```
Construct from odds value.

#### Factory Methods

```cpp
static constexpr odds_ratio from_probability(T prob)
```
Create from probability value (0 < prob < 1).

```cpp
static constexpr odds_ratio from_log_odds(T log_odds)
```
Create from log-odds value.

#### Member Functions

```cpp
constexpr T value() const
```
Get the odds value.

```cpp
constexpr T to_probability() const
```
Convert to probability (odds / (1 + odds)).

```cpp
constexpr T to_log_odds() const
```
Convert to log-odds representation.

```cpp
constexpr odds_ratio operator*(const odds_ratio& other) const
```
Bayesian update (multiply odds ratios).

**Example:**
```cpp
auto prior = odds_ratio<double>::from_probability(0.01);
auto likelihood_ratio = odds_ratio<double>(18.0);
auto posterior = prior * likelihood_ratio;
```

---

## Stern-Brocot Transform

### Class: `cbt::stern_brocot<T>`

Exact rational arithmetic using the Stern-Brocot tree.

#### Template Parameters
- `T` - Integer type for numerator/denominator

#### Constructors

```cpp
explicit stern_brocot(T num, T den = 1)
```
Construct from numerator and denominator.

#### Factory Methods

```cpp
static stern_brocot approximate(double value, T max_den)
```
Find best rational approximation with denominator ≤ max_den.

**Example:**
```cpp
auto pi_approx = stern_brocot<int>::approximate(3.14159, 1000);
// Returns 355/113
```

#### Member Functions

```cpp
T numerator() const
T denominator() const
```
Access components.

```cpp
double to_double() const
```
Convert to floating-point.

```cpp
stern_brocot operator+(const stern_brocot& other) const
stern_brocot operator-(const stern_brocot& other) const
stern_brocot operator*(const stern_brocot& other) const
stern_brocot operator/(const stern_brocot& other) const
```
Exact rational arithmetic operations.

---

## Residue Number System

### Class: `cbt::residue_number_system<T, N>`

Parallel arithmetic without carry propagation.

#### Template Parameters
- `T` - Base integer type
- `N` - Number of moduli (determines parallelism level)

#### Type Aliases
- `rns3<T>` - 3-moduli RNS system
- `rns4<T>` - 4-moduli RNS system
- `rns5<T>` - 5-moduli RNS system

#### Factory Methods

```cpp
static residue_number_system from_integer(T value)
```
Convert integer to RNS representation.

#### Member Functions

```cpp
T to_integer() const
```
Convert back to integer (using Chinese Remainder Theorem).

```cpp
residue_number_system operator+(const residue_number_system& other) const
residue_number_system operator*(const residue_number_system& other) const
```
Component-wise parallel operations.

**Example:**
```cpp
auto a = rns3<int>::from_integer(12345);
auto b = rns3<int>::from_integer(67890);
auto sum = a + b;  // Parallel addition
```

---

## Multiscale Transform

### Class: `cbt::multiscale<T>`

Automatic scale management for extreme dynamic ranges.

#### Template Parameters
- `T` - Base type (typically `double`)

#### Constructors

```cpp
explicit multiscale(T value)
```
Construct with automatic scale selection.

#### Member Functions

```cpp
T mantissa() const
```
Get the mantissa (normalized value).

```cpp
int scale_level() const
```
Get the scale level (power of scale_base).

```cpp
T to_value() const
```
Convert to regular floating-point (may overflow/underflow).

```cpp
multiscale operator*(const multiscale& other) const
multiscale operator/(const multiscale& other) const
```
Operations with automatic scale adjustment.

---

## Additional Transforms

### Dual Numbers: `cbt::dual<T>`

Automatic differentiation via dual number arithmetic.

```cpp
dual<double> x(2.0, 1.0);  // value=2, derivative=1
auto y = x * x + dual<double>(3.0);
// y.value() = 7, y.derivative() = 4
```

### Interval Arithmetic: `cbt::interval<T>`

Rigorous error bounds and validated numerics.

```cpp
interval<double> x(1.0, 1.1);  // [1.0, 1.1]
interval<double> y(2.0, 2.1);  // [2.0, 2.1]
auto sum = x + y;  // [3.0, 3.2]
```

### Tropical Algebra: `cbt::tropical<T>`

Min-plus algebra for optimization problems.

```cpp
tropical<double> a(5.0);
tropical<double> b(3.0);
auto sum = a + b;  // min(5, 3) = 3
auto product = a * b;  // 5 + 3 = 8
```

### Modular Arithmetic: `cbt::modular<T, M>`

Cyclic arithmetic with compile-time modulus.

```cpp
modular<int, 7> a(10);  // 10 mod 7 = 3
modular<int, 7> b(5);
auto sum = a + b;  // (3 + 5) mod 7 = 1
```

### Quaternions: `cbt::quaternion<T>`

3D rotations without gimbal lock.

```cpp
quaternion<double> q(1, 0, 0, 0);  // Identity
quaternion<double> rot = quaternion<double>::from_axis_angle({0,0,1}, M_PI/2);
auto result = rot * q * rot.conjugate();  // Rotation
```

---

## Transform Composition

### Class Template: `cbt::composed<Outer, Inner>`

Compose two transforms for combined benefits.

```cpp
template<template<typename> class Outer, 
         template<typename> class Inner>
class composed;
```

#### Example Usage

```cpp
// Extreme scale multiplication
using extreme_mult = composed<multiscale, lg>;
extreme_mult value;

// Stable probabilistic inference
using log_odds = composed<lg, odds_ratio>;
log_odds posterior;
```

---

## Inter-CBT Mappings

### Namespace: `cbt::mappings`

Direct transformations between CBT domains.

#### Available Mappings

```cpp
multiscale<double> lg_to_multiscale(const lg<double>& value)
```
Convert from logarithmic to multiscale (preserves extreme values).

```cpp
lg<double> multiscale_to_lg(const multiscale<double>& value)
```
Convert from multiscale to logarithmic.

```cpp
lg<odds_ratio<double>> odds_to_log_odds(const odds_ratio<double>& value)
```
Convert odds-ratio to log-odds (numerical stability).

**Example:**
```cpp
lgd huge = lgd::from_log(800);
auto ms = mappings::lg_to_multiscale(huge);  // No overflow
```

---

## Utility Functions

### Transform Selection Helper

```cpp
template<typename T>
auto select_transform(TransformHint hint) -> /* appropriate transform */
```

Automatically selects the best transform based on usage hints.

### Performance Utilities

```cpp
template<typename Transform>
size_t memory_footprint();
```
Get memory usage of a transform instance.

```cpp
template<typename Transform>
constexpr bool is_parallelizable();
```
Check if transform supports parallel operations.

---

## Error Handling

All transforms use safe conversions and handle edge cases:

- **Logarithmic**: Negative values → -∞
- **Odds-Ratio**: Invalid probabilities → throws
- **Stern-Brocot**: Zero denominator → throws
- **RNS**: Out-of-range values → modular reduction
- **Multiscale**: Automatic scale adjustment

---

## Thread Safety

All transform classes are:
- **Immutable**: Operations return new instances
- **Thread-safe**: Can be shared between threads
- **Lock-free**: No synchronization needed

---

## Performance Characteristics

| Transform | Space | Multiplication | Addition | Comparison |
|-----------|-------|----------------|----------|------------|
| Logarithmic | O(1) | O(1) | N/A | O(1) |
| Odds-Ratio | O(1) | O(1) | Complex | O(1) |
| Stern-Brocot | O(1) | O(1) | O(1) | O(1) |
| RNS | O(N) | O(N) parallel | O(N) parallel | O(N²) |
| Multiscale | O(1) | O(1) | O(1) | O(1) |