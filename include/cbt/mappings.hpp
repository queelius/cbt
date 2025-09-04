/**
 * Inter-CBT Mappings - Direct Transformations Between Domains
 * 
 * Principle: CBTs can map directly to each other without going through
 * the original domain, preserving information and avoiding overflow.
 * 
 * This creates a network of transformations where each path has
 * different computational properties and trade-offs.
 * 
 * Key Insight: The "normal" computational basis is just one node
 * in a graph of possible representations. Direct edges between
 * CBTs can be more efficient and preserve more information.
 */

#pragma once
#include "logarithmic.hpp"
#include "multiscale.hpp"
#include "dual.hpp"
#include "interval.hpp"
#include "tropical.hpp"
#include <limits>
#include <cmath>

namespace cbt {
namespace mappings {

/**
 * Logarithmic ↔ Multiscale
 * 
 * These domains have natural correspondence:
 * - Both handle extreme ranges
 * - lg preserves ratios, multiscale preserves scales
 * - Direct mapping avoids overflow that would occur via normal domain
 */
template<typename T, int SCALE_FACTOR = 3>
multiscale<T, SCALE_FACTOR> lg_to_multiscale(const lg<T>& x) {
    T log_val = x.log();
    
    // Handle special cases
    if (std::isinf(log_val)) {
        if (log_val < 0) return multiscale<T, SCALE_FACTOR>(0);
        // Positive infinity in log domain
        return multiscale<T, SCALE_FACTOR>(std::numeric_limits<T>::max());
    }
    
    // Convert log value directly to mantissa and scale
    // log(x) = log(mantissa) + scale * log(SCALE)
    constexpr T LOG_SCALE = std::log(T(10)) * SCALE_FACTOR;
    int scale = static_cast<int>(log_val / LOG_SCALE);
    T mantissa_log = log_val - scale * LOG_SCALE;
    T mantissa = std::exp(mantissa_log);
    
    return multiscale<T, SCALE_FACTOR>(mantissa, scale);
}

template<typename T, int SCALE_FACTOR = 3>
lg<T> multiscale_to_lg(const multiscale<T, SCALE_FACTOR>& x) {
    // Direct conversion: log(mantissa * 10^(scale*SCALE_FACTOR))
    T log_val = std::log(x.mantissa()) + 
                x.scale_level() * SCALE_FACTOR * std::log(T(10));
    return lg<T>::from_log(log_val);
}

/**
 * Dual ↔ Interval
 * 
 * Dual numbers track derivatives (sensitivities)
 * Intervals can represent this as uncertainty bounds
 */
template<typename T>
interval<T> dual_to_interval(const dual<T>& x, T epsilon = 1e-6) {
    // Interpret derivative as local rate of change
    // Create interval based on linear approximation
    T value = x.value();
    T sensitivity = std::abs(x.derivative() * epsilon);
    return interval<T>(value - sensitivity, value + sensitivity);
}

template<typename T>
dual<T> interval_to_dual(const interval<T>& x) {
    // Use midpoint as value, width as uncertainty measure
    T value = x.mid();
    T derivative = x.width() / 2;  // Rough sensitivity estimate
    return dual<T>(value, derivative);
}

/**
 * Logarithmic ↔ Tropical
 * 
 * Natural correspondence: both work in log-like domains
 * lg: multiplicative group → additive group
 * tropical: (min,+) or (max,+) semiring
 */
template<typename T>
tropical_min<T> lg_to_tropical_min(const lg<T>& x) {
    return tropical_min<T>(x.log());
}

template<typename T>
lg<T> tropical_min_to_lg(const tropical_min<T>& x) {
    if (x.is_infinite()) {
        return lg<T>(0);  // Return lg of 0 (maps to -infinity)
    }
    return lg<T>::from_log(x.value());
}

/**
 * Multiscale ↔ Interval
 * 
 * Multiscale tracks scale levels
 * Can create intervals representing scale uncertainty
 */
template<typename T, int SCALE_FACTOR = 3>
interval<T> multiscale_to_interval(const multiscale<T, SCALE_FACTOR>& x) {
    T value = x.to_value();
    // Account for potential precision loss at scale boundaries
    T epsilon = value * std::numeric_limits<T>::epsilon() * 
                std::pow(10, std::abs(x.scale_level()));
    return interval<T>::from_radius(value, epsilon);
}

/**
 * Composed mappings for multi-hop transformations
 */
template<typename T>
class cbt_network {
    // This could be extended to a full graph-based transformation system
    // with optimal path finding between any two CBT domains
public:
    // Example: Transform through intermediate domain to preserve properties
    template<int SCALE_FACTOR = 3>
    static multiscale<T, SCALE_FACTOR> lg_to_multiscale_via_dual(const lg<T>& x) {
        // Create dual number with log derivative
        dual<T> d(x.value(), x.value());  // d/dx of e^x = e^x
        
        // Convert to interval for bounds checking
        interval<T> i = dual_to_interval(d);
        
        // Finally to multiscale
        return multiscale<T, SCALE_FACTOR>(i.mid());
    }
    
    // Optimal path finder between CBTs could be implemented here
    // using graph algorithms to minimize information loss
};

/**
 * Universal CBT Interface
 * 
 * Concept: All CBTs could implement a common interface for
 * inter-domain mappings, enabling automatic conversion networks
 */
template<typename From, typename To>
struct cbt_converter {
    // Specializations would implement specific conversions
    static To convert(const From& from);
};

// Example specialization
template<typename T>
struct cbt_converter<lg<T>, multiscale<T, 3>> {
    static multiscale<T, 3> convert(const lg<T>& from) {
        return lg_to_multiscale<T, 3>(from);
    }
};

/**
 * Information-Preserving Mappings
 * 
 * Some CBT pairs can map without any information loss:
 * - lg ↔ tropical (both are essentially log domains)
 * - modular with same modulus ↔ modular
 * - quaternion ↔ rotation matrix (different representations of SO(3))
 * 
 * Others have fundamental information barriers:
 * - interval → real (loss of uncertainty information)
 * - dual → real (loss of derivative information)
 * - multiscale → fixed-precision (potential overflow)
 */

} // namespace mappings
} // namespace cbt