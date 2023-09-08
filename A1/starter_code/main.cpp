#include <iostream>
#include <random>
#include <chrono>
#include <fstream>
#include <bits/stdc++.h>

#include "./models/SportsLayout.h"

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
    
    SportsLayout *s  = new SportsLayout( inputfilename );

    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    std::chrono::high_resolution_clock::time_point curr = std::chrono::high_resolution_clock::now();
    int maxTime = s->getTime()*1000;
    int iteration = 0;
    while (std::chrono::duration_cast<std::chrono::milliseconds>(curr - start).count() < maxTime)
    {
        s->compute_allocation();
        curr = std::chrono::high_resolution_clock::now();
        ++iteration;
    }
    s->write_to_file(outputfilename);

    long long cost = s->cost_fn_best ();
    cout<< "cost:"<<cost<<endl;
    cout << "iterations:" << iteration << endl;

    return 0;

}