////////////////////////////////////////////////////////////////////////////////
// Filename: SafeTownDisplay.h
// Created: 10-1-2024
// Modified: 11-22-2024
// Author: Benjamin Kinard (SafeTown)
// Purpose: Header file for the SafeTownDisplay class
////////////////////////////////////////////////////////////////////////////////

#ifndef SafeTownDisplay_h
#define SafeTownDisplay_h

#include "Arduino.h"
#include "MenuItem.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class SafeTownDisplay {
  public:
    // Setup and constructors
    SafeTownDisplay();
    void setup();

    // Display and robot hardware helper functions
    void clearDisplay();
    void printToScreen(String s);
    void encoderPress();
    void encoderLeft();
    void encoderRight();

    // Main menu setup and display
    void menuSetup();
    void displayMenu(bool updateScreen);

    // Debugging
    void setCurrEMA(float EMA);

    // Speed Control
    void setSpeed(int speedVal);
    int getSpeed();

    // MenuItem action functions
    static void displayIRValues(SafeTownDisplay* displayLibInst);
    static void displayDebugValues(SafeTownDisplay* displayLibInst);
    static void collectData(SafeTownDisplay* displayLibInst);
    static void outputData(SafeTownDisplay* displayLibInst);
    static void toggleData(SafeTownDisplay* displayLibInst);
    static void changeIntData(SafeTownDisplay* displayLibInst);

    // Menu variables
    MenuItem mainMenu; // the main (root) MenuItem
    MenuItem* currentMenu = &mainMenu;
    int numLines; // the number of lines of text on the display
    int currentIndex = 0; // currently selected MenuItem index
    bool updateScreen = true; // holds the most recent value of whether to refresh the display screen passed to displayMenu()
    bool runSelectedAction = false; // denotes whether an action is currently being run for a MenuItem
    bool encoderAction = false; // denotes whether the currently run action is an encoder input action

    // IR pin values
    const static int down_ir_pin = 29;
    const static int front_ir_pin = 28;
    const static int in_ir_pin = 27;
    const static int out_ir_pin = 26;

    // Data collection variables
    MenuItem* settingSampleRate; // pointer to the sample rate setting MenuItem
    MenuItem* settingSampleTime; // pointer to the sample time setting MenuItem
    MenuItem* settingDownIR; // pointer to the down IR sensor setting MenuItem
    MenuItem* settingFrontIR; // pointer to the front IR sensor setting MenuItem
    MenuItem* settingInnerLeftIR; // pointer to the inner left IR sensor setting MenuItem
    MenuItem* settingOuterLeftIR; // pointer to the outer left IR sensor setting MenuItem
    double timeOffset = 0; // timestamp at the start of data collection
    int samplePointer = 0; // index of the next sample to collect
    const static int NUM_SAMPLES = 1000; // maximum number of samples that can be stored
    int times[NUM_SAMPLES] = {}; // timestamps for samples
    int downSamples[NUM_SAMPLES] = {}; // down IR sensor samples
    int frontSamples[NUM_SAMPLES] = {}; // front IR sensor samples
    int innerLeftSamples[NUM_SAMPLES] = {}; // inner left IR sensor samples
    int outerLeftSamples[NUM_SAMPLES] = {}; // outer left IR sensor samples
    int sampleTargetTime = 0; // timestamp to collect next sample
    bool resetData = true; // reset data collection
    int samplesToCollect; // number of samples to collect (calculated based on current settings)
    int sampleRate; // current sample rate (specified by setting)
    int sampleTime; // current sample time (specified by setting)
    double sampleInterval; // current sample interval (calculated based on current settings)

    // Debugging variables
    float currEMA = 0.0; // Exponential Moving Average, the value based on the front sensor reading used for traffic navigation

    // Speed variables
    MenuItem* settingSpeed; // pointer to the speed setting MenuItem
};
#endif