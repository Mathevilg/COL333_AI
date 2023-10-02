import random

def generate_graph(n, k1, k2, density = 0.85):
    if n < k1 + k2 or k1 <= 0 or k2 <= 0:
        print("Invalid input. Unable to generate the graph.")
        return [None, None, None]

    # Create two separate cliques
    graph = [[0] * n for _ in range(n)]

    arr = [i for i in range(n)]
    set1 = random.sample(arr, k1)
    arr2 = [i for i in arr if i not in set1]
    set2 = random.sample(arr2, k2)
    # print(set1)
    # print(set2)
    
    
    # making the cliques !!
    for x in set1:
        for y in set1:
            if (x == y):
                continue
            graph[x][y] = 1
            graph[y][x] = 1
            
    for x in set2:
        for y in set2:
            if (x == y):
                continue
            graph[x][y] = 1
            graph[y][x] = 1
            
            
            
    #  add addition edges !!
    for i in range(n):
        for j in range(i+1, n):
            if (graph[i][j] == 0):
                if (random.random() < density):
                    graph[i][j] = 1
                    graph[j][i] = 1
                    
    for i in range(len(set1)):
        set1[i] += 1
    for i in range(len(set2)):
        set2[i] += 1
                
    return [graph, set1, set2]

def write_graph_to_file(graph, filename, k1, k2):
    if graph is None:
        return

    n = len(graph)
    m = sum(sum(row) for row in graph) // 2  # Counting edges

    with open(filename, 'w') as file:
        file.write(f"{n} {m} {k1} {k2}\n")
        for i in range(n):
            for j in range(i + 1, n):
                if graph[i][j]:
                    file.write(f"{i + 1} {j + 1}\n")




# generate_graph(10, 4, 3)

if __name__ == "__main__":
    n = int(input("Enter the total number of vertices (n): "))
    k1 = int(input("Enter the size of the first clique (k1): "))
    k2 = int(input("Enter the size of the second clique (k2): "))
    
    data = generate_graph(n, k1, k2)
    graph = data[0]
    set1 = data[1]
    set2 = data[2]
    
    if graph:
        filename = "test.graphs"
        write_graph_to_file(graph, filename, k1, k2)
        
        print(f"Graph written to '{filename}'.")
        
        with open(filename, 'a') as file:
            file.write("\n\n\n\n\n\n\n\n")
            file.write(str(sorted(set1)) + "\n")
            file.write(str(sorted(set2)) + "\n")
            