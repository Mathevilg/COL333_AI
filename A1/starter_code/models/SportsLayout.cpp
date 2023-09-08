#include <fstream>
#include <iostream>
#include <random>
#include <chrono>
#include <bits/stdc++.h>
using namespace std;

#include "SportsLayout.h"

    SportsLayout::SportsLayout(string inputfilename)
    {
          
        readInInputFile(inputfilename);
        mapping = generateRandomPermutation(l);
        // printVector(mapping);
        bestMapping = mapping;
        best = cost_fn();

    }

    vector<int> SportsLayout::generateRandomPermutation(int n) {
        std::vector<int> permutation(n);
        for (int i = 0; i < n; ++i) {
            permutation[i] = i + 1; // Initialize with consecutive integers
        }

        // Use random_device to seed the random number generator
        std::random_device rd;
        std::mt19937 gen(rd());

        shuffle(permutation.begin(), permutation.end(), gen);

        return permutation;
    }

    bool SportsLayout::check_output_format()
    {

        vector<bool> visited(l,false);
        for(int i=0;i<z;i++)
        {
            if((mapping[i]>=1 && mapping[i]<=l))
            {
                if(!visited[mapping[i]-1])
                visited[mapping[i]-1]=true;
                else
                {
                    cout<<"Repeated locations, check format\n";
                    return false;
                }
            }
            else
            {
                cout<<"Invalid location, check format\n";
                return false;
            }
        }

        return true;

    }

    void SportsLayout::readInInputFile(string inputfilename)
    {
            fstream ipfile;
	        ipfile.open(inputfilename, ios::in);
            if (!ipfile) {
                cout << "No such file\n";
                exit( 0 );
            }
            else {
                

                ipfile>> time;
                ipfile >> z;
                ipfile >> l;

                if(z>l)
                {
                    cout<<"Number of zones more than locations, check format of input file\n";
                    exit(0);
                }


            

            int **tempT;
            int **tempN;

          tempT = new int*[l];
         for (int i = 0; i < l; ++i)
            tempT[i] = new int[l];
        
        tempN = new int*[z];
        for (int i = 0; i < z; ++i)
            tempN[i] = new int[z];

        for(int i=0;i<z;i++)
        {
            for(int j=0;j<z;j++)
            ipfile>>tempN[i][j];
        }

        for(int i=0;i<l;i++)
        {
            for(int j=0;j<l;j++)
            ipfile>>tempT[i][j];
        }

        ipfile.close();

        T= tempT;
        N= tempN;
            }

    }

    void SportsLayout::write_to_file(string outputfilename){

         // Open the file for writing
    ofstream outputFile(outputfilename);

    // Check if the file is opened successfully
    if (!outputFile.is_open()) {
        cerr << "Failed to open the file for writing." << std::endl;
        exit(0);
    }

    for(int i=0;i<z;i++)
    outputFile<<mapping[i]<<" ";

    // Close the file
    outputFile.close();

    cout << "Allocation written to the file successfully." << endl;

    }

    int SportsLayout::getTime()
    {
        return time;
    }

    long long SportsLayout::cost_fn(){


        long long cost=0;

        for(int i=0;i<z;i++)
        {
           for(int j=0;j<z;j++) 
           {
                cost+=(long long)N[i][j]*(long long)T[mapping[i]-1][mapping[j]-1];
           }
        }

        return cost;
    }

    long long SportsLayout::cost_fn_best(){


        long long cost=0;

        for(int i=0;i<z;i++)
        {
           for(int j=0;j<z;j++) 
           {
                cost+=(long long)N[i][j]*(long long)T[bestMapping[i]-1][bestMapping[j]-1];
           }
        }

        return cost;
    }

    long long SportsLayout::gain(int i0, int j0){
        long long gained = 0;
        if (j0 < z) {
            for (int i=0; i<z; i++){
                if(i==i0) continue;
                gained -= N[i][j0]*T[mapping[i]-1][mapping[j0]-1];
                gained -= N[j0][i]*T[mapping[j0]-1][mapping[i]-1];
            }
            for (int j=0; j<z; j++){
                gained -= N[j][i0]*T[mapping[j]-1][mapping[i0]-1];
                gained -= N[i0][j]*T[mapping[i0]-1][mapping[j]-1];
            }
            swap(mapping[i0],mapping[j0]);
            for (int i=0; i<z; i++){
                if(i==i0) continue;
                gained += N[i][j0]*T[mapping[i]-1][mapping[j0]-1];
                gained += N[j0][i]*T[mapping[j0]-1][mapping[i]-1];
            }
            for (int j=0; j<z; j++){
                gained += N[j][i0]*T[mapping[j]-1][mapping[i0]-1];
                gained += N[i0][j]*T[mapping[i0]-1][mapping[j]-1];
            }
            swap(mapping[i0],mapping[j0]);
        }
        // if (j0 < z) {
        //     for (int i=0; i<z; i++) {
        //         if(i!=i0){
        //                 gained -= N[i][j0]*T[mapping[i]-1][mapping[j0]-1];
        //                 gained += N[i][j0]*T[mapping[i]-1][mapping[i0]-1];
        //             }
        //         }
        //     for (int j=0; j<z; j++) {
        //         if(j!=j0){
        //                 gained -= N[i0][j]*T[mapping[i0]-1][mapping[j]-1];
        //                 gained += N[i0][j]*T[mapping[j0]-1][mapping[j]-1];
        //         }
        //     }
        //     gained -= N[i0][j0]*T[mapping[i0]-1][mapping[j0]-1];
        //     gained += N[i0][j0]*T[mapping[j0]-1][mapping[i0]-1];
        // }
        else {
            for (int i=0; i<z; i++){
                if(i==i0) continue;
                gained -= N[i][i0]*T[mapping[i]-1][mapping[i0]-1];
                gained -= N[i0][i]*T[mapping[i0]-1][mapping[i]-1];
            }
            swap(mapping[i0],mapping[j0]);
            for (int j=0; j<z; j++){
                if(j==i0)continue;
                gained += N[i0][j]*T[mapping[i0]-1][mapping[j]-1];
                gained += N[j][i0]*T[mapping[j]-1][mapping[i0]-1];
            }
            swap(mapping[i0],mapping[j0]);
        }
    
        return gained;
    }

    long long SportsLayout::findNextNeighbor()
    {
        long long mini = 0;
        // vector<int> bestNeighbour = mapping;

        // for (int x=0; x<l; x++){cout << bestNeighbour[x] << " ";}
        // cout << endl;

        int best_i = 0;
        int best_j = 0;
        for (int i=0; i<z; i++)
        {
            for (int j=i+1; j<l; j++)
            {   
                // cout << "here" << i << " " << j << "\n";
                long long gained = gain(i,j);
                // cout << "gained " << gained << "\n"; 
                if (gained < mini)
                {
                    mini = gained;
                    best_i = i;
                    best_j = j;
                } 
                // cout << "done!\n";
            }
        }
        // cout << "best i and j" << best_i << " " << best_j << "\n";
        
        if (mini < 0){swap(mapping[best_i], mapping[best_j]);}
        // cout << "mapping after swapping is :  ";
        // for (int x=0; x<l; x++){cout << mapping[x] << " ";}
        // cout << endl;

        return mini;
        
    }

    void SportsLayout::compute_allocation()
    {
        mapping = generateRandomPermutation(l);
        long long currCost = cost_fn();
        // cout << "Printing the initial randomstate to begin with search\n" ;
        // for (int x=0; x<l; x++){cout << mapping[x] << " ";}
        // cout << endl;
        // reach greedily until local minima 
        // while (true){            
        //     // cost of this state;
        //     // cout << currCost << "\n";
        //     long long gained = findNextNeighbor();

        //     // for (int x=0; x<l; x++){cout << nextNeighbour[x] << " ";}
        //     // cout << endl;

        //     vector<int> tempMapping = mapping;
        //     mapping = nextNeighbour;
        //     long long bestNeighbourCost = cost_fn();
        //     if (bestNeighbourCost >= currCost) 
        //     {
        //         mapping = tempMapping;
        //         break;
        //     }

        // }
        int count = 0;
        // int bestt_i = 0;
        // int bestt_j = 0;
        while (true){            
             // cost of this state;
            // cout << currCost << "\n";
            long long gained = findNextNeighbor();
            // cout << "gained " << gained << "\n\n";
            if (gained == 0) break ; // reached local max !
            currCost += gained;
            count++;
        }
        if (currCost < best) 
        {
            bestMapping = mapping;
            best = currCost;
        } 

        // cout << "best mapping in this iteration has cost \n";
        //  for (int x=0; x<l; x++){cout << mapping[x] << " ";}
        //     cout << endl;
        // long long localOptimum = cost_fn();
        // // cout << localOptimum << "\n";
        // if (localOptimum < best) 
        // {
        //     bestMapping = mapping;
        //     best = localOptimum;
        // } 
  
    }


