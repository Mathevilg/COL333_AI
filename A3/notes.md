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
