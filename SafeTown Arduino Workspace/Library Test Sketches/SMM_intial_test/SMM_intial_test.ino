
#include "Graph.h"
#include "SoftwareMap.h"
    //This is the setup for the town map
    const int col = 4;
    const int row = 3;
    int setTownMap[row][col] = {
      {1, 2, 3, 4}, 
      {5, 6, 7, 8}, 
      {4, 3, 2, 1}};

    //This sets up the starting tile of the car       
    int startingRow = 0;
    int startingCol = 0;

void setup() {
  Serial.begin(9600);

}

void loop() {
  delay(1000);
  int** townMap = new int*[row];
  for (int i = 0; i < row; ++i) {
    townMap[i] = new int[col];
    
    for (int j = 0; j < col; ++j){
      townMap[i][j] = setTownMap[i][j];
    }

  }
  
  //int townMap[row][col] = {{1, 2, 3, 4}, {5, 6, 7, 8}, {4, 3, 2, 1}};
  SoftwareMap myMap(townMap, col, row);

  for (int i = 0; i < row; ++i) {
    delete[] townMap[i];
  }
  delete[] townMap;

  for (int i = 0; i < row; ++i){
    for (int j = 0; j < col; ++i){
      Serial.println(myMap.findTileType(i, j));
    }
  }



}









