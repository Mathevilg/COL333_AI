#include<bits/stdc++.h>
#include<random>
using namespace std;


int z, l;
int **T;
int **N;
vector<int> seed;

vector<int> generateRandomPermutation() {
    random_device rd;
    mt19937 gen(rd());
    shuffle(seed.begin(), seed.end(), gen);
    return seed;
}


void printT()
{
    for (int i=0; i<l; i++)
    {
        for (int j=0; j<l; j++) cout << T[i][j] << " ";
        cout << endl;
    }
}

void printVector(vector<int> &arr)
{
    int i=0;
    for ( ; i<arr.size(); i++) cout << arr[i] << " ";
    cout << endl;
}


long long calculateCostBrute(){
    long long ans = 0;
    for (int i=0; i<z; i++) {
        for (int j=0; j<z; j++){
            ans += T[i][j]*N[i][j];
        }
    }
    return ans;
}



long long gain(int i, int j) {
    long long ans = 0;
    i++;
    j++;
    for (int k=1; k<=l; k++)
    {
        if (k == i || k == j) continue;
        // in case of bound error just return 0;
        // works in case of mapping from z to l !!
        ans += (T[k-1][j-1] - T[k-1][i-1])*((N[i-1][k-1] + N[k-1][i-1]) - (N[j-1][k-1] + N[k-1][j-1]));

    }
    return ans;
}


long long findNextNeighbour(){
    long long mini = LLONG_MAX;
    int best_i = 0;
    int best_j = 0;
    for (int i=0; i<z; i++){
        for (int j=i+1; j<l; j++){
            long long gained  = gain(i, j);
            if (gained < mini){
                mini = gained;
                // cout << mini << endl;
                best_i = i;
                best_j = j;
            }
        }
    }
    if (mini < 0){
        swap(seed[best_i], seed[best_j]);
        swap(T[best_i], T[best_j]);
        for (int i=0; i<l; i++) swap(T[i][best_i], T[i][best_j]);
        return mini;
    }
    return 0;
}

int main(){

    cin >> z >> l;
    T = new int*[l];
    for (int i = 0; i < l; ++i)
        T[i] = new int[l];
    
    N = new int*[l];
    for (int i = 0; i < l; ++i)
        N[i] = new int[l];
    for(int i=0;i<z;i++)
    {
        for(int j=0;j<z;j++)
        cin>>N[i][j];
        for(int j=z;j<l;j++)
        N[i][j] = 0;
    }
    for(int i=z;i<l;i++)
    {
        for(int j=0;j<l;j++)
        {
            N[i][j] = 0;
        }
    }

    for(int i=0;i<l;i++)
    {
        for(int j=0;j<l;j++)
        cin>>T[i][j];
    }

    seed.resize(l);
    for (int i=0; i<l; i++) seed[i] = i+1;
    // input complete !

    // printT(T, l);
    
    // for (int i=0; i<l; i++){
    //     vector<int> arr = generateRandomPermutation();
    //     printVector(arr);
    // }



    // start !
    generateRandomPermutation();
    long long currCost = calculateCostBrute();
    printVector(seed);
    vector<int> arr(l);
    for (int i=0; i<l; i++) arr[i] = i+1;
    // modify T according to seed;
    for (int i=0; i<l; i++){
        if (seed[i] == arr[i]) continue;
        for (int j=i+1; j<l; j++) {
            if (arr[j] == seed[i]){
                currCost += gain(i, j);
                swap(arr[i], arr[j]);
                swap(T[i], T[j]);
                for (int k=0; k<l; k++) swap(T[k][i], T[k][j]);
                break;
            }
        }
    }
    while (true){
        long long gained = findNextNeighbour();
        if (gained >= 0) break;
        currCost += gained;
        cout << currCost << endl;
        printVector(seed);
        printT();
    }


    return 0;
}