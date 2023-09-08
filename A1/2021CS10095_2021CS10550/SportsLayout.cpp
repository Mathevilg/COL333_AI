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

    // cout << "Allocation written to the file successfully." << endl;

    }

    float SportsLayout::getTime()
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
                gained -= N[i][j0]*T[mapping[i]-1][mapping[j0]-1];
                gained -= N[j0][i]*T[mapping[j0]-1][mapping[i]-1];
                gained -= N[i][i0]*T[mapping[i]-1][mapping[i0]-1];
                gained -= N[i0][i]*T[mapping[i0]-1][mapping[i]-1];
            }
            swap(mapping[i0],mapping[j0]);
            for (int i=0; i<z; i++){
                gained += N[i][j0]*T[mapping[i]-1][mapping[j0]-1];
                gained += N[j0][i]*T[mapping[j0]-1][mapping[i]-1];
                gained += N[i][i0]*T[mapping[i]-1][mapping[i0]-1];
                gained += N[i0][i]*T[mapping[i0]-1][mapping[i]-1];
            }
            swap(mapping[i0],mapping[j0]);
        }
        else {
            for (int i=0; i<z; i++){
                gained -= N[i][i0]*T[mapping[i]-1][mapping[i0]-1];
                gained -= N[i0][i]*T[mapping[i0]-1][mapping[i]-1];
            }
            swap(mapping[i0],mapping[j0]);
            for (int j=0; j<z; j++){
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
        // vector<int> i_range = generateRandomPermutation(z);
        // vector<int> j_range = generateRandomPermutation(l);
        
        
        int done = 0;
        for (int i=0; i<z; i++)
        {
            for (int j=i+1; j<l; j++)
            {   
                if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start_time).count() > maxTime)
                {return 0;}
                // cout << "here" << i << " " << j << "\n";
                // long long gained = gain(i_range[i]-1,j_range[j]-1);
                long long gained = gain(i,j);
                // cout << "gained " << gained << "\n"; 
                if (gained < mini)
                {
                    mini = gained;
                    // best_i = i_range[i]-1;
                    // best_j = j_range[j]-1;
                    best_i = i;
                    best_j = j;
                    done = 1;
                    break;
                } 
                // cout << "done!\n";
            }
            if (done == 1) {break;}
        }
        // cout << "best i and j" << best_i << " " << best_j << "\n";
        
        swap(mapping[best_i], mapping[best_j]);
        return mini;
        // cout << "mapping after swapping is :  ";
        // for (int x=0; x<l; x++){cout << mapping[x] << " ";}
        // cout << endl;
    }


    // long long SportsLayout::findNextNeighbor()
    // {   
    //     if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start_time).count() < (time-0.1)){return 0;}
    //     long long mini = 0;
    //     // vector<int> bestNeighbour = mapping;

    //     // for (int x=0; x<l; x++){cout << bestNeighbour[x] << " ";}
    //     // cout << endl;

    //     int best_i = 0;
    //     int best_j = 0;
    //     // vector<int> i_range = generateRandomPermutation(z);
    //     // vector<int> j_range = generateRandomPermutation(l);
    //     for (int i=0; i<z; i++)
    //     {
    //         for (int j=i+1; j<l; j++)
    //         {   
    //             // cout << "here" << i << " " << j << "\n";
    //             // long long gained = gain(i_range[i]-1,j_range[j]-1);
    //             long long gained = gain(i,j);
    //             // cout << "gained " << gained << "\n"; 
    //             if (gained < mini)
    //             {
    //                 mini = gained;
    //                 // best_i = i_range[i]-1;
    //                 // best_j = j_range[j]-1;
    //                 best_i = i;
    //                 best_j = j;
    //             } 
    //             // cout << "done!\n";
    //         }
    //     }
    //     // cout << "best i and j" << best_i << " " << best_j << "\n";
        
    //     swap(mapping[best_i], mapping[best_j]);
    //     // cout << "mapping after swapping is :  ";
    //     // for (int x=0; x<l; x++){cout << mapping[x] << " ";}
    //     // cout << endl;

    //     return mini;
        
    // }

    void SportsLayout::compute_allocation()
    {          
            // if(iterations<100){mapping=generateRandomPermutation(l);}
            // else{
            //     random_device rd;
            //     mt19937 gen(rd());
            //     std::uniform_int_distribution<int> distribution(0, 30);
            //     mapping = bestMapping;
            //     int random_integer = distribution(gen);
            //     // cout << "rand " << random_integer << endl;
            //     vector<int> i_range = generateRandomPermutation(z);
            //     vector<int> j_range = generateRandomPermutation(l);
            //     int i=0;
            //     int j=0;
            //     int c=0;
            //     while (i<z && c<random_integer){
            //         j = i+1;
            //         while(j<l && c<random_integer){
            //             swap(mapping[i_range[i]-1], mapping[j_range[j]-1]);
            //             c++;
            //             j++;
            //         }
            //         i++;
            //     }
            // }

            mapping=generateRandomPermutation(l);
            // reverse(bestMapping.begin(), bestMapping.end());
            // mapping = generateRandomPermutation(l);
            // mapping = bestMapping;
            // reverse(bestMapping.begin(), bestMapping.end());
            long long currCost = cost_fn();
            while (true){            
                // cost of this state;
                // cout << currCost << "\n";
                long long gained = findNextNeighbor();
                // cout << "gained " << gained << "\n\n";
                if (gained == 0) break ; // reached local max !
                currCost += gained;
            }
            // cout << " curr cost in this iteration " << currCost << "\n";
            if (currCost < best) 
            {
                bestMapping = mapping;
                best = currCost;
            } 
  
    }


