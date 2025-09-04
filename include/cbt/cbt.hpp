/**
 * Computational Basis Transforms (CBT)
 * 
 * A unified framework for understanding transformations that trade
 * computational complexity in one domain for simplicity in another.
 * 
 * Core Principle:
 * A CBT is a quadruple (D, D', φ, Ω) where:
 *   - D, D' are computational domains
 *   - φ: D → D' is the transform
 *   - Ω captures the trade-offs
 * 
 * Key Insight:
 * No universal "best" representation exists - only fitness for purpose.
 * Every transform involves fundamental trade-offs.
 */

#pragma once

// Core transforms
#include "cbt/logarithmic.hpp"
#include "cbt/odds_ratio.hpp"
#include "cbt/stern_brocot.hpp"
#include "cbt/residue_number_system.hpp"
#include "cbt/multiscale.hpp"

// Additional transforms
#include "cbt/dual.hpp"
#include "cbt/interval.hpp"
#include "cbt/tropical.hpp"
#include "cbt/modular.hpp"
#include "cbt/quaternion.hpp"

// Composed transforms
#include "cbt/composed.hpp"

// Inter-CBT mappings
#include "cbt/mappings.hpp"

namespace cbt {

/**
 * CBT Selection Guide
 * 
 * Use logarithmic when:
 *   - Multiplication is the dominant operation
 *   - Working with products of many terms
 *   - Need to handle very large/small positive numbers
 * 
 * Use odds_ratio when:
 *   - Performing Bayesian inference
 *   - Sequential probability updates
 *   - Working with likelihood ratios
 * 
 * Use stern_brocot when:
 *   - Need exact rational arithmetic
 *   - Finding optimal rational approximations
 *   - Working with musical intervals or frequencies
 * 
 * Use residue_number_system when:
 *   - Need parallel arithmetic without carries
 *   - Implementing cryptographic operations
 *   - Working on parallel hardware (GPU/FPGA)
 * 
 * Use multiscale when:
 *   - Values span many orders of magnitude
 *   - Automatic scale management needed
 *   - Physics simulations across scales
 * 
 * Use dual when:
 *   - Need automatic differentiation
 *   - Optimization algorithms
 *   - Sensitivity analysis
 * 
 * Use interval when:
 *   - Need rigorous error bounds
 *   - Validated numerics
 *   - Computer-aided proofs
 * 
 * Use tropical when:
 *   - Shortest path problems
 *   - Task scheduling
 *   - Discrete optimization
 * 
 * Use modular when:
 *   - Cryptographic operations
 *   - Hash functions
 *   - Cyclic computations
 * 
 * Use quaternion when:
 *   - 3D rotations needed
 *   - Avoiding gimbal lock
 *   - Smooth interpolation
 * 
 * Compose transforms when:
 *   - Need benefits of multiple transforms
 *   - Example: multiscale<lg<T>> for extreme-scale multiplication
 */

// Version information
constexpr const char* CBT_VERSION = "1.0.0";

} // namespace cbt