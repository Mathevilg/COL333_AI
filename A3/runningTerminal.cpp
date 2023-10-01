#include <bits/stdc++.h>
using namespace std;



int main(){
    const char* command = "minisat out.txt miniOut.txt";

    FILE* pipe = popen(command, "r");

    if (pipe == nullptr) {
        std::cerr << "Error: Failed to open pipe." << std::endl;
        return 1; // Return an error code
    }

    char buffer[128]; // Buffer to read the command output
    std::string result = "";

    // Read the command output and append it to the result string
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result = buffer;
    }

    // Close the pipe
    pclose(pipe);

    // Print or process the captured output
    std::cout << "Command output:\n" << result << std::endl;
}