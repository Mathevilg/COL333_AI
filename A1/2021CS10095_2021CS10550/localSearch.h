#ifndef LOCALSEARCH_H
#define LOCALSEARCH_H


#include <fstream>
#include <iostream>
#include <random>
#include <bits/stdc++.h>
using namespace std;



class localSearch
{
private:
    int z, l;
    int **T;
    int **N;
    float time;
    vector<int> mapping;
    long long curr;
    long long best;
    vector<int> bestMapping;


public:
    localSearch(string inputfilename);
    // ~localSearch();
    bool check_output_format();

    long long cost_fn();
    long long cost_fn(vector<int> &arr);
    long long gain(int i, int j);

    void write_to_file(string outputfilename);

    void readInInputFile(string inputfilename);

    void compute_allocation(std::chrono::high_resolution_clock::time_point curr);


    // 0 means no allocation of zone to that location !
    // 0 base indexing followed !
    vector<int> randomPermutation();

    float getTime();

    long long bestTime();

    vector<int> findNextNeighbour();

};





#endif // !LOCALSEARCH_H