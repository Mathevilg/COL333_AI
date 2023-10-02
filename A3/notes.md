## Part1
k1 and k2 size cliques finding in an undirected graph
steps 

1. read the graph
2. convert to CNF sat problem (`time and size of encoding in polynomial`)
3. give encoding to minisat 
4. take assignment from minisat and then complete to 2 complete subgraphs
#### Note: `only one call to minisat`.




## Part2


Find maximum size of clique

1. read a graph
2. convert to CNF (`time and size of encoding in polynomial`)
3. give encoding to minisat 
4. take sat encoding and convert to complete subgraph

#### Note: `multiple calls are allowed to minisat`



## Note: ` Your code will be killed after the given problem cutoff time, so please write good code.`



## Done !!
#### How to run ??
1. `./compile.sh`
2. `./run1.sh`
3. `minisat out_1.txt satOut1.txt` This name satOut1.txt have to be changed as per the assignment !!
4. `./run2.sh`
5. `./run3.sh`


## what is left ??
1. check for bigger testcases by creating file checker
2. check if optimality can be found !! (`Devang`)


## how to check for part 1 ??
1. run `make`.
2. provide the parameters `n`, `k1`, `k2` in the terminal
3. wait and check output :)


