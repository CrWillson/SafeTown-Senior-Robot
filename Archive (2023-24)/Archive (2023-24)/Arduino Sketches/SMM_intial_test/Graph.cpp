#include "Graph.h"


Graph::Graph(int vertices) {
    numVertices = vertices;
    adjLists = new List<int>[vertices];
}
// Add edges to the graph
void Graph::addEdge(int src, int dest) {
    adjLists[src].add(dest);
    adjLists[dest].add(src);
}

// BFS algorithm
void Graph::BFS(int startVertex) {
    visited = new bool[numVertices];
    for (int i = 0; i < numVertices; i++)
        visited[i] = false;

    List<int> queue;

    visited[startVertex] = true;
    queue.add(startVertex);

    
    while (!queue.isEmpty()) {
        
        int currVertex = queue.getValue(0);
        queue.removeFirst();

        for (int i = adjLists->getValue(currVertex); i != adjLists->getValue(adjLists->getSize()); ++i) {
            int adjVertex = i;
            if (!visited[adjVertex]) {
                visited[adjVertex] = true;
                queue.add(adjVertex);
            }
        }
        
    }
}