/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package my.contacteditor;

/**
 *
 * @author sethcampbell150
 */
// Tile class to represent each tile in the town matrix
class Tile {
    private int roadType;
    private int rotation;
    private boolean hasDestination;
    
    // Other properties as needed

    // Constructor
    public Tile(int roadType, int rotation, boolean hasDestination) {
        this.roadType = roadType;
        this.rotation = rotation;
        this.hasDestination = hasDestination;
    }

    // Getters and setters
    public int getRoadType() {
        return roadType;
    }

    public void setRoadType(int roadType) {
        this.roadType = roadType;
    }

    public int getRotation() {
        return rotation;
    }

    public void setRotation(int rotation) {
        this.rotation = rotation;
    }

    public boolean isHasDestination() {
        return hasDestination;
    }

    public void setHasDestination(boolean hasDestination) {
        this.hasDestination = hasDestination;
    }

    // Other methods as needed
}
