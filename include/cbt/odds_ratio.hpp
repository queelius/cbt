/**
 * Odds-Ratio Transform - Bayesian Computation Without Normalization
 * 
 * Transform: p → p/(1-p)
 * Homomorphism: Bayesian update → multiplication
 * 
 * Trade-off:
 *   Gain: No normalization needed for Bayesian inference
 *   Loss: Cannot directly add probabilities
 * 
 * Applications:
 *   - Medical diagnosis
 *   - Spam filtering  
 *   - Machine learning (logistic regression)
 *   - Information retrieval
 */

#pragma once
#include <cmath>
#include <iostream>
#include <limits>

namespace cbt {

template<typename T>
class odds_ratio {
    static_assert(std::is_floating_point_v<T>, "odds_ratio requires floating-point type");
    
private:
    T odds_;
    
public:
    // Constructors
    constexpr odds_ratio() : odds_(1) {}
    
    explicit constexpr odds_ratio(T odds) : odds_(odds) {}
    
    // Factory from probability
    static constexpr odds_ratio from_probability(T prob) {
        if (prob <= 0) return odds_ratio(0);
        if (prob >= 1) return odds_ratio(std::numeric_limits<T>::infinity());
        return odds_ratio(prob / (1 - prob));
    }
    
    // Conversion back to probability
    constexpr T to_probability() const {
        if (std::isinf(odds_)) return 1;
        return odds_ / (1 + odds_);
    }
    
    constexpr T value() const { return odds_; }
    
    // Bayesian update (multiplication of odds ratios)
    constexpr odds_ratio operator*(const odds_ratio& likelihood_ratio) const {
        return odds_ratio(odds_ * likelihood_ratio.odds_);
    }
    
    constexpr odds_ratio operator/(const odds_ratio& other) const {
        return odds_ratio(odds_ / other.odds_);
    }
    
    // Comparison operators (full set for consistency)
    constexpr bool operator==(const odds_ratio& other) const {
        return odds_ == other.odds_;
    }
    
    constexpr bool operator!=(const odds_ratio& other) const {
        return !(*this == other);
    }
    
    constexpr bool operator<(const odds_ratio& other) const {
        return odds_ < other.odds_;
    }
    
    constexpr bool operator<=(const odds_ratio& other) const {
        return odds_ <= other.odds_;
    }
    
    constexpr bool operator>(const odds_ratio& other) const {
        return odds_ > other.odds_;
    }
    
    constexpr bool operator>=(const odds_ratio& other) const {
        return odds_ >= other.odds_;
    }
    
    // Output
    friend std::ostream& operator<<(std::ostream& os, const odds_ratio& o) {
        return os << o.odds_ << ":1";
    }
};

// Log-odds for numerical stability with extreme probabilities
template<typename T>
class log_odds {
    static_assert(std::is_floating_point_v<T>, "log_odds requires floating-point type");
    
private:
    T log_odds_;
    
public:
    constexpr log_odds() : log_odds_(0) {}
    
    explicit constexpr log_odds(T log_val) : log_odds_(log_val) {}
    
    static constexpr log_odds from_probability(T prob) {
        if (prob <= 0) return log_odds(-std::numeric_limits<T>::infinity());
        if (prob >= 1) return log_odds(std::numeric_limits<T>::infinity());
        return log_odds(std::log(prob / (1 - prob)));
    }
    
    static constexpr log_odds from_odds(T odds) {
        return log_odds(std::log(odds));
    }
    
    constexpr T to_probability() const {
        T exp_val = std::exp(log_odds_);
        return exp_val / (1 + exp_val);
    }
    
    constexpr T value() const { return log_odds_; }
    
    // Bayesian update (addition in log space)
    constexpr log_odds operator+(const log_odds& log_lr) const {
        return log_odds(log_odds_ + log_lr.log_odds_);
    }
    
    constexpr log_odds operator-(const log_odds& other) const {
        return log_odds(log_odds_ - other.log_odds_);
    }
    
    friend std::ostream& operator<<(std::ostream& os, const log_odds& lo) {
        return os << "log_odds(" << lo.log_odds_ << ")";
    }
};

} // namespace cbt