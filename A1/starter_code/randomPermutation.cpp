#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

std::vector<int> generateRandomPermutation(int n) {
    std::vector<int> permutation(n);
    for (int i = 0; i < n; ++i) {
        permutation[i] = i + 1; // Initialize with consecutive integers
    }

    // Use random_device to seed the random number generator
    std::random_device rd;
    std::mt19937 gen(rd());

    // Perform the Fisher-Yates shuffle
    for (int i = n - 1; i > 0; --i) {
        // Generate a random index between 0 and i (inclusive)
        std::uniform_int_distribution<int> dist(0, i);
        int randomIndex = dist(gen);

        // Swap the elements at randomIndex and i
        std::swap(permutation[i], permutation[randomIndex]);
    }

    return permutation;
}

int main() {
    int n;
    std::cout << "Enter the value of n: ";
    std::cin >> n;

    std::vector<int> randomPerm = generateRandomPermutation(n);

    std::cout << "Random Permutation:";
    for (int num : randomPerm) {
        std::cout << " " << num;
    }
    std::cout << std::endl;

    return 0;
}
