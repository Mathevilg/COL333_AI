#include <bits/stdc++.h>
using namespace std;



bool checkClique(set<pair<int, int> >& graph, vector<int> &st){
    int k = st.size();
    for (int i=0; i<k; i++){
        for (int j=i+1; j<k; j++){
            if (graph.find(make_pair(min(st[i], st[j]), max(st[i], st[j]))) == graph.end()) return false;
        }
    }
    return true;
}


bool checkOverlapping(vector<int> &st1, vector<int> &st2){
    set<int> st;
    for (int i=0; i<st1.size(); i++) st.insert(st1[i]);
    for (int i=0; i<st2.size(); i++) {
        if (st.find(st2[i]) != st.end()) return true;
    }
    return false;
}


int main(int argc, char* argv[]) {
    // Specify the file path
    
    if (argv[1][0] == '1'){
        string file_path = "test.mapping";

        ifstream file(file_path);
        if (!file.is_open()) {
            cerr << "Failed to open file: " << file_path << endl;
            return 1;
        }

        string line;
        vector<int> set1;
        vector<int> set2;

        // Read the second line (line number 2) into a vector of ints
        for (int i = 1; i <= 2; ++i) {
            getline(file, line);
            if (i == 2) {
                istringstream iss(line);
                int num;
                while (iss >> num) {
                    set1.push_back(num);
                }
            }
        }

        // Reset the file stream and read the fourth line (line number 4) into a vector of ints
        file.clear();
        file.seekg(0);

        for (int i = 1; i <= 4; ++i) {
            getline(file, line);
            if (i == 4) {
                istringstream iss(line);
                int num;
                while (iss >> num) {
                    set2.push_back(num);
                }
            }
        }

        file.close();

        int n, m, k1, k2;
        cin >> n >> m >> k1 >> k2;
        set<pair<int, int> > st;
        int u, v;
        for (int i=0; i<m; i++){
            cin >> u >> v;
            st.insert(make_pair(min(u, v), max(u, v)));
        }
        cout << "\n\n\n\n\n\n";
        if (checkClique(st, set1)) cout << "set1 is a clique of size " << set1.size() << endl;
        if (checkClique(st, set2)) cout << "set2 is a clique of size " << set2.size() << endl;
        if (!checkOverlapping(set1, set2)) cout << "They are distinct as expected " << endl;
        cout << "\n\n\n\n\n\n";
    }
    else {
        string file_path = "test2.mapping";

        ifstream file(file_path);
        if (!file.is_open()) {
            cerr << "Failed to open file: " << file_path << endl;
            return 1;
        }

        string line;
        vector<int> set1;

        // Read the second line (line number 2) into a vector of ints
        for (int i = 1; i <= 2; ++i) {
            getline(file, line);
            if (i == 2) {
                istringstream iss(line);
                int num;
                while (iss >> num) {
                    set1.push_back(num);
                }
            }
        }

        file.close();

        int n, m, k1, k2;
        cin >> n >> m >> k1 >> k2;
        set<pair<int, int> > st;
        int u, v;
        for (int i=0; i<m; i++){
            cin >> u >> v;
            st.insert(make_pair(min(u, v), max(u, v)));
        }
        cout << "\n\n\n\n\n\n";
        if (checkClique(st, set1)) cout << "set1 is a clique of size " << set1.size() << endl;
        cout << "\n\n\n\n\n\n";
    }

    return 0;
}
