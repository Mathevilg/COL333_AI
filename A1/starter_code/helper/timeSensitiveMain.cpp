#include <bits/stdc++.h>
using namespace std;


int main(){

    
    int t;
    cin >> t;
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

    // Your program's code here
    vector<int> arr(1000000);
    // Get the end time
    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();

    // Calculate the duration using std::chrono::duration_cast
    int duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "Program execution time: " << duration << " milliseconds" << std::endl;

    return 0;
}




