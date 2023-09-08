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
    int time;
    vector<int> seed;
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

    void compute_allocation();


    // 0 means no allocation of zone to that location !
    // 0 base indexing followed !
    vector<int> randomPermutation();

    int getTime();

    long long bestTime();

    vector<int> findNextNeighbour();

};





#endif // !LOCALSEARCH_H