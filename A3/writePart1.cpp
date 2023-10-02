#include <bits/stdc++.h>
using namespace std;




void write(int n, int k1, int k2, string sat_output = "satOut1.txt", string output_file = "test_1.mapping"){
    ifstream inputFile(sat_output);
    string result;
    getline(inputFile, result);
    bool out =  result == "SAT";
    cout << "\n\n\n\n\n\n bool isSAT ? ------->>>>>>" << "     " << out << "\n\n\n\n\n\n\n\n";
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


int main(int argc, char* argv[]){

    int n, m, k1, k2;
    cin >> n >> m >> k1 >> k2;
    string sat_output = argv[1];
    sat_output += ".satoutput";
    string output_file = argv[1];
    output_file += ".mapping";
    // cout << sat_output << " " << output_file << endl;
    // cout << n << " " << m << " " << k1 << " " << k2 << endl;
    write(n, k1, k2, sat_output, output_file);
    return 0;
}