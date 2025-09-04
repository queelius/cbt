/**
 * Stern-Brocot Transform - Exact Rational Arithmetic
 * 
 * Transform: ℝ → Tree position in Stern-Brocot tree
 * Representation: All positive rationals in lowest terms
 * 
 * Trade-off:
 *   Gain: Exact rational arithmetic, optimal approximations
 *   Loss: Irrational numbers require infinite representation
 * 
 * Applications:
 *   - Computer algebra systems
 *   - Music theory (frequency ratios)
 *   - Continued fractions
 *   - Farey sequences
 */

#pragma once
#include <numeric>
#include <vector>
#include <iostream>
#include <string>
#include <cmath>

namespace cbt {

template<typename T>
class stern_brocot {
    static_assert(std::is_integral_v<T>, "stern_brocot requires integral type");
    
private:
    T num_, den_;
    
    void reduce() {
        T g = std::gcd(num_, den_);
        if (g > 1) {
            num_ /= g;
            den_ /= g;
        }
        if (den_ < 0) {
            num_ = -num_;
            den_ = -den_;
        }
    }
    
public:
    // Constructors
    stern_brocot() : num_(0), den_(1) {}
    
    stern_brocot(T n, T d) : num_(n), den_(d) {
        if (den_ == 0) throw std::invalid_argument("Denominator cannot be zero");
        reduce();
    }
    
    explicit stern_brocot(T n) : num_(n), den_(1) {}
    
    // Getters
    T numerator() const { return num_; }
    T denominator() const { return den_; }
    
    double to_double() const {
        return static_cast<double>(num_) / den_;
    }
    
    // Arithmetic
    stern_brocot operator+(const stern_brocot& other) const {
        return stern_brocot(num_ * other.den_ + other.num_ * den_,
                           den_ * other.den_);
    }
    
    stern_brocot operator-(const stern_brocot& other) const {
        return stern_brocot(num_ * other.den_ - other.num_ * den_,
                           den_ * other.den_);
    }
    
    stern_brocot operator*(const stern_brocot& other) const {
        return stern_brocot(num_ * other.num_, den_ * other.den_);
    }
    
    stern_brocot operator/(const stern_brocot& other) const {
        if (other.num_ == 0) throw std::invalid_argument("Division by zero");
        return stern_brocot(num_ * other.den_, den_ * other.num_);
    }
    
    // Mediant (fundamental Stern-Brocot operation)
    stern_brocot mediant(const stern_brocot& other) const {
        return stern_brocot(num_ + other.num_, den_ + other.den_);
    }
    
    // Find best rational approximation with denominator ≤ max_den
    static stern_brocot approximate(double x, T max_den) {
        if (x < 0) {
            auto result = approximate(-x, max_den);
            return stern_brocot(-result.num_, result.den_);
        }
        
        // Simple brute force for small denominators
        T best_num = 0, best_den = 1;
        double best_error = std::abs(x);
        
        for (T den = 1; den <= max_den; ++den) {
            T num = static_cast<T>(x * den + 0.5);  // Round to nearest
            double error = std::abs(x - static_cast<double>(num) / den);
            
            if (error < best_error) {
                best_error = error;
                best_num = num;
                best_den = den;
            }
        }
        
        return stern_brocot(best_num, best_den);
    }
    
    // Continued fraction representation
    std::vector<T> to_continued_fraction() const {
        std::vector<T> cf;
        T n = num_, d = den_;
        
        while (d != 0) {
            T q = n / d;
            cf.push_back(q);
            T temp = d;
            d = n - q * d;
            n = temp;
        }
        
        return cf;
    }
    
    // Comparison
    bool operator==(const stern_brocot& other) const {
        return num_ * other.den_ == other.num_ * den_;
    }
    
    bool operator<(const stern_brocot& other) const {
        return num_ * other.den_ < other.num_ * den_;
    }
    
    // Output
    friend std::ostream& operator<<(std::ostream& os, const stern_brocot& r) {
        os << r.num_;
        if (r.den_ != 1) os << "/" << r.den_;
        return os;
    }
};

// Farey sequence generator
template<typename T>
class farey_sequence {
    T n_;
    
public:
    explicit farey_sequence(T n) : n_(n) {}
    
    std::vector<stern_brocot<T>> generate() const {
        std::vector<stern_brocot<T>> sequence;
        sequence.emplace_back(0, 1);
        sequence.emplace_back(1, n_);
        
        size_t i = 0;
        while (i < sequence.size() - 1) {
            T a = sequence[i].numerator();
            T b = sequence[i].denominator();
            T c = sequence[i + 1].numerator();
            T d = sequence[i + 1].denominator();
            
            if (b + d <= n_) {
                sequence.insert(sequence.begin() + i + 1, 
                               stern_brocot<T>(a + c, b + d));
            } else {
                i++;
            }
        }
        
        return sequence;
    }
};

} // namespace cbt