/**
 * Residue Number System (RNS) - Parallel Arithmetic Without Carries
 * 
 * Transform: n → (n mod p₁, n mod p₂, ..., n mod pₖ)
 * where p₁, p₂, ..., pₖ are coprime moduli
 * 
 * Trade-off:
 *   Gain: Fully parallel addition/multiplication, no carry propagation
 *   Loss: Comparison and division become complex operations
 * 
 * Applications:
 *   - Cryptography (RSA, ECC)
 *   - Digital signal processing
 *   - Fault-tolerant computing
 *   - High-performance computing
 */

#pragma once
#include <array>
#include <numeric>
#include <iostream>
#include <optional>

namespace cbt {

template<typename T, size_t N>
class residue_number_system {
    static_assert(std::is_integral_v<T>, "RNS requires integral type");
    
private:
    std::array<T, N> residues_;
    std::array<T, N> moduli_;
    T dynamic_range_;
    
    // Extended Euclidean algorithm for modular inverse
    static T extended_gcd(T a, T b, T& x, T& y) {
        if (b == 0) {
            x = 1;
            y = 0;
            return a;
        }
        T x1, y1;
        T gcd = extended_gcd(b, a % b, x1, y1);
        x = y1;
        y = x1 - (a / b) * y1;
        return gcd;
    }
    
    static T mod_inverse(T a, T m) {
        T x, y;
        T g = extended_gcd(a, m, x, y);
        if (g != 1) throw std::runtime_error("Modular inverse does not exist");
        return (x % m + m) % m;
    }
    
public:
    // Constructor
    residue_number_system(const std::array<T, N>& moduli) : moduli_(moduli) {
        // Verify moduli are coprime
        for (size_t i = 0; i < N; ++i) {
            for (size_t j = i + 1; j < N; ++j) {
                if (std::gcd(moduli_[i], moduli_[j]) != 1) {
                    throw std::invalid_argument("Moduli must be coprime");
                }
            }
        }
        
        dynamic_range_ = 1;
        for (T m : moduli_) {
            dynamic_range_ *= m;
        }
        
        residues_.fill(0);
    }
    
    // Default moduli for common use cases
    static std::array<T, N> default_moduli() {
        if constexpr (N == 3) {
            return {251, 253, 255};
        } else if constexpr (N == 4) {
            return {251, 253, 255, 256};
        } else {
            std::array<T, N> result;
            T current = 2;
            for (size_t i = 0; i < N; ++i) {
                result[i] = current++;
                while (i > 0) {
                    bool coprime = true;
                    for (size_t j = 0; j < i; ++j) {
                        if (std::gcd(current, result[j]) != 1) {
                            coprime = false;
                            break;
                        }
                    }
                    if (coprime) break;
                    current++;
                }
            }
            return result;
        }
    }
    
    residue_number_system() : residue_number_system(default_moduli()) {}
    
    // Convert from integer
    static residue_number_system from_integer(T value, 
                                              const std::array<T, N>& moduli = default_moduli()) {
        residue_number_system result(moduli);
        for (size_t i = 0; i < N; ++i) {
            result.residues_[i] = ((value % moduli[i]) + moduli[i]) % moduli[i];
        }
        return result;
    }
    
    // Convert to integer using Chinese Remainder Theorem
    T to_integer() const {
        T result = 0;
        
        for (size_t i = 0; i < N; ++i) {
            T Mi = dynamic_range_ / moduli_[i];
            T Mi_inv = mod_inverse(Mi % moduli_[i], moduli_[i]);
            result = (result + residues_[i] * Mi * Mi_inv) % dynamic_range_;
        }
        
        return result;
    }
    
    // Parallel arithmetic operations
    residue_number_system operator+(const residue_number_system& other) const {
        residue_number_system result(moduli_);
        for (size_t i = 0; i < N; ++i) {
            result.residues_[i] = (residues_[i] + other.residues_[i]) % moduli_[i];
        }
        return result;
    }
    
    residue_number_system operator-(const residue_number_system& other) const {
        residue_number_system result(moduli_);
        for (size_t i = 0; i < N; ++i) {
            result.residues_[i] = ((residues_[i] - other.residues_[i]) % moduli_[i] + moduli_[i]) % moduli_[i];
        }
        return result;
    }
    
    residue_number_system operator*(const residue_number_system& other) const {
        residue_number_system result(moduli_);
        for (size_t i = 0; i < N; ++i) {
            result.residues_[i] = (residues_[i] * other.residues_[i]) % moduli_[i];
        }
        return result;
    }
    
    // Getters
    const std::array<T, N>& residues() const { return residues_; }
    const std::array<T, N>& moduli() const { return moduli_; }
    T dynamic_range() const { return dynamic_range_; }
    
    // Comparison (expensive - requires conversion)
    bool operator==(const residue_number_system& other) const {
        return residues_ == other.residues_;
    }
    
    // Output
    friend std::ostream& operator<<(std::ostream& os, const residue_number_system& rns) {
        os << "RNS(";
        for (size_t i = 0; i < N; ++i) {
            os << rns.residues_[i] << " mod " << rns.moduli_[i];
            if (i < N - 1) os << ", ";
        }
        os << ")";
        return os;
    }
};

// Common type aliases
template<typename T>
using rns3 = residue_number_system<T, 3>;

template<typename T>
using rns4 = residue_number_system<T, 4>;

} // namespace cbt