#include <iostream>
#include <bits/stdc++.h>
#include <fstream>
#include "./models/SportsLayout.h"
#include "./models/localSearch.h"


using namespace std;


int main(int argc, char** argv )
{

     // Parse the input.
    

    if ( argc < 3 )
    {   
        cout<<"Missing arguments\n";
        cout<<"Correct format : \n";
        cout << "./main <input_filename> <output_filename>";
        exit ( 0 );
    }
    string inputfilename ( argv[1] );
    string outputfilename ( argv[2] );
    
    // SportsLayout *s  = new SportsLayout( inputfilename );
    // s->compute_allocation();
    // s->write_to_file(outputfilename);

    // long long cost = s->cost_fn ();
    // cout<< "cost:"<<cost<<endl;



    localSearch *l = new localSearch(inputfilename);
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    std::chrono::high_resolution_clock::time_point curr = std::chrono::high_resolution_clock::now();
    int maxTime = l->getTime()*1000;
    int iteration = 0;
    while (std::chrono::duration_cast<std::chrono::milliseconds>(curr - start).count() < maxTime)
    {
        l->compute_allocation();
        curr = std::chrono::high_resolution_clock::now();
        ++iteration;
    }
    l->write_to_file(outputfilename);
    long long cost = l->bestTime();
    cout << inputfilename << endl;
    cout << "cost:" << cost << endl;
    cout << "iterations:" << iteration << endl;
    return 0;

}