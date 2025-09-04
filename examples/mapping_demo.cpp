/**
 * Inter-CBT Mapping Demonstration
 * 
 * Shows how CBTs can map directly to each other without going
 * through the original domain, preserving information and avoiding overflow.
 */

#include <iostream>
#include <iomanip>
#include <limits>
#include "../include/cbt/cbt.hpp"

using namespace cbt;
using namespace cbt::mappings;

void demo_lg_extended_range() {
    std::cout << "\n=== Logarithmic Extended Range ===\n";
    std::cout << "Working with values that would overflow in normal domain\n\n";
    
    // Create huge numbers in log domain
    lgd huge1 = lgd::from_log(1000);  // e^1000 - would overflow double!
    lgd huge2 = lgd::from_log(2000);  // e^2000 - way beyond double range
    
    std::cout << "huge1 log value: " << huge1.log() << "\n";
    std::cout << "huge2 log value: " << huge2.log() << "\n";
    
    // Can still do arithmetic!
    lgd product = huge1 * huge2;
    std::cout << "Product log value: " << product.log() << " (= 1000 + 2000)\n";
    
    lgd ratio = huge2 / huge1;
    std::cout << "Ratio log value: " << ratio.log() << " (= 2000 - 1000)\n";
    
    // Converting back would overflow
    std::cout << "\nAttempting to convert ratio back: " << ratio.value() << "\n";
    std::cout << "But product would overflow: ";
    if (product.log() > 700) {
        std::cout << "TOO LARGE (log > 700)\n";
    }
}

void demo_lg_to_multiscale() {
    std::cout << "\n=== Direct lg → multiscale Mapping ===\n";
    std::cout << "Preserves huge values that would overflow via normal domain\n\n";
    
    // Create a value that would overflow double
    lgd huge = lgd::from_log(800);  // e^800
    std::cout << "lg value with log = 800 (e^800)\n";
    
    // Direct mapping to multiscale preserves the value
    auto ms = lg_to_multiscale<double, 3>(huge);
    std::cout << "Mapped to multiscale: " << ms << "\n";
    
    // Map back
    lgd back = multiscale_to_lg(ms);
    std::cout << "Mapped back to lg, log value: " << back.log() << "\n";
    
    // Compare with tiny value
    lgd tiny = lgd::from_log(-800);  // e^(-800)
    auto ms_tiny = lg_to_multiscale<double, 3>(tiny);
    std::cout << "\nlg value with log = -800 (e^-800)\n";
    std::cout << "Mapped to multiscale: " << ms_tiny << "\n";
}

void demo_dual_to_interval() {
    std::cout << "\n=== Dual → Interval Mapping ===\n";
    std::cout << "Converting derivatives to uncertainty bounds\n\n";
    
    // Function: f(x) = x² at x = 3
    duald x = duald::variable(3);  // dx/dx = 1
    duald f = x * x;
    
    std::cout << "f(x) = x² at x = 3\n";
    std::cout << "Value: " << f.value() << ", Derivative: " << f.derivative() << "\n";
    
    // Convert to interval representing local behavior
    intervald bounds = dual_to_interval(f, 0.1);
    std::cout << "As interval with ε=0.1: " << bounds << "\n";
    
    // More complex function
    duald g = sin(x) * exp(x);
    std::cout << "\ng(x) = sin(x)·e^x at x = 3\n";
    std::cout << "Value: " << g.value() << ", Derivative: " << g.derivative() << "\n";
    
    intervald g_bounds = dual_to_interval(g, 0.01);
    std::cout << "As interval with ε=0.01: " << g_bounds << "\n";
}

void demo_tropical_correspondence() {
    std::cout << "\n=== lg ↔ Tropical Correspondence ===\n";
    std::cout << "Both work in logarithmic-like domains\n\n";
    
    lgd a(100);
    lgd b(200);
    
    // Convert to tropical
    auto t_a = lg_to_tropical_min(a);
    auto t_b = lg_to_tropical_min(b);
    
    std::cout << "lg(100) → tropical: " << t_a.value() << "\n";
    std::cout << "lg(200) → tropical: " << t_b.value() << "\n";
    
    // Tropical addition = minimum = exp(min(log(a), log(b)))
    auto t_sum = t_a + t_b;
    std::cout << "\nTropical sum (min): " << t_sum.value() << "\n";
    std::cout << "This corresponds to min(100, 200) = 100\n";
    
    // Tropical multiplication = addition
    auto t_prod = t_a * t_b;
    std::cout << "\nTropical product: " << t_prod.value() << "\n";
    std::cout << "This is log(100) + log(200) = log(20000)\n";
    
    // Convert back
    lgd back = tropical_min_to_lg(t_prod);
    std::cout << "Back to lg, value: " << back.value() << " (= 100 * 200)\n";
}

void demo_network_path() {
    std::cout << "\n=== CBT Network Paths ===\n";
    std::cout << "Different paths through CBT network have different properties\n\n";
    
    double start_value = 1000.0;
    std::cout << "Starting value: " << start_value << "\n\n";
    
    // Path 1: Direct to interval
    intervald i1(start_value);
    std::cout << "Direct → interval: " << i1 << "\n";
    
    // Path 2: Via dual to interval (adds derivative information)
    duald d = duald::variable(start_value);
    intervald i2 = dual_to_interval(d);
    std::cout << "Via dual → interval: " << i2 << "\n";
    
    // Path 3: Via lg to multiscale
    lgd lg_val(start_value);
    auto ms = lg_to_multiscale<double>(lg_val);
    intervald i3 = multiscale_to_interval(ms);
    std::cout << "Via lg → multiscale → interval: " << i3 << "\n";
    
    std::cout << "\nEach path adds different information:\n";
    std::cout << "- Direct: just the value\n";
    std::cout << "- Via dual: includes sensitivity\n";
    std::cout << "- Via lg→multiscale: includes scale precision\n";
}

void demo_information_preservation() {
    std::cout << "\n=== Information Preservation in Mappings ===\n";
    std::cout << "Some mappings preserve all information, others lose some\n\n";
    
    // Lossless: lg ↔ tropical
    lgd original(42.0);
    auto tropical = lg_to_tropical_min(original);
    lgd recovered = tropical_min_to_lg(tropical);
    std::cout << "lg → tropical → lg:\n";
    std::cout << "  Original: " << original.value() << "\n";
    std::cout << "  Recovered: " << recovered.value() << "\n";
    std::cout << "  Lossless: " << (original.log() == recovered.log() ? "YES" : "NO") << "\n\n";
    
    // Lossy: interval → real
    intervald interval(10, 20);
    duald dual_approx = interval_to_dual(interval);
    intervald back = dual_to_interval(dual_approx);
    std::cout << "interval → dual → interval:\n";
    std::cout << "  Original: " << interval << "\n";
    std::cout << "  Recovered: " << back << "\n";
    std::cout << "  Lossless: NO (uncertainty interpretation changes)\n";
}

int main() {
    std::cout << "========================================\n";
    std::cout << "     INTER-CBT MAPPING DEMONSTRATION\n";
    std::cout << "========================================\n";
    
    std::cout << std::scientific << std::setprecision(3);
    
    demo_lg_extended_range();
    demo_lg_to_multiscale();
    demo_dual_to_interval();
    demo_tropical_correspondence();
    demo_network_path();
    demo_information_preservation();
    
    std::cout << "\n========================================\n";
    std::cout << "Key Insights:\n";
    std::cout << "1. CBTs can map directly without going through normal domain\n";
    std::cout << "2. This preserves information and avoids overflow\n";
    std::cout << "3. Different paths through CBT network have different trade-offs\n";
    std::cout << "4. The 'normal' domain is just one node in the network\n";
    std::cout << "========================================\n";
    
    return 0;
}