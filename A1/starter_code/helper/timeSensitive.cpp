#include <bits/stdc++.h>
#include <chrono>
using namespace std;



int main() {
    auto start = std::chrono::high_resolution_clock::now();

    // Your time-sensitive code here
    vector<int> arr(1000000);
    

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "Execution time: " << duration << " ms\n";

    return 0;
}
