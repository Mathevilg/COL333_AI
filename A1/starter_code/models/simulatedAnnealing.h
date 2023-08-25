#ifndef SIMULATEDANNEALING_H
#define SIMULATEDANNEALING_H


#include <fstream>
#include <iostream>
#include <random>
#include <bits/stdc++.h>
using namespace std;



class simulatedAnnealing
{
private:
    int z, l;
    int **T;
    int **N;
    int time;
    vector<int> mapping;
    long long best;
    vector<int> bestMapping;


public:
    simulatedAnnealing(string inputfilename);

    bool check_output_format();

    long long cost_fn();
    long long cost_fn(vector<int> &arr);

    void write_to_file(string outputfilename);

    void readInInputFile(string inputfilename);



    // 0 means no allocation of zone to that location !
    // 0 base indexing followed !
    vector<int> randomPermutation();

    int getTime();

    long long bestTime();

    void compute_allocation();

    vector<int> findNextNeighbour();

    double generateRandom();
    
};





#endif // !SIMULATEDANNEALING_H