#include <bits/stdc++.h>
using namespace std;


#include <iostream>
#include <vector>
#include <random>

void gen(int matrixSize) {
    std::vector<std::vector<int>> matrix(matrixSize, std::vector<int>(matrixSize, 0));

    // Seed the random number generator
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(1, 100); // Range for random integers

    // Fill the matrix with random values
    for (int i = 0; i < matrixSize; ++i) {
        for (int j = i+1; j < matrixSize; ++j) {
            if (i != j) {
                matrix[i][j] = distribution(generator);
                matrix[j][i] = matrix[i][j];
            }
        }
    }

    // Display the matrix
    for (int i = 0; i < matrixSize; ++i) {
        for (int j = 0; j < matrixSize; ++j) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }

}

int main(){
    int z, l, t;
    cin >> t >> z >> l;
    cout << t << endl;
    cout << z << endl;
    cout << l << endl;
    gen(z);
    gen(l);
    return 0;
}