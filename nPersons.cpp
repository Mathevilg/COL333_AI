#include<bits/stdc++.h>
using namespace std;

vector<int> timeArr;
int maxCostTillGoal = 1e9;
class state{
    private:
    
    
    public:
    bool torchOnLeft;
    set<int> left, right;
    state(){
        torchOnLeft = true;
        for (int i=0; i<timeArr.size(); i++) left.insert(i); // 0 base indexing
        right.clear();
    }

    bool isGoal(){
        return left.empty();
    }

    bool isTorchOnLeft(){
        return torchOnLeft;
    }
};


class Node
{
private:
    /* data */
    vector<vector<int> > pathTillNow;
public:
    state currState;
    int g;
    Node(){
        g = 0;
        state start;
        currState = start;
    }
    
    void printPath(){
        cout << "printing a new valid path" << endl;
        for (auto &step: pathTillNow) {
            if (step[0]) cout << step[1] << " gone to left from right with cost " << timeArr[step[1]] << endl;
            else cout << step[1] << " and " << step[2] << " gone to right from left with cost " << max(timeArr[step[1]], timeArr[step[2]]) << endl; 
        }
        cout << "\n\n\n\n";
    }

    bool isGoal(){
        return currState.isGoal();
    }

    void addEdgeToPath(vector<int> &edge){
        pathTillNow.push_back(edge);
    }

    void removeLastEdge(){
        if (pathTillNow.size()){
            pathTillNow.pop_back();
        }
    }

    bool isTorchOnLeft(){
        return currState.isTorchOnLeft();
    }

};


void recurse(Node node){
    if (node.g > maxCostTillGoal) return ; 
    if (node.isGoal()) {
        maxCostTillGoal = min(node.g, maxCostTillGoal);
        cout << "raeched a goal !! " << endl;
        node.printPath();
        return ;
    }
    cout << node.g << endl;
    node.printPath() ;
    
    if (node.isTorchOnLeft()){
        // chosing 2 persons 
        int p1, p2;
        for (auto it: node.currState.left){
            p1 = it;
            node.currState.left.erase(p1);
            for (auto it: node.currState.left){
                p2 = it;
                node.currState.left.erase(p2);
                node.currState.torchOnLeft = false;
                int cost = max(timeArr[p1], timeArr[p2]);
                node.g += cost;
                vector<int> edge;
                edge.push_back(0);
                edge.push_back(p1);
                edge.push_back(p2);
                node.addEdgeToPath(edge);
                node.currState.right.insert(p1);
                node.currState.right.insert(p2);
                recurse(node);
                node.currState.right.erase(p1);
                node.currState.right.erase(p2);
                node.removeLastEdge();
                node.g -= cost;
                node.currState.torchOnLeft = true;
                node.currState.left.insert(p2);
            }
            node.currState.left.insert(p1);
        }
    }
    else {
        // case 1 sending 1 person back
        for (auto it: node.currState.right){
            int p = it;
            node.currState.right.erase(p);
            vector<int> edge;
            edge.push_back(1);
            edge.push_back(p);
            node.addEdgeToPath(edge);
            int cost = timeArr[p];
            node.g += cost;
            node.currState.torchOnLeft = true;
            node.currState.left.insert(p);
            recurse(node);
            node.currState.left.erase(p);
            node.currState.torchOnLeft = false;
            node.g -= cost;
            node.removeLastEdge();
            node.currState.right.insert(p);
        }

        // case 2 sending 2 people back

    }
}



int main(){
    int n;
    cin >> n ;
    vector<int> time(n);
    for (int i=0; i<n; i++) cin >> time[i];
    timeArr = time;
    Node start;
    recurse(start);
    return 0;
}