#include<bits/stdc++.h>
using namespace std;

vector<int> timeArr;
int maxCostTillGoal = 50;
int nodesVis = 0;
int maxDepth = 6;
int maxAllowedNodes = 500;

class state{
public:
    vector<bool> stat;
    state(int p){
        for (int i = 0; i < p + 1; ++i) {
            stat.push_back(false);
        }
    }

    bool isGoal(){;
        bool ans = true;
        for (int i = 0; i < stat.size(); ++i) {
            ans = ans and stat[i];
        }
        return ans;
    }

};


class Node
{
private:
    /* data */
    vector<vector<int>> pathTillNow;
public:
    state *currState;
    int g;
    int depth = 0;
    Node(int p){
        g = 0;
        currState = new state(p);
    }
    void printPath(){
        cout << "printing a new valid path" << endl;
        for (auto &step: pathTillNow) {
//            if (step[0]) cout<< step[1] << " and " << step[2] <<  " gone to left from right with cost "<< max(timeArr[step[1]], timeArr[step[2]]) << endl;
//            else
                cout << step[0] << " and " << step[1] << " moved with cost " << step[2] << endl;
        }
        cout << "\n\n\n\n";
    }
    bool isGoal(){
        return currState->isGoal();
    }

    void addEdgeToPath(vector<int> &edge){
        pathTillNow.push_back(edge);
    }

    void removeLastEdge(){
        if (pathTillNow.size()){
            pathTillNow.pop_back();
        }
    }
};

void branchAndBound(Node *curr){
    if(curr->depth > maxDepth or nodesVis > maxAllowedNodes or curr->g >= maxCostTillGoal) {
//        cout<<"finished\n"<<curr->depth<<"\n"<<nodesVis<<"\n"<<curr->g<<"\n";
        return;
    }
    if (curr->isGoal()){
        curr->printPath();
        maxCostTillGoal = min(curr->g, maxCostTillGoal);
        return;
    }
    nodesVis++;
    curr->depth++;
//    cout<<not
    curr->currState->stat[0] = not curr->currState->stat[0];
    int length = curr->currState->stat.size();
    vector<bool> &v = curr->currState->stat;
    for (int i = 1; i < length; ++i) {
        if (not v[0] == v[i]){
            v[i] = not v[i];
            curr->g += timeArr[i - 1];
            vector<int> edge = {i - 1, -1, timeArr[i - 1]};
            curr->addEdgeToPath(edge);
            branchAndBound(curr);
//            cout<<"hello\n";
            curr->removeLastEdge();
            curr->g -= timeArr[i - 1];
            for (int j = i + 1; j < length; ++j) {
//                cout<<"hi "<<not v[0]<<" "<<v[j]<<"\n";
                if ((not v[0]) == v[j]){
//                    cout<<"op\n";
                    v[j] = not v[j];
                    curr->g += max(timeArr[i - 1], timeArr[j - 1]);
                    edge = {i - 1, j - 1, max(timeArr[i - 1], timeArr[j - 1])};
                    curr->addEdgeToPath(edge);
                    branchAndBound(curr);
                    curr->removeLastEdge();
                    curr->g -= max(timeArr[i - 1], timeArr[j - 1]);
                    v[j] = not v[j];
                }
            }
            v[i] = not v[i];
        }
    }
    curr->depth--;
    curr->currState->stat[0] = not curr->currState->stat[0];
    return;
}

int main(){
    int t;
    cin>>t;
    for (int i = 0; i < t; ++i) {
        int x;
        cin>>x;
        timeArr.push_back(x);
    }
    Node *x = new Node(t);
    branchAndBound(x);
}