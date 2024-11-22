#ifndef Graph_h
#define Graph_h
#include <List.hpp> 
 

class Graph {
    int numVertices;
    List<int>* adjLists;
    bool* visited;

  public:
      Graph(int vertices);
      void addEdge(int src, int dest);
      void BFS(int startVertex);
};

#endif