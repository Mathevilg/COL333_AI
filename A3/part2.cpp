#include <bits/stdc++.h>
using namespace std;

#define endl "\n"

int s(int i, int j, int n, int k){
    return (i * (k + 1)) + j + (n + 1);
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


    for (auto it: st){
        cout << it.first << " " << it.second << endl;
    }


    // cout << st.size() << endl;


    string output_filename = "out.txt";
    ofstream outputFile(output_filename);
    if (!outputFile.is_open()) {
        cout << "cant write to output file \n" ;
        return 1;
    }

    int k = stoi(argv[1]);

    int clauses = 0;

    // clauses += n ; // for n vertices to be in or out !!
    clauses += 1 ; // for (n, k) to be true
    clauses += 4*(n)*(k); // for non base cases of (i, j)
    clauses += n + k + 1 ; // for base cases
    clauses += st.size(); // for the non edges !!


    outputFile << "p cnf " << (n + (n+1)*(k+1) ) << " " << clauses <<  endl;

    outputFile << s(n, k, n, k) << " 0" <<  endl;

    for (int i=1; i<=n; i++){
        for (int j=1; j<=k; j++){
            outputFile << "-" << s(i-1, j, n, k) << " " << s(i, j, n, k) << " 0" <<  endl;
            outputFile << "-" << s(i-1, j-1, n, k) << " " << s(i, j, n, k) << " " << "-" << i << " 0" <<  endl;
            outputFile << "-" << s(i, j, n, k) << " " << s(i-1, j-1, n, k) << " " << s(i-1, j, n, k) << " 0" <<  endl;
            outputFile << "-" << s(i, j, n, k) << " " << i << " " << s(i-1, j, n, k) << " 0" <<  endl;
        }
    }


    for (int j = 1; j<= k; j++){
        outputFile << "-" << s(0, j, n, k) << " 0" <<  endl;
    }

    for (int i = 0; i<=n; i++) {
        outputFile << s(i, 0, n, k) << " 0" <<  endl;
    }

    for (set<pair<int, int> >:: iterator  it =  st.begin(); it != st.end(); it++){
        u = (*it).first;
        v = (*it).second;
        outputFile << "-" << u << " " << "-" << v << " 0" <<  endl;
    }


    return 0;
}