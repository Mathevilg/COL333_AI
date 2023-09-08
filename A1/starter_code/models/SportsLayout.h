
#ifndef SPORTSLAYOUT_H
#define	SPORTSLAYOUT_H

#include <fstream>
#include <iostream>
#include <random>
#include <chrono>
#include <bits/stdc++.h>
using namespace std;

class SportsLayout{

    private:
    int z,l;
    int** T;
    int **N;
    int time;
    long long currCost;
    long long bestNeighbourCost;
    long long best;
    // int best_i;
    // int best_j;
    vector<int> mapping;
    vector<int> bestMapping;


    public:
    SportsLayout(string inputfilename);

    bool check_output_format();

    // void readOutputFile(string output_filename);
    
    long long cost_fn();

    long long cost_fn_best();

    long long gain(int i0, int j0);

    void write_to_file(string outputfilename);

    void readInInputFile(string inputfilename);

    void compute_allocation();

    int getTime();

    vector<int> generateRandomPermutation(int n);

    long long findNextNeighbor();


};


#endif