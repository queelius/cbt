/**
 * Multiscale Transform - Automatic Scale Management
 * 
 * Transform: Value → (mantissa, scale_level)
 * Manages values across many orders of magnitude
 * 
 * Trade-off:
 *   Gain: Handle 200+ orders of magnitude without overflow
 *   Loss: Some precision loss at scale boundaries
 * 
 * Applications:
 *   - Physics simulations (quantum to cosmic scales)
 *   - Financial modeling (micropayments to national debt)
 *   - Scientific computing
 *   - Computer graphics (LOD systems)
 */

#pragma once
#include <cmath>
#include <iostream>
#include <limits>

namespace cbt {

template<typename T, int SCALE_FACTOR = 3>
class multiscale {
    static_assert(std::is_floating_point_v<T>, "multiscale requires floating-point type");
    static_assert(SCALE_FACTOR > 0, "SCALE_FACTOR must be positive");
    
private:
    T mantissa_;
    int8_t scale_level_;
    
    static constexpr T SCALE = std::pow(10, SCALE_FACTOR);
    static constexpr T INV_SCALE = 1.0 / SCALE;
    
    void normalize() {
        if (mantissa_ == 0) {
            scale_level_ = 0;
            return;
        }
        
        while (std::abs(mantissa_) >= 1 && scale_level_ < 127) {
            mantissa_ *= INV_SCALE;
            scale_level_++;
        }
        
        while (std::abs(mantissa_) < INV_SCALE && scale_level_ > -128) {
            mantissa_ *= SCALE;
            scale_level_--;
        }
    }
    
public:
    // Constructors
    multiscale() : mantissa_(0), scale_level_(0) {}
    
    explicit multiscale(T value) : mantissa_(value), scale_level_(0) {
        normalize();
    }
    
    multiscale(T mantissa, int8_t scale) : mantissa_(mantissa), scale_level_(scale) {
        normalize();
    }
    
    // Conversion
    T to_value() const {
        return mantissa_ * std::pow(SCALE, scale_level_);
    }
    
    // Getters
    T mantissa() const { return mantissa_; }
    int8_t scale_level() const { return scale_level_; }
    
    // Arithmetic
    multiscale operator+(const multiscale& other) const {
        if (mantissa_ == 0) return other;
        if (other.mantissa_ == 0) return *this;
        
        int8_t scale_diff = scale_level_ - other.scale_level_;
        
        if (scale_diff == 0) {
            return multiscale(mantissa_ + other.mantissa_, scale_level_);
        } else if (scale_diff > 0) {
            T scaled_other = other.mantissa_ * std::pow(INV_SCALE, scale_diff);
            return multiscale(mantissa_ + scaled_other, scale_level_);
        } else {
            T scaled_this = mantissa_ * std::pow(INV_SCALE, -scale_diff);
            return multiscale(scaled_this + other.mantissa_, other.scale_level_);
        }
    }
    
    multiscale operator*(const multiscale& other) const {
        return multiscale(mantissa_ * other.mantissa_, 
                         scale_level_ + other.scale_level_);
    }
    
    multiscale operator/(const multiscale& other) const {
        if (other.mantissa_ == 0) {
            throw std::runtime_error("Division by zero");
        }
        return multiscale(mantissa_ / other.mantissa_,
                         scale_level_ - other.scale_level_);
    }
    
    // Comparison
    bool operator==(const multiscale& other) const {
        if (mantissa_ == 0 && other.mantissa_ == 0) return true;
        return mantissa_ == other.mantissa_ && scale_level_ == other.scale_level_;
    }
    
    bool operator<(const multiscale& other) const {
        if (scale_level_ != other.scale_level_) {
            return scale_level_ < other.scale_level_;
        }
        return mantissa_ < other.mantissa_;
    }
    
    // Output
    friend std::ostream& operator<<(std::ostream& os, const multiscale& ms) {
        return os << ms.mantissa_ << "e" << (ms.scale_level_ * SCALE_FACTOR);
    }
};

} // namespace cbt