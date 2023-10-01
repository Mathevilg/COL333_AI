#include <bits/stdc++.h>
using namespace std;

#define endl "\n"

int s(int i, int j, int n, int k){
    return (i * (k + 1)) + j + (n + 1);
}



bool checkSat(string sat_input = "out.txt", string sat_output = "miniOut.txt"){
    const char* command = "minisat out.txt miniOut.txt";
    FILE* pipe = popen(command, "r");

    // if (pipe == nullptr) {
    //     std::cerr << "Error: Failed to open pipe." << std::endl;
    //     return 1; // Return an error code
    // }

    char buffer[128]; // Buffer to read the command output
    string result = "";

    // Read the last command output 
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result = buffer;
    }

    // Close the pipe
    pclose(pipe);

    // Print or process the captured output
    return result == "SATISFIABLE\n";
}


void generateSAT(int n, int k, set<pair<int, int> > &st){

    string output_filename = "out.txt";
    ofstream outputFile(output_filename);



    if (!outputFile.is_open()) {
        cout << "cant write to output file \n" ;
        return ;
    }



    int clauses = 0;

    clauses += 1 ; // for (n, k) to be true
    clauses += 4*(n)*(k); // for non base cases of (i, j)
    clauses += n + k + 1 ; // for base cases
    clauses += st.size(); // for the non edges !!


    outputFile << "p cnf " << (n + (n+1)*(k+1) ) << " " << clauses <<  endl;

    outputFile << s(n, k, n, k) << " 0" <<  endl;

    for (int i=1; i<=n; i++){
        for (int j=1; j<=k; j++){
            outputFile << -s(i-1, j, n, k) << " " << s(i, j, n, k) << " 0" <<  endl;
            outputFile << -s(i-1, j-1, n, k) << " " << s(i, j, n, k) << " " << -i << " 0" <<  endl;
            outputFile << -s(i, j, n, k) << " " << s(i-1, j-1, n, k) << " " << s(i-1, j, n, k) << " 0" <<  endl;
            outputFile << -s(i, j, n, k) << " " << i << " " << s(i-1, j, n, k) << " 0" <<  endl;
        }
    }


    for (int j = 1; j<= k; j++){
        outputFile << -s(0, j, n, k) << " 0" <<  endl;
    }

    for (int i = 0; i<=n; i++) {
        outputFile << s(i, 0, n, k) << " 0" <<  endl;
    }

    int u, v;

    for (set<pair<int, int> >:: iterator  it =  st.begin(); it != st.end(); it++){
        u = (*it).first;
        v = (*it).second;
        outputFile << -u << " " << -v << " 0" <<  endl;
    }
}



bool predicate(int n, int k, set<pair<int, int> > &st){
    generateSAT(n, k, st);
    return checkSat();
}


int search(int n, set<pair<int, int> > &st){
    int low = 1;
    int high = n;
    while (low <= high){
        int mid = (low+high)/2;
        if (predicate(n, mid, st)) low = mid + 1;
        else high = mid - 1;
    }
    return high;
}


int main(int argc, char* argv[]){

    int n, m;
    cin >> n >> m;

    set<pair<int, int> > st;
    for (int i=1; i<=n; i++){
        for (int j=i+1; j<=n; j++){
            st.insert(make_pair(i, j));
        }
    }
    int u, v;
    for (int i=0; i<m; i++){
        cin >> u >> v;
        st.erase(make_pair(min(u, v), max(u, v)));
    }


    // int k = stoi(argv[1]);

    cout << search(n, st) << endl;

    


    return 0;
}