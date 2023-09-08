#include <bits/stdc++.h>
#include "./localSearch.h"

using namespace std;


int main(int argc, char** argv )
{


    if ( argc < 3 )
    {   
        cout<<"Missing arguments\n";
        cout<<"Correct format : \n";
        cout << "./main <input_filename> <output_filename>";
        exit ( 0 );
    }
    string inputfilename ( argv[1] );
    string outputfilename ( argv[2] );
    


    localSearch *l = new localSearch(inputfilename);
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    std::chrono::high_resolution_clock::time_point curr = std::chrono::high_resolution_clock::now();
    float maxTime = l->getTime()*985;
    int iteration = 0;
    while (std::chrono::duration_cast<std::chrono::milliseconds>(curr - start).count() < maxTime)
    {
        l->compute_allocation(curr);
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