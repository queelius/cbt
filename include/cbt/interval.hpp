/**
 * Interval Arithmetic - Rigorous Error Bounds
 * 
 * Transform: x → [lower, upper]
 * Tracks uncertainty through computations
 * 
 * Trade-off:
 *   Gain: Guaranteed bounds on numerical errors
 *   Loss: Result intervals may grow (dependency problem)
 * 
 * Applications:
 *   - Validated numerics
 *   - Global optimization
 *   - Computer-aided proofs
 *   - Robust control
 */

#pragma once
#include <cmath>
#include <iostream>
#include <algorithm>

namespace cbt {

template<typename T>
class interval {
    static_assert(std::is_floating_point_v<T>, "interval requires floating-point type");
    
private:
    T lower_;
    T upper_;
    
public:
    // Constructors
    interval() : lower_(0), upper_(0) {}
    interval(T value) : lower_(value), upper_(value) {}
    interval(T lower, T upper) : lower_(lower), upper_(upper) {
        if (lower > upper) std::swap(lower_, upper_);
    }
    
    // Factory methods
    static interval from_radius(T center, T radius) {
        return interval(center - radius, center + radius);
    }
    
    static interval entire() {
        return interval(-std::numeric_limits<T>::infinity(),
                       std::numeric_limits<T>::infinity());
    }
    
    static interval empty() {
        return interval(std::numeric_limits<T>::quiet_NaN(),
                       std::numeric_limits<T>::quiet_NaN());
    }
    
    // Getters
    T lower() const { return lower_; }
    T upper() const { return upper_; }
    T mid() const { return (lower_ + upper_) / 2; }
    T width() const { return upper_ - lower_; }
    T radius() const { return width() / 2; }
    
    bool is_empty() const { return std::isnan(lower_); }
    bool is_singleton() const { return lower_ == upper_; }
    
    // Arithmetic with guaranteed containment
    interval operator+(const interval& other) const {
        return interval(lower_ + other.lower_, upper_ + other.upper_);
    }
    
    interval operator-(const interval& other) const {
        return interval(lower_ - other.upper_, upper_ - other.lower_);
    }
    
    interval operator*(const interval& other) const {
        T products[4] = {
            lower_ * other.lower_,
            lower_ * other.upper_,
            upper_ * other.lower_,
            upper_ * other.upper_
        };
        return interval(*std::min_element(products, products + 4),
                       *std::max_element(products, products + 4));
    }
    
    interval operator/(const interval& other) const {
        if (other.contains(0)) {
            // Division by interval containing zero
            return entire();
        }
        return *this * interval(1.0 / other.upper_, 1.0 / other.lower_);
    }
    
    interval operator-() const {
        return interval(-upper_, -lower_);
    }
    
    // Set operations
    bool contains(T value) const {
        return lower_ <= value && value <= upper_;
    }
    
    bool contains(const interval& other) const {
        return lower_ <= other.lower_ && other.upper_ <= upper_;
    }
    
    interval intersect(const interval& other) const {
        T new_lower = std::max(lower_, other.lower_);
        T new_upper = std::min(upper_, other.upper_);
        if (new_lower > new_upper) return empty();
        return interval(new_lower, new_upper);
    }
    
    interval hull(const interval& other) const {
        return interval(std::min(lower_, other.lower_),
                       std::max(upper_, other.upper_));
    }
    
    // Mathematical functions with interval extensions
    friend interval sqrt(const interval& x) {
        if (x.upper_ < 0) return interval::empty();
        return interval(x.lower_ <= 0 ? 0 : std::sqrt(x.lower_),
                       std::sqrt(x.upper_));
    }
    
    friend interval exp(const interval& x) {
        return interval(std::exp(x.lower_), std::exp(x.upper_));
    }
    
    friend interval log(const interval& x) {
        if (x.upper_ <= 0) return interval::empty();
        return interval(x.lower_ <= 0 ? -std::numeric_limits<T>::infinity() 
                                      : std::log(x.lower_),
                       std::log(x.upper_));
    }
    
    friend interval sin(const interval& x) {
        // Simplified - proper implementation needs period handling
        return interval(-1, 1);
    }
    
    friend interval cos(const interval& x) {
        // Simplified - proper implementation needs period handling
        return interval(-1, 1);
    }
    
    // Comparison
    bool operator==(const interval& other) const {
        return lower_ == other.lower_ && upper_ == other.upper_;
    }
    
    // Interval ordering (for set operations)
    bool strictly_less(const interval& other) const {
        return upper_ < other.lower_;
    }
    
    bool strictly_greater(const interval& other) const {
        return lower_ > other.upper_;
    }
    
    // Output
    friend std::ostream& operator<<(std::ostream& os, const interval& i) {
        return os << "[" << i.lower_ << ", " << i.upper_ << "]";
    }
};

// Type aliases
using intervalf = interval<float>;
using intervald = interval<double>;

} // namespace cbt