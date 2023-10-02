## Part1
k1 and k2 size cliques finding in an undirected graph
steps 

1. read the graph
2. convert to CNF sat problem (`time and size of encoding in polynomial`)
3. give encoding to minisat 
4. take assignment from minisat and then complete to 2 complete subgraphs
#### Note: 
`only one call to minisat is allowed`.




## Part2


Find maximum size of clique

1. read a graph
2. convert to CNF (`time and size of encoding in polynomial`)
3. give encoding to minisat 
4. take sat encoding and convert to complete subgraph

#### Note: 
`multiple calls are allowed to minisat`



### Note: 
`Your code will be killed after the given problem cutoff time, so please write good code.`



## Done !!
#### How to run ??
1. run `make` in terminal.
2. Enjoy :)


## what is left ??
1. (Done) check for bigger testcases by creating file checker -> testing left !!
2. (Done)check if optimality can be found !!
3. remove chat gpt codes for I/O


## how to check for part 1 and part 2 ??
1. run `make`.
2. provide the parameters `n`, `k1`, `k2` in the terminal
3. wait and check output :)


## Scope of improvement 
1. part2 is taking more time in cases of n ~ 150. Investigate that wether this is because of writing I/O multiple times or what. In `part2.cpp` the updation of `ans` vector can be done once the `max_size_clique` is found.
2. `part1` seems good enough.
3. maybe and just maybe the ordering of variables in the `*.satinput` may decrease execution time !!

