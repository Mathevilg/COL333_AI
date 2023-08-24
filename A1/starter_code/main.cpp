#include <iostream>
#include <bits/stdc++.h>
#include <fstream>
#include "SportsLayout.h"
#include "localSearch.h"


using namespace std;

int main(int argc, char** argv )
{

     // Parse the input.
    
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

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
    std::chrono::duration<int> timeInChrono(l->getTime());
    std::chrono::high_resolution_clock::time_point curr = std::chrono::high_resolution_clock::now();
    std::chrono::high_resolution_clock::time_point maxTime = start + timeInChrono;
    int i = 0;
    while (i < 100)
    {

        l->compute_allocation();
        curr = std::chrono::high_resolution_clock::now();
        i++;
    }
    l->write_to_file(outputfilename);
    long long cost = l->bestTime();
    cout << "cost:" << cost << endl;
    return 0;

}