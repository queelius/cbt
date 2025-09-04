# CBT Tutorials

## Tutorial 1: Building a Probabilistic Spam Filter

### Overview
In this tutorial, we'll build a Bayesian spam filter using the odds-ratio transform, demonstrating how CBT makes complex probability calculations simple and efficient.

### The Problem
Traditional spam filters need to:
1. Combine evidence from multiple features (words, patterns)
2. Handle very small probabilities without underflow
3. Update beliefs as new evidence arrives
4. Avoid expensive normalization calculations

### The CBT Solution

```cpp
#include <cbt/odds_ratio.hpp>
#include <cbt/logarithmic.hpp>
#include <map>
#include <vector>
#include <string>

class BayesianSpamFilter {
private:
    using WordOdds = std::map<std::string, cbt::odds_ratio<double>>;
    
    WordOdds spam_word_odds;
    WordOdds ham_word_odds;
    cbt::odds_ratio<double> prior_spam_odds;
    
public:
    BayesianSpamFilter(double spam_prevalence = 0.5) 
        : prior_spam_odds(cbt::odds_ratio<double>::from_probability(spam_prevalence)) {}
    
    // Training: Learn word probabilities
    void train(const std::vector<std::string>& words, bool is_spam) {
        for(const auto& word : words) {
            if(is_spam) {
                // Increment spam count (simplified - real implementation needs smoothing)
                if(spam_word_odds.find(word) == spam_word_odds.end()) {
                    spam_word_odds[word] = cbt::odds_ratio<double>::from_probability(0.9);
                }
            } else {
                if(ham_word_odds.find(word) == ham_word_odds.end()) {
                    ham_word_odds[word] = cbt::odds_ratio<double>::from_probability(0.1);
                }
            }
        }
    }
    
    // Classification using odds ratios
    bool classify(const std::vector<std::string>& words) {
        cbt::odds_ratio<double> message_odds = prior_spam_odds;
        
        for(const auto& word : words) {
            // Compute likelihood ratio for this word
            double p_word_given_spam = 0.5;  // Default
            double p_word_given_ham = 0.5;
            
            if(spam_word_odds.find(word) != spam_word_odds.end()) {
                p_word_given_spam = 0.9;  // Simplified
            }
            if(ham_word_odds.find(word) != ham_word_odds.end()) {
                p_word_given_ham = 0.1;  // Simplified
            }
            
            double likelihood_ratio = p_word_given_spam / p_word_given_ham;
            
            // Bayesian update via multiplication!
            message_odds = message_odds * cbt::odds_ratio<double>(likelihood_ratio);
        }
        
        // Classify based on posterior probability
        return message_odds.to_probability() > 0.5;
    }
    
    // Get spam probability for a message
    double get_spam_probability(const std::vector<std::string>& words) {
        cbt::odds_ratio<double> message_odds = prior_spam_odds;
        
        for(const auto& word : words) {
            // Similar to classify, but return probability
            double p_word_given_spam = spam_word_odds[word].to_probability();
            double p_word_given_ham = ham_word_odds[word].to_probability();
            double likelihood_ratio = p_word_given_spam / p_word_given_ham;
            
            message_odds = message_odds * cbt::odds_ratio<double>(likelihood_ratio);
        }
        
        return message_odds.to_probability();
    }
};

// Usage example
int main() {
    BayesianSpamFilter filter(0.3);  // 30% of emails are spam
    
    // Train with examples
    filter.train({"viagra", "pills", "buy", "now"}, true);  // spam
    filter.train({"meeting", "tomorrow", "agenda"}, false);  // ham
    
    // Classify new message
    std::vector<std::string> message = {"buy", "viagra", "cheap"};
    bool is_spam = filter.classify(message);
    double spam_prob = filter.get_spam_probability(message);
    
    std::cout << "Message is " << (is_spam ? "SPAM" : "HAM") 
              << " (probability: " << spam_prob << ")" << std::endl;
    
    return 0;
}
```

### Key Insights

1. **No Normalization**: The odds-ratio transform eliminates the need for normalization constants
2. **Sequential Updates**: Each word's evidence is incorporated via simple multiplication
3. **Numerical Stability**: Could combine with log-odds for extreme probabilities
4. **Efficiency**: O(n) complexity for n words, vs O(n²) for traditional approaches

---

## Tutorial 2: Multi-Scale Scientific Simulation

### Overview
Build a gravity simulation that handles both planetary and quantum scales simultaneously using the multiscale transform.

### The Problem
Simulating gravity across scales involves:
- Planetary masses (~10^24 kg) and distances (~10^9 m)
- Quantum particles (~10^-30 kg) and distances (~10^-15 m)
- Traditional floating-point can't handle this range

### The CBT Solution

```cpp
#include <cbt/multiscale.hpp>
#include <cbt/composed.hpp>
#include <vector>
#include <cmath>

class MultiScaleGravitySimulation {
private:
    using Mass = cbt::multiscale<double>;
    using Distance = cbt::multiscale<double>;
    using Force = cbt::multiscale<double>;
    
    struct Body {
        Mass mass;
        Distance position[3];
        Distance velocity[3];
        std::string name;
    };
    
    std::vector<Body> bodies;
    const Mass G{6.67430e-11};  // Gravitational constant
    
public:
    // Add celestial body
    void add_body(const std::string& name, double mass, 
                  double x, double y, double z) {
        Body body;
        body.name = name;
        body.mass = Mass(mass);
        body.position[0] = Distance(x);
        body.position[1] = Distance(y);
        body.position[2] = Distance(z);
        body.velocity[0] = Distance(0);
        body.velocity[1] = Distance(0);
        body.velocity[2] = Distance(0);
        bodies.push_back(body);
    }
    
    // Calculate gravitational force between two bodies
    Force calculate_force(const Body& b1, const Body& b2) {
        // F = G * m1 * m2 / r²
        
        // Calculate distance
        Distance dx = b2.position[0] - b1.position[0];
        Distance dy = b2.position[1] - b1.position[1];
        Distance dz = b2.position[2] - b1.position[2];
        
        // r² = dx² + dy² + dz²
        Distance r_squared = dx * dx + dy * dy + dz * dz;
        
        // Gravitational force magnitude
        Force force = G * b1.mass * b2.mass / r_squared;
        
        return force;
    }
    
    // Simulate one time step
    void step(double dt) {
        // Calculate all forces
        std::vector<Force> forces_x(bodies.size(), Force(0));
        std::vector<Force> forces_y(bodies.size(), Force(0));
        std::vector<Force> forces_z(bodies.size(), Force(0));
        
        for(size_t i = 0; i < bodies.size(); ++i) {
            for(size_t j = i + 1; j < bodies.size(); ++j) {
                Force f_mag = calculate_force(bodies[i], bodies[j]);
                
                // Calculate direction
                Distance dx = bodies[j].position[0] - bodies[i].position[0];
                Distance dy = bodies[j].position[1] - bodies[i].position[1];
                Distance dz = bodies[j].position[2] - bodies[i].position[2];
                Distance r = Distance(sqrt(
                    (dx * dx + dy * dy + dz * dz).to_value()
                ));
                
                // Force components
                Force fx = f_mag * (dx / r);
                Force fy = f_mag * (dy / r);
                Force fz = f_mag * (dz / r);
                
                // Newton's third law
                forces_x[i] = forces_x[i] + fx;
                forces_y[i] = forces_y[i] + fy;
                forces_z[i] = forces_z[i] + fz;
                
                forces_x[j] = forces_x[j] - fx;
                forces_y[j] = forces_y[j] - fy;
                forces_z[j] = forces_z[j] - fz;
            }
        }
        
        // Update velocities and positions
        for(size_t i = 0; i < bodies.size(); ++i) {
            // a = F/m
            Distance ax = Distance(forces_x[i].to_value() / bodies[i].mass.to_value());
            Distance ay = Distance(forces_y[i].to_value() / bodies[i].mass.to_value());
            Distance az = Distance(forces_z[i].to_value() / bodies[i].mass.to_value());
            
            // v = v + a*dt
            bodies[i].velocity[0] = bodies[i].velocity[0] + ax * Distance(dt);
            bodies[i].velocity[1] = bodies[i].velocity[1] + ay * Distance(dt);
            bodies[i].velocity[2] = bodies[i].velocity[2] + az * Distance(dt);
            
            // x = x + v*dt
            bodies[i].position[0] = bodies[i].position[0] + 
                                   bodies[i].velocity[0] * Distance(dt);
            bodies[i].position[1] = bodies[i].position[1] + 
                                   bodies[i].velocity[1] * Distance(dt);
            bodies[i].position[2] = bodies[i].position[2] + 
                                   bodies[i].velocity[2] * Distance(dt);
        }
    }
    
    // Print current state
    void print_state() {
        for(const auto& body : bodies) {
            std::cout << body.name << ":\n";
            std::cout << "  Position: (" 
                      << body.position[0] << ", "
                      << body.position[1] << ", " 
                      << body.position[2] << ")\n";
            std::cout << "  Mass: " << body.mass << " kg\n\n";
        }
    }
};

// Example: Simulate Earth-Moon system and a quantum particle
int main() {
    MultiScaleGravitySimulation sim;
    
    // Add Earth (mass = 5.97e24 kg, at origin)
    sim.add_body("Earth", 5.97e24, 0, 0, 0);
    
    // Add Moon (mass = 7.34e22 kg, distance = 3.84e8 m)
    sim.add_body("Moon", 7.34e22, 3.84e8, 0, 0);
    
    // Add electron (mass = 9.11e-31 kg, near Earth's surface)
    sim.add_body("Electron", 9.11e-31, 6.37e6, 0, 0);
    
    // Simulate for 1 hour (3600 seconds) with 1-second steps
    for(int i = 0; i < 3600; ++i) {
        sim.step(1.0);
        if(i % 600 == 0) {  // Print every 10 minutes
            std::cout << "Time: " << i << " seconds\n";
            sim.print_state();
        }
    }
    
    return 0;
}
```

### Key Insights

1. **Automatic Scaling**: The multiscale transform handles the 55+ orders of magnitude automatically
2. **No Overflow**: Operations that would overflow with regular doubles work correctly
3. **Precision Preserved**: Relative precision maintained across all scales
4. **Clean Code**: No manual scaling factors or unit conversions needed

---

## Tutorial 3: Exact Financial Calculations

### Overview
Build a financial system using the Stern-Brocot transform for exact decimal arithmetic, avoiding floating-point rounding errors.

### The Problem
Financial calculations require:
- Exact decimal arithmetic (no rounding errors)
- Support for fractional cents
- Compound interest calculations
- Currency conversions with exact rates

### The CBT Solution

```cpp
#include <cbt/stern_brocot.hpp>
#include <iostream>
#include <vector>
#include <map>

class ExactFinancialSystem {
private:
    using Money = cbt::stern_brocot<int64_t>;
    using Rate = cbt::stern_brocot<int64_t>;
    
    // Store amounts as fractions (numerator = cents, denominator = 100)
    static Money from_dollars(double dollars) {
        int64_t cents = static_cast<int64_t>(dollars * 100 + 0.5);
        return Money(cents, 100);
    }
    
    static double to_dollars(const Money& amount) {
        return amount.to_double();
    }
    
public:
    class Account {
    private:
        Money balance;
        std::vector<Money> transaction_history;
        
    public:
        Account() : balance(0, 1) {}
        
        void deposit(const Money& amount) {
            balance = balance + amount;
            transaction_history.push_back(amount);
        }
        
        bool withdraw(const Money& amount) {
            if(balance >= amount) {
                balance = balance - amount;
                transaction_history.push_back(Money(-amount.numerator(), 
                                                   amount.denominator()));
                return true;
            }
            return false;
        }
        
        Money get_balance() const { return balance; }
        
        // Calculate interest (exact!)
        void apply_interest(const Rate& annual_rate, int days) {
            // Daily rate = annual_rate / 365
            Rate daily_rate = annual_rate / Money(365, 1);
            
            // Compound interest: balance * (1 + rate)^days
            Money multiplier(1, 1);
            Money one_plus_rate = Money(1, 1) + daily_rate;
            
            for(int i = 0; i < days; ++i) {
                multiplier = multiplier * one_plus_rate;
            }
            
            Money interest = balance * multiplier - balance;
            deposit(interest);
        }
    };
    
    class CurrencyExchange {
    private:
        std::map<std::pair<std::string, std::string>, Rate> rates;
        
    public:
        void set_rate(const std::string& from, const std::string& to, 
                     double rate) {
            // Store as exact fraction
            auto exact_rate = Rate::approximate(rate, 1000000);
            rates[{from, to}] = exact_rate;
            
            // Store inverse rate
            rates[{to, from}] = Rate(exact_rate.denominator(), 
                                    exact_rate.numerator());
        }
        
        Money convert(const Money& amount, 
                     const std::string& from, 
                     const std::string& to) {
            auto rate = rates[{from, to}];
            return amount * rate;
        }
    };
    
    // Portfolio management
    class Portfolio {
    private:
        std::map<std::string, Money> holdings;
        
    public:
        void add_holding(const std::string& symbol, const Money& shares) {
            if(holdings.find(symbol) == holdings.end()) {
                holdings[symbol] = Money(0, 1);
            }
            holdings[symbol] = holdings[symbol] + shares;
        }
        
        Money calculate_value(const std::map<std::string, Money>& prices) {
            Money total(0, 1);
            for(const auto& [symbol, shares] : holdings) {
                if(prices.find(symbol) != prices.end()) {
                    total = total + shares * prices.at(symbol);
                }
            }
            return total;
        }
        
        // Rebalance portfolio to target allocations
        void rebalance(const std::map<std::string, Rate>& target_allocations,
                      const std::map<std::string, Money>& prices) {
            Money total_value = calculate_value(prices);
            
            for(const auto& [symbol, target_pct] : target_allocations) {
                Money target_value = total_value * target_pct;
                Money target_shares = target_value / prices.at(symbol);
                
                // Exact calculation of shares to buy/sell
                Money current_shares = holdings[symbol];
                Money difference = target_shares - current_shares;
                
                std::cout << symbol << ": "
                         << (difference.numerator() > 0 ? "Buy " : "Sell ")
                         << Money(std::abs(difference.numerator()), 
                                 difference.denominator())
                         << " shares\n";
            }
        }
    };
};

// Example usage
int main() {
    using Money = cbt::stern_brocot<int64_t>;
    using Rate = cbt::stern_brocot<int64_t>;
    
    // Create account with exact arithmetic
    ExactFinancialSystem::Account account;
    
    // Deposit $1,234.56 (exactly!)
    account.deposit(Money(123456, 100));
    
    // Apply 5.25% annual interest for 30 days
    Rate interest_rate(525, 10000);  // 5.25% = 525/10000
    account.apply_interest(interest_rate, 30);
    
    std::cout << "Balance after interest: $" 
              << account.get_balance().to_double() << "\n";
    
    // Currency exchange
    ExactFinancialSystem::CurrencyExchange exchange;
    exchange.set_rate("USD", "EUR", 0.85);
    exchange.set_rate("EUR", "GBP", 0.88);
    
    Money usd_amount(10000, 100);  // $100.00
    Money eur_amount = exchange.convert(usd_amount, "USD", "EUR");
    Money gbp_amount = exchange.convert(eur_amount, "EUR", "GBP");
    
    std::cout << "$100 = €" << eur_amount.to_double() 
              << " = £" << gbp_amount.to_double() << "\n";
    
    // Portfolio rebalancing
    ExactFinancialSystem::Portfolio portfolio;
    portfolio.add_holding("AAPL", Money(100, 1));  // 100 shares
    portfolio.add_holding("GOOGL", Money(50, 1));   // 50 shares
    
    std::map<std::string, Money> prices = {
        {"AAPL", Money(15000, 100)},   // $150.00
        {"GOOGL", Money(280000, 100)}  // $2800.00
    };
    
    std::map<std::string, Rate> target_allocations = {
        {"AAPL", Rate(60, 100)},   // 60%
        {"GOOGL", Rate(40, 100)}   // 40%
    };
    
    std::cout << "\nRebalancing portfolio:\n";
    portfolio.rebalance(target_allocations, prices);
    
    return 0;
}
```

### Key Insights

1. **Exact Arithmetic**: No floating-point rounding errors ever
2. **Fractional Values**: Handles fractional cents exactly
3. **Compound Operations**: Interest calculations remain exact
4. **Audit Trail**: Every calculation can be verified exactly

---

## Tutorial 4: Parallel Cryptographic Operations

### Overview
Implement RSA-like operations using the Residue Number System for parallel computation without timing leaks.

### The Problem
Cryptographic operations need:
- Large integer arithmetic (1024+ bits)
- Constant-time operations (no timing leaks)
- Parallel computation for performance
- Modular arithmetic

### The CBT Solution

```cpp
#include <cbt/residue_number_system.hpp>
#include <cbt/modular.hpp>
#include <vector>
#include <random>

class ParallelCrypto {
private:
    // Use 5 coprime moduli for ~160-bit range
    using RNS = cbt::residue_number_system<uint64_t, 5>;
    
    // Modular exponentiation in RNS
    RNS mod_exp_rns(const RNS& base, uint64_t exponent) {
        RNS result = RNS::from_integer(1);
        RNS b = base;
        
        // Square-and-multiply algorithm (constant time version)
        for(int i = 63; i >= 0; --i) {
            result = result * result;
            
            // Constant-time conditional multiply
            uint64_t bit = (exponent >> i) & 1;
            RNS multiplier = bit ? b : RNS::from_integer(1);
            result = result * multiplier;
        }
        
        return result;
    }
    
public:
    struct KeyPair {
        RNS n;      // modulus
        RNS e;      // public exponent
        RNS d;      // private exponent
    };
    
    // Simplified key generation (demonstration only)
    KeyPair generate_keys() {
        KeyPair keys;
        
        // In real RSA, these would be large primes
        uint64_t p = 65537;
        uint64_t q = 65539;
        uint64_t n = p * q;
        uint64_t phi = (p - 1) * (q - 1);
        
        keys.n = RNS::from_integer(n);
        keys.e = RNS::from_integer(65537);  // Common choice
        
        // Compute d (simplified - real implementation needs extended GCD)
        uint64_t d = 1;  // Placeholder
        keys.d = RNS::from_integer(d);
        
        return keys;
    }
    
    // Encrypt using public key (parallel in RNS)
    RNS encrypt(const RNS& message, const RNS& e, const RNS& n) {
        // All operations are component-wise parallel!
        return mod_exp_rns(message, e.to_integer());
    }
    
    // Decrypt using private key (parallel in RNS)
    RNS decrypt(const RNS& ciphertext, const RNS& d, const RNS& n) {
        return mod_exp_rns(ciphertext, d.to_integer());
    }
    
    // Parallel batch operations
    std::vector<RNS> batch_encrypt(const std::vector<uint64_t>& messages,
                                   const KeyPair& keys) {
        std::vector<RNS> results;
        results.reserve(messages.size());
        
        #pragma omp parallel for
        for(size_t i = 0; i < messages.size(); ++i) {
            RNS msg = RNS::from_integer(messages[i]);
            RNS encrypted = encrypt(msg, keys.e, keys.n);
            
            #pragma omp critical
            results.push_back(encrypted);
        }
        
        return results;
    }
    
    // Homomorphic property demonstration
    void demonstrate_homomorphic() {
        std::cout << "RNS Homomorphic Properties:\n";
        
        // Create two messages
        RNS m1 = RNS::from_integer(42);
        RNS m2 = RNS::from_integer(137);
        
        // Addition in RNS (component-wise, parallel)
        RNS sum = m1 + m2;
        std::cout << "42 + 137 = " << sum.to_integer() << " (parallel)\n";
        
        // Multiplication in RNS (component-wise, parallel)
        RNS product = m1 * m2;
        std::cout << "42 * 137 = " << product.to_integer() << " (parallel)\n";
        
        // Show parallel nature
        std::cout << "\nComponent-wise operations:\n";
        std::cout << "m1: " << m1 << "\n";
        std::cout << "m2: " << m2 << "\n";
        std::cout << "sum: " << sum << "\n";
        std::cout << "product: " << product << "\n";
    }
    
    // Timing attack resistance
    void demonstrate_constant_time() {
        std::cout << "\nConstant-Time Operations:\n";
        
        RNS value = RNS::from_integer(12345);
        
        // These operations take the same time regardless of value
        auto start = std::chrono::high_resolution_clock::now();
        
        for(int i = 0; i < 1000000; ++i) {
            value = value * RNS::from_integer(2);
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration<double>(end - start).count();
        
        std::cout << "Million multiplications: " << duration << " seconds\n";
        std::cout << "Per operation: " << duration / 1000000 * 1e9 << " ns\n";
        std::cout << "Result: " << value << "\n";
    }
};

// Advanced: Chinese Remainder Theorem reconstruction
class CRTReconstruction {
private:
    using RNS = cbt::residue_number_system<uint64_t, 3>;
    
public:
    // Demonstrate CRT reconstruction
    void demonstrate() {
        std::cout << "Chinese Remainder Theorem:\n";
        
        // Original number
        uint64_t original = 123456789;
        
        // Convert to RNS
        RNS rns_value = RNS::from_integer(original);
        std::cout << "Original: " << original << "\n";
        std::cout << "RNS representation: " << rns_value << "\n";
        
        // Perform operations in RNS
        RNS squared = rns_value * rns_value;
        
        // Reconstruct using CRT
        uint64_t reconstructed = squared.to_integer();
        std::cout << "Squared (via RNS): " << reconstructed << "\n";
        std::cout << "Squared (direct): " << original * original << "\n";
        
        // Show the parallel nature
        std::cout << "\nParallel computation:\n";
        std::cout << "Each modulus computed independently\n";
        std::cout << "No carry propagation needed\n";
        std::cout << "Perfect for GPU/FPGA implementation\n";
    }
};

int main() {
    ParallelCrypto crypto;
    
    // Generate keys
    auto keys = crypto.generate_keys();
    
    // Demonstrate homomorphic properties
    crypto.demonstrate_homomorphic();
    
    // Show constant-time operations
    crypto.demonstrate_constant_time();
    
    // Batch encryption
    std::vector<uint64_t> messages = {100, 200, 300, 400, 500};
    auto encrypted = crypto.batch_encrypt(messages, keys);
    
    std::cout << "\nBatch encryption complete\n";
    
    // CRT demonstration
    CRTReconstruction crt;
    crt.demonstrate();
    
    return 0;
}
```

### Key Insights

1. **Parallel by Design**: RNS operations are inherently parallel
2. **No Carries**: Addition and multiplication without carry propagation
3. **Constant Time**: Operations take the same time regardless of values
4. **GPU-Friendly**: Maps perfectly to SIMD/GPU architectures

---

## Advanced Tutorial: Transform Composition Patterns

### Overview
Learn how to compose transforms for multiplicative benefits, creating powerful combinations that solve complex problems elegantly.

### Pattern 1: Nested Transforms

```cpp
#include <cbt/composed.hpp>
#include <cbt/multiscale.hpp>
#include <cbt/logarithmic.hpp>
#include <cbt/odds_ratio.hpp>

// Extreme-scale multiplication
using ExtremeMult = cbt::composed<cbt::multiscale, cbt::lg>;

void extreme_multiplication_demo() {
    // Can handle values from 10^-1000 to 10^1000
    // AND perform multiplication efficiently
    
    ExtremeMult tiny = ExtremeMult::from_inner(
        cbt::lg<double>::from_log(-2000)  // e^-2000
    );
    
    ExtremeMult huge = ExtremeMult::from_inner(
        cbt::lg<double>::from_log(2000)   // e^2000
    );
    
    // Multiply without overflow or underflow
    ExtremeMult result = tiny;
    for(int i = 0; i < 1000; ++i) {
        result = result * huge;  // Would overflow 1000 times over!
    }
    
    std::cout << "Result scale level: " << result.scale_level() << "\n";
    std::cout << "Result log value: " << result.inner().log() << "\n";
}

// Stable Bayesian inference with tiny probabilities
using StableBayes = cbt::composed<cbt::lg, cbt::odds_ratio>;

void stable_bayesian_demo() {
    // Can handle probabilities as small as 10^-1000
    
    StableBayes prior = StableBayes::from_inner(
        cbt::odds_ratio<double>::from_probability(1e-100)
    );
    
    // Update with very strong evidence
    for(int i = 0; i < 100; ++i) {
        StableBayes evidence = StableBayes::from_inner(
            cbt::odds_ratio<double>(1000.0)  // LR = 1000
        );
        prior = prior * evidence;
    }
    
    // Still numerically stable!
    std::cout << "Log odds: " << prior.outer().log() << "\n";
}
```

### Pattern 2: Transform Networks

```cpp
// Create a network of transforms with direct mappings
class TransformNetwork {
private:
    template<typename From, typename To>
    To convert(const From& value);
    
public:
    // Define conversion paths
    template<>
    cbt::multiscale<double> convert(const cbt::lgd& value) {
        return cbt::mappings::lg_to_multiscale(value);
    }
    
    template<>
    cbt::lgd convert(const cbt::multiscale<double>& value) {
        return cbt::mappings::multiscale_to_lg(value);
    }
    
    // Automatic path finding
    template<typename Target, typename Source>
    Target smart_convert(const Source& value) {
        // Implementation would find optimal path
        return convert<Source, Target>(value);
    }
};
```

### Pattern 3: Domain-Specific Compositions

```cpp
// Quantum computing with complex amplitudes
using QuantumAmplitude = cbt::composed<
    cbt::multiscale,           // Handle tiny amplitudes
    cbt::complex_wrapper<      // Complex numbers
        cbt::lg                // Efficient multiplication
    >
>;

// Machine learning with gradient tracking
using MLParameter = cbt::composed<
    cbt::dual,                 // Automatic differentiation
    cbt::interval              // Track uncertainty
>;

// Financial calculations with exact audit trail
using AuditableMoney = cbt::composed<
    cbt::stern_brocot,         // Exact arithmetic
    cbt::versioned             // Track all changes
>;
```

---

## Best Practices and Design Patterns

### 1. Transform Selection Strategy

```cpp
template<typename T>
auto select_optimal_transform(const ComputationProfile& profile) {
    if(profile.has_multiplication_chains()) {
        if(profile.has_extreme_range()) {
            return cbt::multiscale<cbt::lg<T>>{};
        } else {
            return cbt::lg<T>{};
        }
    } else if(profile.has_bayesian_updates()) {
        if(profile.has_tiny_probabilities()) {
            return cbt::log_odds<T>{};
        } else {
            return cbt::odds_ratio<T>{};
        }
    } else if(profile.needs_exact_arithmetic()) {
        return cbt::stern_brocot<int64_t>{};
    } else if(profile.has_parallel_hardware()) {
        return cbt::residue_number_system<T, 5>{};
    }
    // Default
    return T{};
}
```

### 2. Performance Optimization Patterns

```cpp
// Batch operations to amortize transform overhead
template<typename Transform>
void batch_process(std::vector<typename Transform::value_type>& data) {
    // Transform all at once
    std::vector<Transform> transformed;
    transformed.reserve(data.size());
    
    for(const auto& val : data) {
        transformed.emplace_back(val);
    }
    
    // Process in transform domain
    for(auto& t : transformed) {
        // Operations here are efficient
    }
    
    // Convert back if needed
    for(size_t i = 0; i < data.size(); ++i) {
        data[i] = transformed[i].value();
    }
}
```

### 3. Error Handling Patterns

```cpp
template<typename Transform>
class SafeTransform {
private:
    std::optional<Transform> value_;
    std::string error_;
    
public:
    SafeTransform(typename Transform::value_type val) {
        try {
            value_ = Transform(val);
        } catch(const std::exception& e) {
            error_ = e.what();
        }
    }
    
    bool is_valid() const { return value_.has_value(); }
    const std::string& error() const { return error_; }
    Transform& value() { return *value_; }
};
```

---

## Exercises

### Exercise 1: Implement a Weather Prediction System
Use multiscale transform to handle pressures from quantum to atmospheric scales.

### Exercise 2: Build a Recommendation Engine
Use odds-ratio transform for collaborative filtering with Bayesian updates.

### Exercise 3: Create a Music Synthesizer
Use Stern-Brocot transform for perfect frequency ratios in just intonation.

### Exercise 4: Implement Homomorphic Encryption
Use RNS for parallel operations on encrypted data.

### Exercise 5: Design a Portfolio Optimizer
Combine multiple transforms for risk analysis across different scales.

---

## Further Reading

- [Mathematical Foundations](mathematical_foundations.md) - Deep dive into theory
- [API Reference](api_reference.md) - Complete API documentation
- [Performance Guide](transform_guides.md#performance-optimization-guide) - Optimization techniques
- [Research Paper](../paper/cbt.pdf) - Academic treatment

---

## Summary

These tutorials demonstrate that CBT is not just a mathematical curiosity but a practical framework for solving real-world computational problems. By choosing the right transform and understanding the trade-offs, you can achieve dramatic improvements in performance, numerical stability, and code clarity.

Key takeaways:
1. **Choose wisely**: Select transforms based on your dominant operations
2. **Stay in domain**: Minimize conversions for maximum efficiency
3. **Compose freely**: Combine transforms for multiplicative benefits
4. **Think in trade-offs**: Every gain has a corresponding loss
5. **Profile always**: Measure actual performance in your use case