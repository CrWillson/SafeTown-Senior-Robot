#include "SoftwareMap.h"

//Create a new Software Map
SoftwareMap::SoftwareMap(int** arr, int col, int row) : myArray(nullptr), col(col), row(row) {
    // Allocate memory for the dynamic 2D array
    myArray = new int*[row];
    for (int i = 0; i < row; ++i) {
        myArray[i] = new int[col];
        for (int j = 0; j < col; ++j) {
            myArray[i][j] = arr[i][j];
        }
    }
}


SoftwareMap::~SoftwareMap() {
    // Deallocate the dynamically allocated memory for the 2D array
    if (myArray != nullptr) {
        for (int i = 0; i < row; ++i) {
            delete[] myArray[i];
        }
        delete[] myArray;
    }
}


// Add edges to the SoftwareMap
void SoftwareMap::addEdge(int col, int row) {

}

// BFS algorithm
void SoftwareMap::BFS(int startVertex) {
    visited = new bool[numVertices];
    for (int i = 0; i < numVertices; i++){
        visited[i] = false;
    }
    List<int> queue;

    visited[startVertex] = true;
    queue.add(startVertex);

    
    while (!queue.isEmpty()) {
        
        int currVertex = queue.getValue(0);
        queue.removeFirst();

        for (int i = 0; i != 0; ++i) {
            int adjVertex = i;
            if (!visited[adjVertex]) {
                visited[adjVertex] = true;
                queue.add(adjVertex);
            }
        }
        
    }
}

int SoftwareMap::findTileType(int reqRow, int reqCol){
  return(this->myArray[reqRow][reqCol]);
}

