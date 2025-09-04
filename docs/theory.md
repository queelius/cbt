# CBT Theory Guide

## Mathematical Foundation

### Formal Definition

A **Computational Basis Transform (CBT)** is a quadruple (D, D', φ, Ω) where:

- **D = (S_D, O_D, P_D)**: Source domain with state space, operations, and predicates
- **D' = (S_D', O_D', P_D')**: Target domain
- **φ: D → D'**: Transform function
- **Ω = (Ω_+, Ω_-, Ω_c)**: Trade-off specification
  - Ω_+: Operations becoming more efficient
  - Ω_-: Operations becoming less efficient
  - Ω_c: Transformation cost

### Homomorphic Property

A CBT φ is homomorphic with respect to operation ω if:

```
φ(ω(x₁, ..., xₙ)) = ω'(φ(x₁), ..., φ(xₙ))
```

This property is what makes CBTs powerful - operations in the source domain map to (potentially simpler) operations in the target domain.

## The No Free Lunch Theorem

### Statement

> For any non-trivial CBT, if some operations become more efficient (Ω_+ ≠ ∅), then others must become less efficient (Ω_- ≠ ∅).

### Proof Sketch

1. Assume a CBT improves some operations without worsening any
2. This implies the target domain strictly dominates the source domain
3. By information-theoretic arguments, this violates conservation of computational complexity
4. Therefore, every gain must be balanced by a loss

### Implications

- No universal "best" representation exists
- CBT selection depends on the specific computation profile
- Understanding trade-offs is crucial for algorithm design

## CBT Categories

### 1. Algebraic CBTs

Transform algebraic structures while preserving certain operations.

**Example: Logarithmic Transform**
```
Domain: (ℝ⁺, ×, ÷)
Target: (ℝ, +, -)
Transform: x → log(x)
Homomorphism: log(a×b) = log(a) + log(b)
```

### 2. Geometric CBTs

Transform geometric representations.

**Example: Quaternions**
```
Domain: 3D rotations with Euler angles
Target: Unit quaternions
Transform: (roll, pitch, yaw) → q = w + xi + yj + zk
Benefit: No gimbal lock, smooth interpolation
```

### 3. Probabilistic CBTs

Transform probability representations.

**Example: Odds-Ratio**
```
Domain: Probabilities [0,1]
Target: Odds [0,∞]
Transform: p → p/(1-p)
Benefit: Bayesian updates become multiplication
```

### 4. Number-Theoretic CBTs

Transform number representations.

**Example: Residue Number System**
```
Domain: Integers mod M
Target: Tuple of residues (mod m₁, ..., mod mₖ)
Transform: n → (n mod m₁, ..., n mod mₖ)
Benefit: Parallel arithmetic without carries
```

## Composition Theory

### Composition Rules

For CBTs φ₁: D₁ → D₂ and φ₂: D₂ → D₃:

```
Ω_+(φ₂ ∘ φ₁) ⊇ Ω_+(φ₁) ∩ φ₁⁻¹(Ω_+(φ₂))
```

### Power of Composition

Composed CBTs can exhibit emergent properties:

```cpp
// multiscale<lg<T>> combines benefits:
// - lg: efficient multiplication
// - multiscale: huge dynamic range
// Together: extreme-scale multiplicative computations
```

## Inter-CBT Mappings

### Direct Transformations

CBTs can map directly to each other without going through the original domain:

```
lg<T> ──────> multiscale<T>
  ↑              ↓
  └──── T ───────┘
```

Direct path preserves information and avoids overflow.

### Information Preservation

Some mappings are lossless:
- lg ↔ tropical (both essentially log domains)
- quaternion ↔ rotation matrix (different representations of SO(3))

Others involve information loss:
- interval → real (loses uncertainty information)
- dual → real (loses derivative information)

## Design Patterns

### 1. Domain Selection Pattern

Choose CBT based on operation profile:

```cpp
template<typename T>
using ComputeDomain = std::conditional_t<
    needs_multiplication,
    lg<T>,
    std::conditional_t<
        needs_exact_rationals,
        stern_brocot<T>,
        T
    >
>;
```

### 2. Lazy Conversion Pattern

Stay in transformed domain as long as possible:

```cpp
lg<double> result = compute_in_log_domain();
// Only convert when absolutely necessary
if (need_real_value) {
    double real = result.value();
}
```

### 3. Multi-Path Pattern

Use different CBT paths for different operations:

```cpp
// Path 1: For multiplication
auto log_path = to_log(value);
// Path 2: For error tracking
auto interval_path = to_interval(value);
```

## Advanced Topics

### CBT Networks

CBTs form a directed graph where:
- Nodes are computational domains
- Edges are transformations
- Multiple paths exist with different trade-offs

### Optimal Path Finding

Given operation profile O and CBT network G, find path P that minimizes:

```
Cost(P) = Σ(transformation_cost) + Σ(operation_cost_in_domain)
```

### Automatic CBT Selection

Future work: Machine learning to predict optimal CBT based on:
- Operation histogram
- Data characteristics
- Performance requirements

## Examples

### Example 1: Stable Product Computation

```cpp
// Problem: Compute ∏pᵢ where each pᵢ ≈ 10⁻¹⁰

// Naive approach (fails):
double product = 1.0;
for(auto p : probabilities) {
    product *= p;  // Underflows to 0
}

// CBT approach (works):
lg<double> log_product = lg<double>(1.0);
for(auto p : probabilities) {
    log_product = log_product * lg<double>(p);
}
// Can stay in log domain for comparisons
if(log_product.log() > threshold) { /*...*/ }
```

### Example 2: Bayesian Network

```cpp
// Traditional: Expensive normalization
double posterior = prior * likelihood;
double normalizer = integrate(posterior);
posterior /= normalizer;

// CBT: No normalization needed
odds_ratio<double> posterior_odds = 
    prior_odds * likelihood_ratio;
// Convert only when needed
double posterior = posterior_odds.to_probability();
```

## Further Reading

- [No Free Lunch Theorems (Wolpert & Macready)](https://ti.arc.nasa.gov/m/profile/dhw/papers/78.pdf)
- [Category Theory for Computing](https://www.cambridge.org/core/books/basic-category-theory-for-computer-scientists/23B89475C43838DC6E5438AB89C17C50)
- [Tropical Mathematics](https://arxiv.org/abs/math/0408099)