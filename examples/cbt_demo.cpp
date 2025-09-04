/**
 * CBT Demonstration - Showcasing Computational Basis Transforms
 */

#include <iostream>
#include <iomanip>
#include <vector>
#include <chrono>
#include "../include/cbt/cbt.hpp"

using namespace cbt;

void demo_logarithmic() {
    std::cout << "\n=== Logarithmic Transform ===\n";
    std::cout << "Transform: multiplication → addition\n\n";
    
    lgd a(1000.0);
    lgd b(2000.0);
    lgd c(3000.0);
    
    // Traditional: 1000 * 2000 * 3000
    auto product = a * b * c;
    std::cout << "1000 * 2000 * 3000 = " << product.value() << "\n";
    
    // Show the internal representation
    std::cout << "Internal: " << a.log() << " + " << b.log() << " + " << c.log()
              << " = " << product.log() << "\n";
    
    // Power operation is just multiplication
    auto squared = a.pow(2);
    std::cout << "1000² = " << squared.value() << "\n";
}

void demo_odds_ratio() {
    std::cout << "\n=== Odds-Ratio Transform ===\n";
    std::cout << "Transform: Bayesian update → multiplication\n\n";
    
    // Disease with 1% prevalence
    double prior_prob = 0.01;
    auto prior_odds = odds_ratio<double>::from_probability(prior_prob);
    
    std::cout << "Prior probability: " << prior_prob << " → odds " << prior_odds << "\n";
    
    // Test with 90% sensitivity, 95% specificity
    double sensitivity = 0.90;
    double specificity = 0.95;
    double likelihood_ratio = sensitivity / (1 - specificity);
    
    // Positive test result
    auto posterior_odds = prior_odds * odds_ratio<double>(likelihood_ratio);
    std::cout << "After positive test (LR=" << likelihood_ratio << "): "
              << posterior_odds << "\n";
    std::cout << "Posterior probability: " << posterior_odds.to_probability() << "\n";
}

void demo_stern_brocot() {
    std::cout << "\n=== Stern-Brocot Transform ===\n";
    std::cout << "Exact rational arithmetic & approximation\n\n";
    
    // Exact rational arithmetic
    stern_brocot<int> a(1, 3);
    stern_brocot<int> b(1, 7);
    
    auto sum = a + b;
    std::cout << a << " + " << b << " = " << sum << " (exact!)\n";
    
    // Approximate π
    double pi = 3.14159265358979;
    auto approx = stern_brocot<int>::approximate(pi, 100);
    std::cout << "\nπ ≈ " << approx << " = " << approx.to_double()
              << " (error: " << std::abs(pi - approx.to_double()) << ")\n";
    
    // Famous approximation
    auto pi_355_113 = stern_brocot<int>::approximate(pi, 1000);
    std::cout << "Better: π ≈ " << pi_355_113 << " = " << pi_355_113.to_double() << "\n";
}

void demo_rns() {
    std::cout << "\n=== Residue Number System ===\n";
    std::cout << "Parallel arithmetic without carry propagation\n\n";
    
    using RNS3 = residue_number_system<int32_t, 3>;
    
    int32_t a = 12345;
    int32_t b = 67890;
    
    auto rns_a = RNS3::from_integer(a);
    auto rns_b = RNS3::from_integer(b);
    
    std::cout << a << " → " << rns_a << "\n";
    std::cout << b << " → " << rns_b << "\n\n";
    
    // Addition (component-wise, fully parallel!)
    auto sum = rns_a + rns_b;
    std::cout << "Sum: " << sum << " = " << sum.to_integer() << "\n";
    
    // Multiplication (component-wise, no carries!)
    auto product = rns_a * rns_b;
    std::cout << "Product: " << product << " = " << product.to_integer() << "\n";
}

void demo_multiscale() {
    std::cout << "\n=== Multiscale Transform ===\n";
    std::cout << "Automatic scale management across orders of magnitude\n\n";
    
    multiscale<double, 3> planck_length(1.616e-35);
    multiscale<double, 3> observable_universe(8.8e26);
    
    std::cout << "Planck length: " << planck_length << " meters\n";
    std::cout << "Observable universe: " << observable_universe << " meters\n";
    
    auto ratio = observable_universe / planck_length;
    std::cout << "Ratio: " << ratio << " (" << ratio.to_value() << ")\n";
    
    // Can handle intermediate calculations without overflow
    auto squared = observable_universe * observable_universe;
    std::cout << "Universe²: " << squared << " m²\n";
}

void demo_composed() {
    std::cout << "\n=== Composed Transforms ===\n";
    std::cout << "Combining transforms for multiplicative power\n\n";
    
    // Bayesian diagnostic with multiple tests
    bayesian_diagnostic<double> diagnostic(0.01);  // 1% prior
    diagnostic.add_test(0.95, 0.90);  // Test 1
    diagnostic.add_test(0.85, 0.95);  // Test 2
    
    std::vector<bool> results = {true, true};  // Both positive
    auto posterior = diagnostic.update(results);
    
    std::cout << "Prior: 1%\n";
    std::cout << "After 2 positive tests: " << posterior.to_probability() * 100 << "%\n";
}

void demo_trade_offs() {
    std::cout << "\n=== CBT Trade-offs ===\n";
    std::cout << "Every transform has fundamental trade-offs\n\n";
    
    std::cout << "Transform       | Gain                | Loss\n";
    std::cout << "----------------|--------------------|-----------------\n";
    std::cout << "Logarithmic     | mult → add         | No direct addition\n";
    std::cout << "Odds-Ratio      | Bayes → mult       | Cannot add probs\n";
    std::cout << "Stern-Brocot    | Exact rationals    | Irrationals infinite\n";
    std::cout << "RNS             | Parallel ops       | Comparison complex\n";
    std::cout << "Multiscale      | Huge range         | Precision at boundaries\n";
}

int main() {
    std::cout << "========================================\n";
    std::cout << "   COMPUTATIONAL BASIS TRANSFORMS\n";
    std::cout << "   Version " << CBT_VERSION << "\n";
    std::cout << "========================================\n";
    
    demo_logarithmic();
    demo_odds_ratio();
    demo_stern_brocot();
    demo_rns();
    demo_multiscale();
    demo_composed();
    demo_trade_offs();
    
    std::cout << "\n========================================\n";
    std::cout << "Key Insight: No universal 'best' representation\n";
    std::cout << "Only fitness for purpose!\n";
    std::cout << "========================================\n";
    
    return 0;
}