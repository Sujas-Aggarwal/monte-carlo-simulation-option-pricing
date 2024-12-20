#include <iostream>
#include <cmath>
#include <random>
#include <vector>

extern "C" {

// Function to generate normally distributed random numbers
double generateGaussianNoise(double mean, double stddev) {
    static std::mt19937 generator(std::random_device{}());
    std::normal_distribution<double> distribution(mean, stddev);
    return distribution(generator);
}

// Function to calculate the payoff of a European call option
double callOptionPayoff(double S, double K) {
    return std::max(S - K, 0.0);
}

// Function to calculate the payoff of a European put option
double putOptionPayoff(double S, double K) {
    return std::max(K - S, 0.0);
}

// Monte Carlo Simulation for European option pricing
double monteCarloOptionPricing(double S0, double K, double r, double sigma, double T, int numSimulations, bool isCallOption) {
    double payoffSum = 0.0;

    for (int i = 0; i < numSimulations; ++i) {
        // Generate a random price path
        double ST = S0 * std::exp((r - 0.5 * sigma * sigma) * T + sigma * std::sqrt(T) * generateGaussianNoise(0.0, 1.0));

        // Calculate the payoff for this path
        double payoff = isCallOption ? callOptionPayoff(ST, K) : putOptionPayoff(ST, K);

        // Accumulate the payoff
        payoffSum += payoff;
    }

    // Calculate the average payoff and discount it to present value
    double averagePayoff = payoffSum / static_cast<double>(numSimulations);
    return std::exp(-r * T) * averagePayoff;
}

// Wrapper functions for JavaScript
double calculateCallOption(double S0, double K, double r, double sigma, double T, int numSimulations) {
    return monteCarloOptionPricing(S0, K, r, sigma, T, numSimulations, true);
}

double calculatePutOption(double S0, double K, double r, double sigma, double T, int numSimulations) {
    return monteCarloOptionPricing(S0, K, r, sigma, T, numSimulations, false);
}

}
