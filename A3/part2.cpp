#include <bits/stdc++.h>
using namespace std;

#define endl "\n"

vector<int> ans;

int s(int i, int j, int n, int k){
    return (i * (k + 1)) + j + (n + 1);
}



bool checkSat(string& sat_input, string& sat_output){
    string ter = "minisat " + sat_input + " " + sat_output ;
    // string ter = "minisat " + sat_input + " " + sat_output + " > /dev/null 2>&1";
    const char* command = ter.c_str();
    int returnCode = system(command);
    bool out = false;

    ifstream inputFile(sat_output);
    string result;
    getline(inputFile, result);
    out =  result == "SAT";
    // cout << "\n\n\n\n\n\n" << returnCode << " " << result << out << "\n\n\n\n\n\n\n\n";
    if (out) {
        string secondLine;
        getline(inputFile, secondLine);
        istringstream iss(secondLine);
        int assignment;
        for (int i = 0; i < ans.size()-1; i++){
            iss >> assignment;
            ans[abs(assignment)] = assignment > 0;
        }
    }
    return out;
}


void generateSAT(int n, int k, set<pair<int, int> > &st, string &sat_input){

    ofstream outputFile(sat_input);



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



bool predicate(int n, int k, set<pair<int, int> > &st, string &sat_input, string &sat_output){
    generateSAT(n, k, st, sat_input);
    return checkSat(sat_input, sat_output);
}


int search(int n, set<pair<int, int> > &st, string &sat_input, string &sat_output){
    int low = 1;
    int high = n;
    while (low <= high){
        int mid = (low+high)/2;
        if (predicate(n, mid, st, sat_input, sat_output)) low = mid + 1;
        else high = mid - 1;
    }
    return high;
}



void write(int flag, string& output_file){
    ofstream outFile(output_file);

    if (!flag) {
        outFile << 0 << endl;
        return ;
    }

    outFile << "#1" << endl;
    for (int i=1; i<ans.size(); i++){
        if (ans[i]) outFile << i << " ";
    }
    outFile << endl;
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


    ans.resize(n+1);
    for (int i=0; i<= n; i++) ans[i] = 0;



    string output_file = argv[1];
    output_file += ".mapping";
    string sat_input = argv[1];
    sat_input += ".satinput";
    string sat_output = argv[1];
    sat_output += ".satoutput";
    int clique = search(n, st, sat_input, sat_output);
    if (!clique) write(0, output_file);
    else write(clique, output_file);
    
    return 0;
}