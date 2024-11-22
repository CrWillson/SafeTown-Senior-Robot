#ifndef SoftwareMap_h
#define SoftwareMap_h
#include <List.hpp> 
 

class SoftwareMap {
  private:

    int** myArray; 
    int currentCol;
    int currentRow;
    int col;
    int row;
    int numVertices;
    int* startVal;
    int* endVal;
    bool* visited;
    enum tileType{ 
    straight = 1,
    curve1 = 2, 
    curve2 = 3, 
    threeWay1 = 4,
    threeWay2 = 5, 
    fourWay1 = 6, 
    fourWay2 = 7, 
    parking = 8, 
    blankSpace = 9};

  public:
      SoftwareMap(int** arr, int col, int row);
      ~SoftwareMap();
      void addEdge(int col, int row);
      void BFS(int startVertex);
      int findTileType(int reqCol, int reqRow);
      void Directions(int);

  
};

#endif