#include <iostream>
#include <random>

int main() {
    std::random_device rd; // Seed for the random number generator
    std::mt19937 gen(rd()); // Mersenne Twister PRNG
    std::uniform_real_distribution<double> dis(0.0, 1.0); // Uniform distribution between 0 and 1

    double random_number = dis(gen); // Generate the random number

    std::cout << "Random number in (0, 1): " << random_number << std::endl;

    return 0;
}
