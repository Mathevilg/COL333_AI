#include <bits/stdc++.h>
using namespace std;

int s(int i, int j, int k, int offset){
    return (i * (k + 1)) + j + (offset + 1);
}




void generateSAT(int n, int k1, int k2, set<pair<int, int> > &st, string output_filename = "out_1.txt"){

    ofstream outputFile(output_filename);



    if (!outputFile.is_open()) {
        cout << "cant write to output file \n" ;
        return ;
    }



    int clauses = 0;

    clauses += 1 ; // for (n, k1) to be true
    clauses += 4*(n)*(k1); // for non base cases of (i, j)
    clauses += n + k1 + 1 ; // for base cases
    clauses += st.size(); // for the non edges !!

    clauses += 1 ; // for (n, k2) to be true
    clauses += 4*(n)*(k2); // for non base cases of (i, j)
    clauses += n + k2 + 1 ; // for base cases
    clauses += st.size(); // for the non edges !!


    clauses += n ; // for non - overlapping sets



    outputFile << "p cnf " << ((n*2) + (n+1)*(k1+1)+ (n+1)*(k2+1)) << " " << clauses <<  endl;
    int offset = 2 * n;

    // start for set 1
    outputFile << s(n, k1, k1, offset) << " 0" <<  endl;

    for (int i=1; i<=n; i++){
        for (int j=1; j<=k1; j++){
            outputFile << -s(i-1, j, k1, offset) << " " << s(i, j, k1, offset) << " 0" <<  endl;
            outputFile << -s(i-1, j-1, k1, offset) << " " << s(i, j, k1, offset) << " " << -i << " 0" <<  endl;
            outputFile << -s(i, j, k1, offset) << " " << s(i-1, j-1, k1, offset) << " " << s(i-1, j, k1, offset) << " 0" <<  endl;
            outputFile << -s(i, j, k1, offset) << " " << i << " " << s(i-1, j, k1, offset) << " 0" <<  endl;
        }
    }


    for (int j = 1; j<= k1; j++){
        outputFile << -s(0, j, k1, offset) << " 0" <<  endl;
    }

    for (int i = 0; i<=n; i++) {
        outputFile << s(i, 0, k1, offset) << " 0" <<  endl;
    }

    int u, v;

    for (set<pair<int, int> >:: iterator  it =  st.begin(); it != st.end(); it++){
        u = (*it).first;
        v = (*it).second;
        outputFile << -u << " " << -v << " 0" <<  endl;
    }

    // start for set 2
    offset +=  (n+1)*(k1+1);

    outputFile << s(n, k2, k2, offset) << " 0" <<  endl;

    for (int i=1; i<=n; i++){
        for (int j=1; j<=k2; j++){
            outputFile << -s(i-1, j, k2, offset) << " " << s(i, j, k2, offset) << " 0" <<  endl;
            outputFile << -s(i-1, j-1, k2, offset) << " " << s(i, j, k2, offset) << " " << - (i + n) << " 0" <<  endl;
            outputFile << -s(i, j, k2, offset) << " " << s(i-1, j-1, k2, offset) << " " << s(i-1, j, k2, offset) << " 0" <<  endl;
            outputFile << -s(i, j, k2, offset) << " " << i + n << " " << s(i-1, j, k2, offset) << " 0" <<  endl;
        }
    }


    for (int j = 1; j<= k2; j++){
        outputFile << -s(0, j, k2, offset) << " 0" <<  endl;
    }

    for (int i = 0; i<=n; i++) {
        outputFile << s(i, 0, k2, offset) << " 0" <<  endl;
    }


    for (set<pair<int, int> >:: iterator  it =  st.begin(); it != st.end(); it++){
        u = (*it).first;
        v = (*it).second;
        outputFile << - (u + n) << " " << - (v + n) << " 0" <<  endl;
    }


    // non - overlapping sets 
    for (int i=1; i<= n; i++){
        outputFile << -i << " " << - (i + n) << " 0" << endl;
    }

}



int main(int argc, char* argv[]){

    int n, m, k1, k2;
    cin >> n >> m >> k1 >> k2;

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

    string output_filename = argv[1];
    output_filename += ".satinput";
    generateSAT(n, k1, k2, st, output_filename);

    return 0;

}