/**
 * Dual Numbers - Automatic Differentiation
 * 
 * Transform: f(x) → (f(x), f'(x))
 * Representation: a + bε where ε² = 0
 * 
 * Trade-off:
 *   Gain: Exact derivatives computed alongside function values
 *   Loss: Doubled storage, only first derivatives
 * 
 * Applications:
 *   - Automatic differentiation
 *   - Optimization algorithms
 *   - Sensitivity analysis
 *   - Physics simulations
 */

#pragma once
#include <cmath>
#include <iostream>

namespace cbt {

template<typename T>
class dual {
    static_assert(std::is_floating_point_v<T>, "dual requires floating-point type");
    
private:
    T real_;      // Function value
    T dual_;      // Derivative
    
public:
    // Constructors
    dual() : real_(0), dual_(0) {}
    dual(T real) : real_(real), dual_(0) {}
    dual(T real, T dual) : real_(real), dual_(dual) {}
    
    // Factory for creating variables
    static dual variable(T value) {
        return dual(value, 1);  // df/dx = 1 for x itself
    }
    
    // Getters
    T value() const { return real_; }
    T derivative() const { return dual_; }
    
    // Arithmetic - Chain rule built in!
    dual operator+(const dual& other) const {
        return dual(real_ + other.real_, dual_ + other.dual_);
    }
    
    dual operator-(const dual& other) const {
        return dual(real_ - other.real_, dual_ - other.dual_);
    }
    
    dual operator*(const dual& other) const {
        // Product rule: (fg)' = f'g + fg'
        return dual(real_ * other.real_, 
                   dual_ * other.real_ + real_ * other.dual_);
    }
    
    dual operator/(const dual& other) const {
        // Quotient rule: (f/g)' = (f'g - fg')/g²
        T denom = other.real_ * other.real_;
        return dual(real_ / other.real_,
                   (dual_ * other.real_ - real_ * other.dual_) / denom);
    }
    
    // Unary functions
    dual operator-() const {
        return dual(-real_, -dual_);
    }
    
    // Mathematical functions with derivatives
    friend dual sin(const dual& x) {
        return dual(std::sin(x.real_), x.dual_ * std::cos(x.real_));
    }
    
    friend dual cos(const dual& x) {
        return dual(std::cos(x.real_), -x.dual_ * std::sin(x.real_));
    }
    
    friend dual exp(const dual& x) {
        T exp_val = std::exp(x.real_);
        return dual(exp_val, x.dual_ * exp_val);
    }
    
    friend dual log(const dual& x) {
        return dual(std::log(x.real_), x.dual_ / x.real_);
    }
    
    friend dual pow(const dual& x, T n) {
        return dual(std::pow(x.real_, n), 
                   n * std::pow(x.real_, n - 1) * x.dual_);
    }
    
    friend dual sqrt(const dual& x) {
        T sqrt_val = std::sqrt(x.real_);
        return dual(sqrt_val, x.dual_ / (2 * sqrt_val));
    }
    
    // Comparison (based on real part)
    bool operator==(const dual& other) const {
        return real_ == other.real_;
    }
    
    bool operator<(const dual& other) const {
        return real_ < other.real_;
    }
    
    // Output
    friend std::ostream& operator<<(std::ostream& os, const dual& d) {
        os << d.real_;
        if (d.dual_ >= 0) os << "+";
        os << d.dual_ << "ε";
        return os;
    }
};

// Type alias
using dualf = dual<float>;
using duald = dual<double>;

} // namespace cbt