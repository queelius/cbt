/**
 * Composed Transforms - Multiplicative Power of CBTs
 * 
 * Principle: CBTs can be composed to combine their strengths
 * Examples:
 *   - multiscale<lg<T>>: Handle extreme scales with multiplication efficiency
 *   - lg<odds_ratio<T>>: Log-odds for numerical stability in Bayesian inference
 *   - rns<stern_brocot<T>>: Parallel exact rational arithmetic
 */

#pragma once
#include "logarithmic.hpp"
#include "multiscale.hpp"
#include "odds_ratio.hpp"

namespace cbt {

// Example: Multiscale logarithmic for physics simulations
// Handles both extreme scales (quantum to cosmic) and efficient multiplication
template<typename T>
class multiscale_log {
    multiscale<lg<T>> value_;
    
public:
    multiscale_log() = default;
    
    explicit multiscale_log(T val) : value_(lg<T>(val)) {}
    
    // Multiplication is highly efficient
    multiscale_log operator*(const multiscale_log& other) const {
        multiscale_log result;
        result.value_ = value_ * other.value_;
        return result;
    }
    
    T to_value() const {
        return value_.to_value().value();
    }
};

// Example: Bayesian inference across scales
template<typename T>
class bayesian_diagnostic {
private:
    log_odds<T> prior_;
    std::vector<std::pair<T, T>> tests_;  // (sensitivity, specificity)
    
public:
    explicit bayesian_diagnostic(T prior_probability)
        : prior_(log_odds<T>::from_probability(prior_probability)) {}
    
    void add_test(T sensitivity, T specificity) {
        tests_.emplace_back(sensitivity, specificity);
    }
    
    // Update with test results
    odds_ratio<T> update(const std::vector<bool>& results) const {
        if (results.size() != tests_.size()) {
            throw std::invalid_argument("Results must match number of tests");
        }
        
        T log_posterior = prior_.value();
        
        for (size_t i = 0; i < results.size(); ++i) {
            T likelihood_ratio;
            if (results[i]) {  // Positive test
                likelihood_ratio = tests_[i].first / (1 - tests_[i].second);
            } else {  // Negative test
                likelihood_ratio = (1 - tests_[i].first) / tests_[i].second;
            }
            log_posterior += std::log(likelihood_ratio);
        }
        
        return odds_ratio<T>::from_probability(
            1.0 / (1.0 + std::exp(-log_posterior)));
    }
};

} // namespace cbt