/**
 * Basic tests for CBT implementations
 */

#include <iostream>
#include <cassert>
#include <cmath>
#include "../include/cbt/cbt.hpp"

using namespace cbt;

void test_logarithmic() {
    std::cout << "Testing logarithmic transform... ";
    
    lgd a(2.0);
    lgd b(3.0);
    
    auto product = a * b;
    assert(std::abs(product.value() - 6.0) < 1e-10);
    
    auto squared = a.pow(2);
    assert(std::abs(squared.value() - 4.0) < 1e-10);
    
    auto sqrt_val = lgd(16.0).sqrt();
    assert(std::abs(sqrt_val.value() - 4.0) < 1e-10);
    
    std::cout << "PASSED\n";
}

void test_odds_ratio() {
    std::cout << "Testing odds-ratio transform... ";
    
    auto odds = odds_ratio<double>::from_probability(0.5);
    assert(std::abs(odds.value() - 1.0) < 1e-10);
    
    auto odds2 = odds_ratio<double>::from_probability(0.75);
    assert(std::abs(odds2.value() - 3.0) < 1e-10);
    
    // Test Bayesian update
    auto prior = odds_ratio<double>::from_probability(0.1);
    auto lr = odds_ratio<double>(2.0);  // Likelihood ratio
    auto posterior = prior * lr;
    double expected_prob = 0.2 / (0.2 + 0.9);  // 2/11
    assert(std::abs(posterior.to_probability() - expected_prob) < 1e-10);
    
    std::cout << "PASSED\n";
}

void test_stern_brocot() {
    std::cout << "Testing Stern-Brocot transform... ";
    
    stern_brocot<int> a(1, 2);
    stern_brocot<int> b(1, 3);
    
    auto sum = a + b;
    assert(sum.numerator() == 5 && sum.denominator() == 6);
    
    auto product = a * b;
    assert(product.numerator() == 1 && product.denominator() == 6);
    
    // Test approximation
    auto pi_approx = stern_brocot<int>::approximate(3.14159, 100);
    // Should get a good approximation
    assert(pi_approx.denominator() <= 100);
    assert(std::abs(pi_approx.to_double() - 3.14159) < 0.001);
    
    std::cout << "PASSED\n";
}

void test_rns() {
    std::cout << "Testing residue number system... ";
    
    using RNS3 = residue_number_system<int32_t, 3>;
    
    auto rns_5 = RNS3::from_integer(5);
    auto rns_7 = RNS3::from_integer(7);
    
    auto sum = rns_5 + rns_7;
    assert(sum.to_integer() == 12);
    
    auto product = rns_5 * rns_7;
    assert(product.to_integer() == 35);
    
    auto diff = rns_7 - rns_5;
    assert(diff.to_integer() == 2);
    
    std::cout << "PASSED\n";
}

void test_multiscale() {
    std::cout << "Testing multiscale transform... ";
    
    multiscale<double, 3> a(1e10);
    multiscale<double, 3> b(1e-10);
    
    auto product = a * b;
    assert(std::abs(product.to_value() - 1.0) < 1e-10);
    
    multiscale<double, 3> c(5e15);
    multiscale<double, 3> d(2e15);
    auto sum = c + d;
    assert(std::abs(sum.to_value() - 7e15) < 1e10);  // Some precision loss expected
    
    std::cout << "PASSED\n";
}

void test_composed() {
    std::cout << "Testing composed transforms... ";
    
    // Test Bayesian diagnostic
    bayesian_diagnostic<double> diagnostic(0.01);
    diagnostic.add_test(0.95, 0.90);
    
    std::vector<bool> positive_result = {true};
    auto posterior = diagnostic.update(positive_result);
    
    // With 1% prior and positive test (sens=0.95, spec=0.90)
    // LR = 0.95/0.10 = 9.5
    // Posterior odds = 0.01/0.99 * 9.5 ≈ 0.096
    // Posterior prob ≈ 0.087
    assert(posterior.to_probability() > 0.08 && posterior.to_probability() < 0.10);
    
    std::cout << "PASSED\n";
}

int main() {
    std::cout << "Running CBT tests...\n\n";
    
    test_logarithmic();
    test_odds_ratio();
    test_stern_brocot();
    test_rns();
    test_multiscale();
    test_composed();
    
    std::cout << "\nAll tests passed!\n";
    return 0;
}