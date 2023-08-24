#include <iostream>
#include <random>
#include <set>

std::pair<int, int> generateDistinctRandomNumbers(int n) {
    if (n < 2) {
        std::cerr << "Cannot generate distinct numbers for n < 2" << std::endl;
        return std::make_pair(0, 0);
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, n);

    std::set<int> distinctNumbers;

    int num1 = dist(gen);
    distinctNumbers.insert(num1);

    int num2;
    do {
        num2 = dist(gen);
    } while (distinctNumbers.count(num2) > 0);

    return std::make_pair(num1, num2);
}

int main() {
    int n;
    std::cout << "Enter the value of n: ";
    std::cin >> n;

    std::pair<int, int> distinctNumbers = generateDistinctRandomNumbers(n);

    std::cout << "Distinct random numbers: " << distinctNumbers.first << " and " << distinctNumbers.second << std::endl;

    return 0;
}
