#include <bits/stdc++.h>
using namespace std;


int main() {
    // Create an input file stream object
    std::ifstream inputFile("example.txt");

    // Check if the file was opened successfully
    if (!inputFile.is_open()) {
        std::cerr << "Error: Could not open the file." << std::endl;
        return 1; // Return an error code
    }

    // Skip the first line (assuming it's not needed)
    std::string skipLine;
    std::getline(inputFile, skipLine);

    // Read the second line into a string
    std::string secondLine;
    std::getline(inputFile, secondLine);

    // Create an istringstream to parse the second line
    std::istringstream iss(secondLine);

    int number;
    std::vector<int> numbers;

    // Read integers from the second line
    while (iss >> number) {
        numbers.push_back(number);
    }

    // Close the file
    inputFile.close();

    // Print the integers from the second line
    for (int num : numbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0; // Return a success code
}
