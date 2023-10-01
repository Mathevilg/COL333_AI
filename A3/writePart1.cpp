#include <bits/stdc++.h>
using namespace std;




void write(int n, int k1, int k2, string sat_output = "satOut1.txt"){
    ifstream inputFile(sat_output);
    string result;
    getline(inputFile, result);
    bool out =  result == "SAT";
    // cout << "\n\n\n\n\n\n" << returnCode << " " << result << out << "\n\n\n\n\n\n\n\n";
    string output_file = "part1_ans.txt";
    ofstream outFile(output_file);
    if (out) {
        string secondLine;
        getline(inputFile, secondLine);
        istringstream iss(secondLine);
        int assignment;


        outFile << "#1" << endl;
        for (int i = 0; i < n; i++){
            iss >> assignment;
            if (assignment > 0 && k1 > 0) {
                outFile << assignment << " ";
                k1 --;
            }
        }
        outFile << endl;


        outFile << "#2" << endl;
        for (int i = n; i < n*2; i++){
            iss >> assignment;
            if (assignment > 0 && k2 > 0) {
                outFile << assignment - n << " ";
                k2 --;
            }
        }
        outFile << endl;

    }
    else {
        outFile << 0 << endl;
    }
}


int main(){

    int n, m, k1, k2;
    cin >> n >> m >> k1 >> k2;
    write(n, k1, k2);
    return 0;
}