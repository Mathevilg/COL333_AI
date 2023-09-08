#include <iostream>
#include <random>
#include <chrono>
#include <fstream>
#include <bits/stdc++.h>

#include "SportsLayout.h"

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

    s->start_time = std::chrono::high_resolution_clock::now();
    // s->tempmapping = s->generateRandomPermutation(s->l);
    if (s->getTime() <= 0.2) {s->maxTime = (s->getTime()-0.045)*(float)60000;}
    else {s->maxTime = (s->getTime()-0.07)*(float)60000;}
    // s->bestMapping = s->generateRandomPermutation(s->l);
    // s->iterations = 0;
    s->write_to_file(outputfilename);
    while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - s->start_time).count() < s->maxTime)
    {
        s->compute_allocation();
        // s->iterations++;
    }
    s->write_to_file(outputfilename);

    // long long cost = s->cost_fn_best ();
    // cout<< "cost:"<<cost<<endl;
    // cout << "iterations:" << s->iterations << endl;

    return 0;
}