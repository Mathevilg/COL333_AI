
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
    int z;
    int** T;
    int **N;
    float time;
    long long currCost;
    long long bestNeighbourCost;
    long long best;
    vector<int> mapping;
    // vector<int> bestMapping;


    public:
    // int iterations;
    float maxTime;
    std::chrono::high_resolution_clock::time_point start_time;
    // vector<int> tempmapping;
    vector<int> bestMapping;
    int l;
    SportsLayout(string inputfilename);

    bool check_output_format();

    // void readOutputFile(string output_filename);
    
    long long cost_fn();

    long long cost_fn_best();

    long long gain(int i0, int j0);

    void write_to_file(string outputfilename);

    void readInInputFile(string inputfilename);

    void compute_allocation();

    float getTime();

    vector<int> generateRandomPermutation(int n);

    long long findNextNeighbor();

    // long long findNextNeighborApprox();


};


#endif